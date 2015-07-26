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

robot.o: $(SRC)/robot.cpp $(SIM_LIB)/robot.h $(SIM_LIB)/point.h $(SIM_LIB)/drawable.h $(SIM_LIB)/modulo.h
	$(CLANG) $(SRC)/robot.cpp

garden.o: $(SRC)/garden.cpp $(SIM_LIB)/garden.h $(SIM_LIB)/point.h $(SIM_LIB)/drawable.h
	$(CLANG) $(SRC)/garden.cpp

world.o: $(SRC)/world.cpp $(SIM_LIB)/world.h $(SIM_LIB)/robot.h $(SIM_LIB)/garden.h $(SIM_LIB)/point.h $(SIM_LIB)/drawable.h
	$(CLANG) $(SRC)/world.cpp

spiking_nnet.o: $(SRC)/spiking_nnet.cpp $(SIM_LIB)/spiking_nnet.h
	$(CLANG) $(SRC)/spiking_nnet.cpp

distance_sensor.o: $(SRC)/distance_sensor.cpp $(SIM_LIB)/distance_sensor.h $(SIM_LIB)/robot.h $(SIM_LIB)/modulo.h $(SIM_LIB)/drawable.h
	$(CLANG) $(SRC)/distance_sensor.cpp


drawable.h: point.h

robot.h: point.h drawable.h

point.h: 

test_sim: sim.o test_sim.o
	$(LINK) -o $(BIN)/test_sim sim.o test_sim.o

test_robot: robot.o sim.o
	$(LINK) -o $(BIN)/test_robot robot.o $(TEST)/test_robot.cpp sim.o

test_garden: garden.o
	$(LINK) -o $(BIN)/test_garden garden.o $(TEST)/test_garden.cpp
	gdb bin/test_garden

test_world: robot.o world.o sim.o garden.o
	$(LINK) -o $(BIN)/test_world world.o robot.o sim.o garden.o $(TEST)/test_world.cpp
	gdb bin/test_world

test_spiking_nnet: spiking_nnet.o sim.o
	$(LINK) -o $(BIN)/test_spiking_nnet $(TEST)/test_spiking_nnet.cpp spiking_nnet.o sim.o 	
	gdb bin/test_spiking_nnet

test_distance_sensor: distance_sensor.o robot.o garden.o
	$(LINK) -o $(BIN)/test_distance_sensor $(TEST)/test_distance_sensor.cpp distance_sensor.o  robot.o garden.o
	gdb bin/test_distance_sensor


clean:
	rm *.o


	

