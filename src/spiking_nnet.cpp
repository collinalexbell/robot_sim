#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "spiking_nnet.h"
#include "../libs/json.hpp"



void Neuron::connect(Neuron *from, Neuron *to, double weight, int speed){
    //Notify from of to
    from->outputs.push_back(to);

    //Set the weight and speed in the to
    //Weight
    std::pair<Neuron*, double> weight_pair;
    weight_pair = std::make_pair(from, weight);
    to->weights.insert(weight_pair);

    //Speed
    std::pair<Neuron*, int> speed_pair;
    speed_pair = std::make_pair(from, speed);
    to->speeds.insert(speed_pair);
}

double Neuron::weight_for(Neuron *input){
    return weights.at(input);
}

int Neuron::speed_for(Neuron *input){
    return speeds.at(input);
}

Neuron* Neuron::output_at(int position){
    return outputs.at(position);
}

void Neuron::stimulate(Neuron* n){
    n->recieve_stimulation(this, output_strength);
}

void Neuron::recieve_stimulation(Neuron* n, double stimulation_strength){
    double incoming = weight_for(n) * stimulation_strength;
    double speed = speed_for(n);

    while(incoming_stimuli.size() < speed){
        incoming_stimuli.push_back(new std::vector<double>());
    }
    if(incoming_stimuli.size() == speed){
        //This will fire if there is no deque entry for this step
        std::vector<double>* insert_me = new std::vector<double>();
        insert_me->push_back(incoming);
        incoming_stimuli.push_back(insert_me);
    }
    else{
        incoming_stimuli[speed]->push_back(incoming);
    }
}

void Neuron::step(){
   handle_stimulation_step(); 
   stimulate_step();
    
}

void Neuron::handle_stimulation_step(){
    int i;
    //Handle incoming stimulation
    if(incoming_stimuli.size() > 0){
        std::vector<double> *incoming = incoming_stimuli[0];
        for( auto it = incoming->begin(); it != incoming->end(); it++){
            //std::cout<<"Incoming: " << *it<<std::endl;
            stimulation += *it;
        }

        delete incoming;
        //2 pointers are pointing to this deleted vector
        incoming = NULL;
        incoming_stimuli.pop_front();
        //Taken care of
    }
}

void Neuron::stimulate_step(){
    if( stimulation >= threshold ){
        stimulation = 0;
        for( auto it = outputs.begin(); it != outputs.end(); it++ ){
            stimulate(*it);
        }
    }
    stimulation *= (1 - stim_decay);
}

Neuron* Spiking_NNet::add_neuron(){
    Neuron* rv = new Neuron();
    neurons.push_back(rv);
    return rv;
}

Neuron* Spiking_NNet::add_neuron(std::string name){
    Neuron* rv = add_neuron(); 
    std::pair<std::string, Neuron*> insert_me;
    std::pair<Neuron*, std::string> insert_me_inverse;
    insert_me = std::make_pair(name, rv);
    insert_me_inverse = std::make_pair(rv, name);
    named_neurons.insert(insert_me);
    neurons_to_name.insert(insert_me_inverse);
    return rv;
}

Neuron* Spiking_NNet::add_neuron(std::string name, double thresh, double decay){
    Neuron *rv = add_neuron(thresh, decay);
    std::pair<std::string, Neuron*> insert_me;
    std::pair<Neuron*, std::string> insert_me_inverse;
    insert_me = std::make_pair(name, rv);
    insert_me_inverse = std::make_pair(rv, name);
    named_neurons.insert(insert_me);
    neurons_to_name.insert(insert_me_inverse);
    return rv;
}

Neuron* Spiking_NNet::add_neuron(double thresh, double decay){
    Neuron* rv = new Neuron(thresh, decay);
    neurons.push_back(rv);
    return rv;
}

Neuron* Spiking_NNet::get_neuron(std::string name){
    try{
        return named_neurons.at(name);
    }
    catch(...){
        throw std::invalid_argument("Named neuron does not exist");
    }
}


std::vector<Neuron*> Spiking_NNet::get_neurons(){
    return neurons;
}

void Spiking_NNet::step(){
    for( auto it=neurons.begin(); it != neurons.end(); it++ ){
        (*it)->step();
    }
}

Spiking_NNet::Spiking_NNet(){
}

Spiking_NNet::Spiking_NNet(std::string json_text){

    using json = nlohmann::json;
    double rnd_buffer;
    std::vector<json> connections;
    std::string to_str;
    std::string name;
    double decay, threshold, weight;
    int to_int, speed;
    Neuron *out_n;

    /* initialize random seed: */
    srand (time(NULL));

    json net_data = json::parse(json_text);
    json json_neurons = net_data["neurons"];

    //Create a neuron for each representation
    for( auto it = json_neurons.begin(); it != json_neurons.end(); it++ ){
        //Handle decay
        if( (*it)["decay"].is_string() ){
            //Special assignment
            if( (*it)["decay"].get<std::string>() == "random" ){
                rnd_buffer = rand()/double(RAND_MAX);
                decay = rnd_buffer;
            }
        }
        else{
            //Assume it is a value
            decay = (*it)["decay"].get<double>();
        }

        //Handle threshold
        if( (*it)["threshold"].is_string() ){
            //Special assignment
            if( (*it)["threshold"].get<std::string>() == "random" ){
                rnd_buffer = rand()/double(RAND_MAX);
                threshold = rnd_buffer;
            }
        }
        else{
            //Assume it is a value
            decay = (*it)["threshold"].get<double>();
        }


        //Check for name and add neuron
        if ( it->count("name") == 1){
            name = (*it)["name"].get<std::string>();
            add_neuron(name, threshold, decay);
        }
        else{
            add_neuron(threshold, decay);
        }
    }


    //Wire up connections
    int index = 0;
    for( auto it = json_neurons.begin(); it != json_neurons.end(); it++ ){

        //Because neurons were added to net in order, 
        //the index of a json_neuron and net neuron is the same
        //therefore..
        in_n = neurons[index];


        connections =  (*it)["connections"].get<std::vector<json> >();
        for ( auto con_it=connections.begin(); con_it != connections.end(); con_it++ ){
            if( (*con_it)["to"].is_string() ){
                std::string to_str = (*con_it)["to"].get<std::string>();
                out_n = get_neuron(to_str);


            }
            else{
                to_int = (*con_it)["to"].get<int>();
                //std::cout << "    connects to: " << to_int << std::endl;
                out_n = neurons.at(to_int);
            }
            
            //GET json weight;
            if( (*con_it)['weight'].is_string() ){
                weight = rand()/double(RAND_MAX);
            }
            else{
                //Assume its a double
                weight = (*con_it)['weight'];
            }


            //GET json speed
            if( (*con_it)['speed'].is_string() ){
                speed = rand()%3 + 1;
            }
            else{
                //Assume its a double
                speed = (*con_it)['speed'];
            }



        }
        Neuron::connect(in_n, out_n, weight, speed)
        index++;
    }

    //Check that neurons got created
    for( int i=0; i<neurons.size(); i++ ){
        printf("Neuron #: %d", i);
        if( neurons_to_name.count(neurons[i]) > 0 ){
            printf(", name: %s\n", neurons_to_name.at(neurons[i]).c_str());
        }
        printf("    threshold: %f\n", neurons[i]->get_threshold());
        printf("    decay: %f\n", neurons[i]->get_decay());
        vector<Neuron*> outputs = neurons[i]->get_outputs;
        for ( int j=0; j<outputs.size(); j++ ){
            //////STOPPING HERE
            //Need to get key
            //printf("Neuron: %d", outputs[j]);

        }
        printf("\n\n");
    }

}

std::string Spiking_NNet::serialize(){
    return "To be implemented";
}
