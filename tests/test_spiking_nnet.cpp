#define CATCH_CONFIG_RUNNER
#include "../libs/catch.hpp"
#include <vector>
#include "spiking_nnet.h"
#include <iostream>


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

TEST_CASE("Neurons be connected"){
    int i;
    Neuron *input = new Neuron();
    Neuron *out1 = new Neuron();
    Neuron *out2 = new Neuron();

    Neuron::connect(input, out1, .2, 1);
    Neuron::connect(input, out2, .4, 2);

    REQUIRE(input->output_at(0) == out1);
    REQUIRE(input->output_at(1) == out2);
    REQUIRE(out1->weight_for(input) == .2);
    REQUIRE(out2->weight_for(input) == .4);
    REQUIRE(out1->speed_for(input) == 1);
    REQUIRE(out2->speed_for(input) == 2);
}

TEST_CASE("Input neurons can stimulate outpute neurons"){
    Neuron *input1 = new Neuron();
    Neuron *input2 = new Neuron();
    Neuron *input3 = new Neuron();

    Neuron *output = new Neuron();

    Neuron::connect(input1, output, .2, 1);
    Neuron::connect(input2, output, .3, 2);
    Neuron::connect(input3, output, .05, 4);

    input1->stimulate(output);
    input2->stimulate(output);
    input3->stimulate(output);

    //std::cout << "First step" << std::endl;
    output->step();
    REQUIRE(output->get_stimulation() == 0);

    //std::cout << "Second step" << std::endl;
    output->step();
    REQUIRE(output->get_stimulation() == .2);

    //std::cout << "Third step" << std::endl;
    output->step();
    REQUIRE(output->get_stimulation() == .5);

    //std::cout << "Fourth step" << std::endl;
    output->step();
    REQUIRE(output->get_stimulation() == .5);

    //std::cout << "Fith step" << std::endl;
    output->step();
    REQUIRE(output->get_stimulation() == .55);
}

TEST_CASE("Neurons will stimulate other neurons in their step"){
    Neuron *input = new Neuron(.6);
    Neuron *output = new Neuron();

    Neuron::connect(input, output, .2, 1);

    input->self_stimulate(.2); 
    //Self stimulation happens instantly
    //This is what will be used for inputing data into the system
    REQUIRE(input->get_stimulation() == .2);

    input->step();
    output->step();
    REQUIRE(output->get_stimulation() == 0);

    input->self_stimulate(.4);
    
    //This will cause input to stimulate output
    input->step();

    //But output will not respond on this step since time = 1
    output->step();
    
    REQUIRE(input->get_stimulation() == 0);
    REQUIRE(output->get_stimulation() == 0);

    input->step();
    output->step();

    REQUIRE(output->get_stimulation() == .2);

    Neuron *input2 = new Neuron(.4);

    Neuron::connect(input2, output, .5, 1);

    input2->self_stimulate(.4);
    input->self_stimulate(.7);

    for( int i=0; i<2; i++ ){
        input->step();
        input2->step();
        output->step();
    }

    REQUIRE(output->get_stimulation() == .9);


}

TEST_CASE("Neuron stim will decay"){
    Neuron *n = new Neuron(.6);

    n->set_decay(.5);

    n->self_stimulate(.4);
    n->step();

    REQUIRE(n->get_stimulation() == .2);
}

TEST_CASE("Build and step net"){
    Spiking_NNet *nnet = new Spiking_NNet();

    //Lets build a semi complex net
    Neuron *in1, *in2, *in3, *out1, *out2;
    //Test first version of add_neuron
    in1 = nnet->add_neuron("in1", .4, .5);
    in2 = nnet->add_neuron("in2", .2, .5);
    //Test second version
    in3 = nnet->add_neuron(.5, .2);
    //Test third version
    out1 = nnet->add_neuron("out1");
    //Test 4th version
    out2 = nnet->add_neuron();

    //Step all neurons
    nnet->step();
}

TEST_CASE("Serialize net"){
}

TEST_CASE("Load Serialized Net"){
}

TEST_CASE("Mutate Serialized Net"){
}





int main( int argc, char* argv[] )
{
      
    int result = Catch::Session().run( argc, argv );
     
          // global clean-up...
    
    return result;
}
