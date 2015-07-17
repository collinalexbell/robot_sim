simulatormake: main.o sim.o
	clang++ -o simulator main.o sim.o `sdl-config --cflags --libs` -lSDL_ttf -std=c++11 -stdlib=libc++ 

main.o: main.cpp
	clang++ -c main.cpp -g

sim.o: sim.cpp sim.h
	clang++ -c sim.cpp -g -std=c++11 -stdlib=libc++

test_sim: sim.o test_sim.o
	clang++ -o tests/test_sim sim.o test_sim.o `sdl-config --cflags --libs` -lSDL_ttf -lSDL_image -lSDL_gfx -std=c++11 -stdlib=libc++ -v -g
	gdb tests/test_sim 

test_robot: robot.o test_robot.o sim.o
	echo "compiling test_tobot now"
	clang++ -o tests/test_robot robot.o test_robot.o sim.o `sdl-config --cflags --libs` -lSDL_ttf  -lSDL_image -lSDL_gfx -std=c++11 -stdlib=libc++ -v
	gdb tests/test_robot 

test_world: robot.o world.o test_world.o sim.o
	clang++ -o tests/test_world world.o robot.o sim.o test_world.o `sdl-config --cflags --libs` -lSDL_ttf -lSDL_image -lSDL_gfx -std=c++11 -stdlib=libc++ -v -g
	gdb tests/test_world 

test_sim.o:
	clang++ -c tests/test_sim.cpp -stdlib=libc++ -std=c++11

test_robot.o:
	clang++ -c tests/test_robot.cpp -g -stdlib=libc++ -std=c++11 

test_world.o:
	clang++ -c tests/test_world.cpp -stdlib=libc++ -std=c++11

robot.o:
	clang++ -c robot.cpp -g

world.o:
	clang++ -c world.cpp

clean:
	rm *.o


	
	
