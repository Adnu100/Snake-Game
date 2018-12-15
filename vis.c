#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "snake.h"

void MakeBoard(SDL_Renderer **ren) {
	SDL_SetRenderDrawColor(*ren, 255, 255, 255, 0);
	int i;
	for(i = 0; i < BLOCKSIZE; i++) {
		SDL_RenderDrawLine(*ren, i, 0, i, 800);
		SDL_RenderDrawLine(*ren, 800 - i, 0, 800 - i, 800);
		SDL_RenderDrawLine(*ren, 0, i, 800, i);
		SDL_RenderDrawLine(*ren, 0, 800 - i, 800, 800 - i);
	}		
}

void AD_DrawCircle(SDL_Renderer **ren, float center_x_coordinate, float center_y_coordinate, float radius, int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(*ren, r, g, b, a);
	int x;
	float y;
	for(x = 0; x <= radius; x++) {
		y = sqrt(radius * radius - (float)(x * x));
		SDL_RenderDrawLine(*ren, center_x_coordinate + x, center_y_coordinate + y, center_x_coordinate + x, center_y_coordinate - y);
		SDL_RenderDrawLine(*ren, center_x_coordinate - x, center_y_coordinate + y, center_x_coordinate - x, center_y_coordinate - y);
	}
}

void AD_DrawSnake(SDL_Renderer **ren, snake *s, struct XY co) {
	if(s->dir == ND)
		return;
	node *n;
	int i;
	float multiplier;
	int sel = Random(10, 20);
	multiplier = sel / 10;
	for(i = 0; i < BLOCKSIZE / STARTSPEED; i++) {
		SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0);
		SDL_RenderClear(*ren);
		n = s->tail;
		while(n != s->head) {
			AD_DrawCircle(ren, n->x, n->y, SNAKENODE, 255, 255, 255, 0);
			switch(n->dir) {
				case UP:
					n->y -= s->speed;
					break;
				case DOWN:
					n->y += s->speed;
					break;
				case RIGHT:
					n->x += s->speed;
					break;
				case LEFT:
					n->x -= s->speed;
					break;
				default:
					break;				
			}
			n = n->prev;
		}
		n = s->head;
		AD_DrawCircle(ren, n->x, n->y, BLOCKSIZE * 1.5, 255, 0, 0, 0);
		switch(s->head->dir) {
			case UP:
				s->head->y -= s->speed;
				break;
			case DOWN:
				s->head->y += s->speed;
				break;
			case RIGHT:
				s->head->x += s->speed;
				break;
			case LEFT:
				s->head->x -= s->speed;
				break;
			default:
				break;				
		}
		MakeBoard(ren);
		if(co.t == SMALL)
			AD_DrawCircle(ren, co.x, co.y, BLOCKSIZE, 255, 255, 0, 0);
		if(co.t == BIG) 
			AD_DrawCircle(ren, co.x, co.y, BLOCKSIZE * multiplier, Random(0, 255), Random(0, 255), Random(0, 255), 0);
		SDL_RenderPresent(*ren);
		SDL_Delay(1000 / 60);
	}
	for(n = s->tail; n != s->head; n = n->prev) 
		n->dir = n->prev->dir;	
	s->head->dir = s->dir;	
}
