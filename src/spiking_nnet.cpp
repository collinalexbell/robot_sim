#include "spiking_nnet.h"
#include <vector>
#include <stdexcept>
#include <unordered_map>


void Neuron::connect(Neuron *from, Neuron *to, double weight, double speed){
    //Notify from of to
    from->outputs.push_back(to);

    //Set the weight and speed in the to
    //Weight
    std::pair<Neuron*, double> weight_pair;
    weight_pair = std::make_pair(from, weight);
    to->weights.insert(weight_pair);

    //Speed
    std::pair<Neuron*, double> speed_pair;
    speed_pair = std::make_pair(from, speed);
    to->speeds.insert(speed_pair);
}

double Neuron::weight_for(Neuron *input){
    return weights.at(input);
}

double Neuron::speed_for(Neuron *input){
    return speeds.at(input);
}

Neuron* Neuron::output_at(int position){
    return outputs.at(position);
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

