#ifndef SPIKING_NNET_H
#define SPIKING_NNET_H

#include <vector>
#include <unordered_map>
#include <string>


class Neuron{
    //Static methods
    public:
        static void connect(Neuron*, Neuron*, double, double);
    
    //For neurons that are recieving stimuli
    private:
        std::unordered_map<Neuron*, double> weights;
        std::unordered_map<Neuron*, double> speeds;
    public:
        double weight_for(Neuron*);
        double speed_for(Neuron*);
   
    //For neurons that are sending stimuli
    private:
        std::vector<Neuron*> outputs;

    public:
        Neuron* output_at(int);


    //Methods for all types
    private:
        double stimulation;

    public:
        Neuron(){
            stimulation = 0;
        }
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
