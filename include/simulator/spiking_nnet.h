#ifndef SPIKING_NNET_H
#define SPIKING_NNET_H

#include <vector>
#include <unordered_map>
#include <string>


struct Neuron{
    std::vector<Neuron*> inputs;
    std::vector<double> weights;
    std::vector<double> speeds;

    void add_input(Neuron *n, double weight, double speed);
};

class Spiking_NNet{
    public:
        Neuron* add_neuron(std::string);
        Neuron* add_neuron();
        Neuron* get_neuron(std::string);
        std::vector<Neuron*> get_neurons();
    private:
        std::vector<Neuron*> neurons;
        std::unordered_map<std::string, Neuron*> named_neurons;
};

#endif
