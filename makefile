game-of-life : game_of_life.c
	gcc game_of_life.c -o game-of-life `sdl2-config --libs -cflags` --std=c18 -Wall
