#include "spiking_nnet.h"
#include <vector>
#include <stdexcept>


void Neuron::add_input(Neuron* n, double weight, double speed){
    inputs.push_back(n);
    weights.push_back(weight);
    speeds.push_back(speed);
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

