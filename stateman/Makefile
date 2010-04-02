
stateman:	gameengine.o introstate.o main.o menustate.o playstate.o
	g++ gameengine.o introstate.o main.o menustate.o playstate.o `sdl-config --libs` -o stateman

main.o:		main.cpp
	g++ -c main.cpp `sdl-config --cflags`

gameengine.o:	gameengine.cpp gameengine.h
	g++ -c gameengine.cpp `sdl-config --cflags`

introstate.o:	introstate.cpp introstate.h
	g++ -c introstate.cpp `sdl-config --cflags`

menustate.o:	menustate.cpp menustate.h
	g++ -c menustate.cpp `sdl-config --cflags`

playstate.o:	playstate.cpp playstate.h
	g++ -c playstate.cpp `sdl-config --cflags`

clean:
	rm -f *.o stateman
