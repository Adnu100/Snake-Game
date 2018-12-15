snake: snake.o backend.o vis.o
	cc snake.o backend.o vis.o -Wall -o snake -lm -lSDL2
snake.o: snake.h snake.c
	cc snake.c -c -Wall
vis.o: snake.h vis.c 
	cc vis.c -c -Wall
backend.o: snake.h backend.c
	cc backend.c -c -Wall
