#include "spiking_nnet.h"
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <iostream>


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
    insert_me = std::make_pair(name, rv);
    named_neurons.insert(insert_me);
    return rv;
}

Neuron* Spiking_NNet::add_neuron(std::string name, double thresh, double decay){
    Neuron *rv = add_neuron(thresh, decay);
    std::pair<std::string, Neuron*> insert_me;
    insert_me = std::make_pair(name, rv);
    named_neurons.insert(insert_me);
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

