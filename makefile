LIB := include/simulator
SIM_LIB :=  include/simulator
CLANG := clang++ -std=c++11 -stdlib=libc++ -c -g -I $(LIB)
LINK := clang++ `sdl-config --cflags --libs` -lSDL_ttf -lSDL_image -lSDL_gfx -std=c++11 -stdlib=libc++ -v -g -I $(LIB)
SRC := src
BIN := bin
TEST := tests

simulatormake: main.o sim.o
	clang++ -o simulator main.o sim.o `sdl-config --cflags --libs` -lSDL_ttf -std=c++11 -stdlib=libc++ 

main.o: $(SRC)/main.cpp
	$(CLANG) $(SRC)/main.cpp

sim.o: $(SRC)/sim.cpp $(SIM_LIB)/sim.h $(SIM_LIB)/drawable.h
	$(CLANG) $(SRC)/sim.cpp 

robot.o: $(SRC)/robot.cpp $(SIM_LIB)/robot.h $(SIM_LIB)/point.h $(SIM_LIB)/drawable.h
	$(CLANG) $(SRC)/robot.cpp

world.o: $(SRC)/world.cpp $(SIM_LIB)/world.h $(SIM_LIB)/robot.h $(SIM_LIB)/point.h $(SIM_LIB)/drawable.h
	$(CLANG) $(SRC)/world.cpp

spiking_nnet.o: $(SRC)/spiking_nnet.cpp $(SIM_LIB)/spiking_nnet.h
	$(CLANG) $(SRC)/spiking_nnet.cpp

drawable.h: point.h

robot.h: point.h drawable.h

point.h: 

test_sim: sim.o test_sim.o
	$(LINK) -o $(BIN)/test_sim sim.o test_sim.o

test_robot: robot.o sim.o
	$(LINK) -o $(BIN)/test_robot robot.o $(TEST)/test_robot.cpp sim.o

test_world: robot.o world.o sim.o
	$(LINK) -o $(BIN)/test_world world.o robot.o sim.o $(TEST)/test_world.cpp

test_spiking_nnet: spiking_nnet.o sim.o
	$(LINK) -o $(BIN)/test_spiking_nnet $(TEST)/test_spiking_nnet.cpp spiking_nnet.o sim.o 	
	gdb bin/test_spiking_nnet

clean:
	rm *.o


	

