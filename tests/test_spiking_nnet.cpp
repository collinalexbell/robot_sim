#define CATCH_CONFIG_RUNNER
#include "../libs/catch.hpp"
#include <vector>
#include "spiking_nnet.h"


TEST_CASE("Spiking nnet can add nuerons"){
    int i, j;
    Spiking_NNet* nnet = new Spiking_NNet();
    std::vector<Neuron*> returned_pointers;
    std::vector<Neuron*> get_neuron_vector;

    //2 ways to have hold of neurons: 
    //  1) use returned pointer
    //  2) use nnet->get_neurons 

    //Method 1: Make and store some neurons
    for( i=0; i<10; i++ ){
        Neuron* tmp_neuron = nnet->add_neuron();
        returned_pointers.push_back(tmp_neuron);
    }

    //Method 2: use get_neurons()
    get_neuron_vector = nnet->get_neurons();

    for( i=0; i<returned_pointers.size(); i++ ){

        //Test all neurons are instances of neurons
        REQUIRE(typeid(*returned_pointers[i]) == typeid(Neuron));
        //Test all both methods return the same thing
        REQUIRE(returned_pointers[i] == get_neuron_vector[i]);

        //Test all neurons are unique
        for( j=0; j<returned_pointers.size(); j++ ){
            if( i != j ){
                REQUIRE(returned_pointers[i] != returned_pointers[j]);
            }
        }
    }

}

TEST_CASE("Spiking nnet can associate names to neurons"){
    Spiking_NNet* nnet ;
    Neuron *n1, *n2, *n3;
        
    nnet = new Spiking_NNet();

    //Make a couple neurons with a name
    n1 = nnet->add_neuron("input");
    n2 = nnet->add_neuron("output");
    n3 = nnet->add_neuron("random_name");

    REQUIRE(nnet->get_neuron("input") == n1);
    REQUIRE(nnet->get_neuron("output") == n2);
    REQUIRE(nnet->get_neuron("random_name") == n3);
    
    //Assert that if neuron does not exist, an exception is thrown
    REQUIRE_THROWS(nnet->get_neuron("does_not_exist"));

    delete nnet;

}

TEST_CASE("Neurons can add an output"){
    int i;
    Neuron *input = new Neuron();
    Neuron *out1 = new Neuron();
    Neuron *out2 = new Neuron();

    Neuron::connect(input, out1, .2, .3);
    Neuron::connect(input, out2, .4, .5);

    REQUIRE(input->output_at(0) == out1);
    REQUIRE(input->output_at(1) == out2);
    REQUIRE(out1->weight_for(input) == .2);
    REQUIRE(out1->speed_for(input) == .3);
    REQUIRE(out2->weight_for(input) == .4);
    REQUIRE(out2->speed_for(input) == .5);
}


int main( int argc, char* argv[] )
{
      
    int result = Catch::Session().run( argc, argv );
     
          // global clean-up...
    
    return result;
}
