snake: s.o b.o v.o
	cc s.o b.o v.o -Wall -o snake -lm -lSDL2 -lSDL2_ttf
s.o: snake.h snake.c
	cc snake.c -c -Wall -o s.o
v.o: snake.h vis.c
	cc vis.c -c -Wall -o v.o
b.o: snake.h backend.c
	cc backend.c -c -Wall -o b.o
clean:
	rm *.o *~ -f
