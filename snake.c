#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "snake.h"

TTF_Font *arial;
SDL_Rect R1 = {10, 2, 200, 70};
SDL_Color white = {255, 255, 255};

int main() {
	int opt, opt_index, stage = 1;
	const struct option optarr[] = {
		{"help", no_argument, NULL, 'h'},
		{"reset", no_argument, NULL, 'r'}, 
		{"max-speed", no_argument, NULL, 'm'}, 
		{"controls", no_arguments, NULL, 'c'},
		{"stage", required_argument, NULL, 's'},
		{NULL, 0, NULL, 0}
	};
	while((opt = getopt_long(argc, argv, "s:hHrmct", optarr, &opt_index)) != -1) {
		switch(opt) {
			case 's':
				stage = atoi(optarg);
				break;
			case 'h':
				Display_help();
				return 0;
				break;
			case 'H':
				//show the highscore
				return 0;
				break;	
			case 'r':
				//reset the highscore;
				return 0;
				break;
			case 'm':
				s.speed = 100;
				Running = 100;
				break;	
			case 'c':
				Display_controls();
				return 0;
				break;
			case 's':
				tflag = 1;
				initsnake(&t);
				addnode(&t);
				break;
			case '?':
				printf("Type %s --help | %s -h for help\n", argv[0], argv[0]);
				exit(7);
				break;			
		}		
	}	
	snake s;
	initsnake(&s);
	addnode(&s);	//first node (mouth of the snake)
	if(SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Could not Initialize SDL : %s\n", SDL_GetError());
		exit(1);
	}
	SDL_Window *window;
	SDL_Renderer *ren;
	if(!(window = SDL_CreateWindow("Adnesh's Snake Game", 100, 100, 800, 800, SDL_WINDOW_SHOWN))) {
		fprintf(stderr, "Could not Create Window : %s\n", SDL_GetError());
		exit(2);
	}
	if(!(ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))) {
		SDL_DestroyWindow(window);
		fprintf(stderr, "could not Create Renderer : %s\n", SDL_GetError());
		exit(3);
	}
	if(TTF_Init()) {
			fprintf(stderr, "Could not Initiate Fonts : %s\n", TTF_GetError());
			exit(4);
	}
	arial = TTF_OpenFont("font.ttf", 50);
	if(arial == NULL) {
		fprintf(stderr, "Could not find the font loader file : %s", TTF_GetError());
		exit(5);
	}
	SDL_Event e;
	direction saved = ND;
	long int score = 0;
	short int Running = 1;
	int num = 1;
	struct XY co;
	while(Running) {
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT:
					Running = 0;
					break;	
				case SDL_KEYDOWN:
    					if(Running == -1 && e.key.keysym.sym != SDLK_n) {
						Running = 0;
						break;
					}	
					if(s.dir == ND && saved == ND) {
						score = 0;
						co = RandomBall(&s);
					}		
					switch(e.key.keysym.sym) {
						case SDLK_UP:
							if(s.dir != DOWN && saved != DOWN) {
								s.dir = UP;
								saved = ND;
							}	
							break;
						case SDLK_DOWN:
							if(s.dir != UP && saved != UP) {
								s.dir = DOWN;
								saved = ND;
							}	
							break;
						case SDLK_RIGHT:
							if(s.dir != LEFT && saved != LEFT) {
								s.dir = RIGHT;
								saved = ND;
							}	
							break;
						case SDLK_LEFT:
							if(s.dir != RIGHT && saved != RIGHT) {
								s.dir = LEFT;
								saved = ND;
							}	
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
							break;
						case SDLK_n:
							printf("\tGame %d score : %ld\n", num, score);
							num++;
							Running = 1;
							score = 0;
							DestroySnake(&s);
							initsnake(&s);
							addnode(&s);
							break;	
						default:
							break;		
					}
				default:
					break;		
			}
			if(e.type == SDL_KEYDOWN)
				break;
		} 	
		AD_DrawSnake(&ren, &s, co, score);		//to update borad conditions (frontend)			
		SnakeCollisionTest(&s);
		AD_DrawSnake(&ren, &s, co, score);		//callled twice for better animation
		switch(CheckGame(&s, co)) {
			case SNAKE_FOOD_SMALL:
				if(score >= LV4 && Running == 4) {
					s.speed *= 2;
					Running = 5;
				}
				else if(score >= LV3 && Running == 3) {
					s.speed *= 2;
					Running = 4;	
				}
				else if(score >= LV2 && Running == 2) {
					s.speed *= 2;	
					Running = 3;
				}
				else if(score >= LV1 && Running == 1) {
					s.speed *= 2;
					Running = 2;
				}	
				score += (s.speed * 5);
				addnode(&s);
				addnode(&s);
				addnode(&s);
				co = RandomBall(&s);
				break;
			case SNAKE_FOOD_LARGE:
				score += (s.speed * 25);
				addnode(&s);
				addnode(&s);
				addnode(&s);
				addnode(&s);
				addnode(&s);
				co = RandomBall(&s);
				break;
			case SNAKE_COLLISION_WALL:
				printf("\tUpps!! The snake collided with the walls!!\n");
				s.dir = ND;
				Running = -1;
				break;
			case SNAKE_COLLISION_SNAKE:
				printf("\tUpps!! The snake collided with itself!!\n");
				s.dir = ND;
				Running = -1;
				break;
			case SNAKE_PROPAGATING:
				score += (s.speed * 0.5);	
				break;
			case SNAKE_HALTED:
				break;	
		}				/*checks the collision of snake with walls	*
						 *and with snake or meals to			*
						 *snake and takes appropriate action accordingly*/		 
	}
	num == 1 ? printf("\tGame score : %ld\n", score * 10) : printf("\tGame %d score : %ld\n", num, score * 10);	
	TTF_CloseFont(arial);
	TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

