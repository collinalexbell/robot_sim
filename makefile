simulatormake: main.o sim.o
	g++ -o simulator main.o sim.o `sdl-config --cflags --libs` -lSDL_ttf

main.o: main.cpp
	g++ -c main.cpp

sim.o: sim.cpp sim.h
	g++ -c sim.cpp 

test_sim: sim.o test.o
	g++ -o tests/test_sim sim.o test_sim.o `sdl-config --cflags --libs` -lSDL_ttf -lSDLmain -v
	tests/test_sim 

test_robot: robot.o test_robot.o
	g++ -o tests/test_robot robot.o test_robot.o `sdl-config --cflags --libs` -lSDL_ttf -lSDLmain -v
	tests/test_robot 

test.o:
	g++ -c tests/test_sim.cpp 

test_robot.o:
	g++ -c tests/test_robot.cpp

robot.o:
	g++ -c robot.cpp

clean:
	rm *.o


	
	
