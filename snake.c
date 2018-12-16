#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "snake.h"

int main() {
	snake s;
	initsnake(&s);
	s.dir = ND;
	addnode(&s);	//first node (mouth of the snake)
	if(SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Could not Initialize SDL : %s\n", SDL_GetError());
		exit(1);
	}
	SDL_Window *window;
	SDL_Renderer *ren;
	if(!(window = SDL_CreateWindow("Adnesh's Snake Game", 100, 100, 800, 950, SDL_WINDOW_SHOWN))) {
		fprintf(stderr, "Could not Create Window : %s\n", SDL_GetError());
		exit(2);
	}
	if(!(ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))) {
		SDL_DestroyWindow(window);
		fprintf(stderr, "could not Create Renderer : %s\n", SDL_GetError());
		exit(3);
	}
	SDL_Event e;
	direction saved = ND;
	long int score = 0;
	short int Running = 1;
	struct XY co;
	while(Running) {
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT:
					Running = 0;
					break;	
				case SDL_KEYDOWN:
    					if(Running == -1) {
						Running = 0;
						break;
					}	
					if(s.dir == ND && saved == ND) {
						score = 0;
						co = RandomBall(&s);
					}		
					switch(e.key.keysym.sym) {
						case SDLK_UP:
							if(s.dir != DOWN)
								s.dir = UP;
							break;
						case SDLK_DOWN:
							if(s.dir != UP)
								s.dir = DOWN;
							break;
						case SDLK_RIGHT:
							if(s.dir != LEFT)
								s.dir = RIGHT;
							break;
						case SDLK_LEFT:
							if(s.dir != RIGHT)
								s.dir = LEFT;
							break;
						case SDLK_SPACE: case SDLK_KP_ENTER: case SDLK_p: case SDLK_RETURN:
							if(s.dir == ND) {
								s.dir = saved;
								saved = ND;
							}	
							else {
								saved = s.dir;
								s.dir = ND;
							}
						default:
							break;		
					}
				default:
					break;		
			}
			if(e.type == SDL_KEYDOWN)
				break;
		} 	
		AD_DrawSnake(&ren, &s, co);		//to update borad conditions (frontend)			
		AD_DrawSnake(&ren, &s, co);		//callled twice for better animation
		switch(CheckGame(&s, co)) {
			case SNAKE_FOOD_SMALL:
				score += s.speed * 5;
				for(Running = 0; Running < 10; Running++)
				addnode(&s);
				co = RandomBall(&s);
				break;
			case SNAKE_FOOD_LARGE:
				score += s.speed * 25;
				addnode(&s);
				addnode(&s);
				co = RandomBall(&s);
				break;
			case SNAKE_COLLISION_WALL:
				printf("Upps!! The snake collided with the walls!!\n");
				s.dir = ND;
				Running = -1;
				break;
			case SNAKE_COLLISION_SNAKE:
				printf("Upps!! The snake collided with itself!!\n");
				s.dir = ND;
				Running = -1;
				break;		 
			case SNAKE_PROPAGATING:
				score += s.speed / 2;	
				break;
			case SNAKE_HALTED:
				break;	
		}				/*checks the collision of snake with walls	*
						 *and with snake or meals to			*
						 *snake and takes appropriate action accordingly*/		 
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(window);
	return 0;
}

