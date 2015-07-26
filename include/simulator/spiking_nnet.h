#ifndef SPIKING_NNET_H
#define SPIKING_NNET_H

#include <vector>
#include <unordered_map>
#include <string>
#include <deque>


class Neuron{
    //Static methods
    public:
        static void connect(Neuron*, Neuron*, double, int);
    
    //For neurons that are recieving stimuli
    private:
        std::unordered_map<Neuron*, double> weights;
        std::unordered_map<Neuron*, int> speeds;
    public:
        double weight_for(Neuron*);
        int speed_for(Neuron*);
        void recieve_stimulation(Neuron*, double);
        void set_speed_for(Neuron*, double);
        void set_weight_for(Neuron*, double);
   
    //For neurons that are sending stimuli
    private:
        std::vector<Neuron*> outputs;

    public:
        Neuron* output_at(int);
        void stimulate(Neuron*);
        void stimulate_step();
        void self_stimulate(double stim){
            stimulation += stim;
        }
        std::vector<Neuron*> get_outputs(){
            return outputs;
        }


    //Methods for all types
    private:
        void handle_stimulation_step();
        std::deque< std::vector<double>* > incoming_stimuli;
        double stimulation;
        double output_strength;
        double threshold;
        double stim_decay;
        double output;

    public:
        double get_stimulation(){return stimulation;}
        Neuron(){
            stimulation = 0;
            threshold = 1;
            output_strength = 1;
            stim_decay = 0;
        }
        Neuron(double threshold):threshold(threshold){
            stimulation = 0;
            output_strength = 1;
        }
        Neuron(double thresh, double decay){
            threshold = thresh;
            stim_decay = decay;
            stimulation = 0;
            output_strength = 1;
        }
        void set_decay(double decay){
            stim_decay = decay;
        }
        void set_threshold(double t){
            threshold = t;
        }
        double get_decay(){
            return stim_decay;
        }

        double get_threshold(){
            return threshold;
        }

        double get_output(){
            return output;
        }

        void step();
};

class Spiking_NNet{
    public:
        Spiking_NNet();

        //This loads a json file to create a net
        Spiking_NNet(std::string);

        void mutate(double rate, double max_amount);

        Neuron* add_neuron(std::string, double thresh, double decay);
        Neuron* add_neuron(double thresh, double decay);
        Neuron* add_neuron(std::string);
        Neuron* add_neuron();
        Neuron* get_neuron(std::string);
        std::vector<Neuron*> get_neurons();
        std::string serialize();
        void step();
    private:
        std::vector<Neuron*> neurons;
        std::unordered_map<std::string, Neuron*> named_neurons;
        std::unordered_map<Neuron*, std::string> neurons_to_name;
};

#endif
