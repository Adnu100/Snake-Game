#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "snake.h"

TTF_Font *arial;
SDL_Rect R1 = {10, 2, 200, 70};
SDL_Color white = {255, 255, 255};

int main(int argc, char *argv[]) {	
	snake s, t;
	initsnake(&s);
	addnode(&s);	//first node (mouth of the snake)
	initsnake(&t);
	short int Running = 1;
	int opt, opt_index, stage = 1, tflag = 0;
	const struct option optarr[] = {
		{"help", no_argument, NULL, 'h'},
		{"reset", no_argument, NULL, 'r'}, 
		{"max-speed", no_argument, NULL, 'm'}, 
		{"controls", no_argument, NULL, 'c'},
		{"stage", required_argument, NULL, 's'},
		{NULL, 0, NULL, 0}
	};
	while((opt = getopt_long(argc, argv, "s:hHrmct", optarr, &opt_index)) != -1) {
		switch(opt) {
			case 's':
				stage = atoi(optarg);
				//stages will be done after 2 players
				break;
			case 'h':
				Display_help();
				return 0;
				break;
			case 'H':
				Display_highscore();
				return 0;
				break;	
			case 'r':
				Reset_highscore();
				return 0;
				break;
			case 'm':
				s.speed = STARTSPEED * 16;
				t.speed = STARTSPEED * 16;
				Running = 5;
				break;	
			case 'c':
				Display_controls();
				return 0;
				break;
			case 't':
				//yet to be done
				tflag = 1;
				addnode(&t);
				s.head->x = INITIALX_TWOPLAYER_SNAKE_s;
				s.head->y = INITIALY_TWOPLAYER_SNAKE_s;
				t.head->x = INITIALX_TWOPLAYER_SNAKE_t;
				t.head->y = INITIALY_TWOPLAYER_SNAKE_t;
				break;
			case '?':
				printf("Type %s --help | %s -h for help\n", argv[0], argv[0]);
				exit(7);
				break;			
		}		
	}
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
	arial = TTF_OpenFont("support/font.ttf", 50);
	if(arial == NULL) {
		fprintf(stderr, "Could not find the font loader file : %s\n", TTF_GetError());
		exit(5);
	}
	SDL_Event e;
	direction saved = ND, saved_t = ND;
	long int score = 0;
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
						t.dir = UP;
						s.dir = UP;
						co = RandomBall(&s, &t, tflag);
					}		
					switch(e.key.keysym.sym) {
						case SDLK_UP:
							if(saved != ND) {
								if(saved != DOWN) {
									s.dir = UP;
									saved = ND;
									t.dir = saved_t;
									saved_t = ND;
								}	
							}	
							else
								s.dir = (s.dir == DOWN ? DOWN : UP);
							break;
						case SDLK_DOWN:
							if(saved != ND) {
								if(saved != UP) {
									s.dir = DOWN;
									saved = ND;
									t.dir = saved_t;
									saved_t = ND;
								}	
							}
							else 
								s.dir = (s.dir == UP ? UP : DOWN);
							break;
						case SDLK_RIGHT:
							if(saved != ND) {
								if(saved != LEFT) {
									s.dir = RIGHT;
									saved = ND;
									t.dir = saved_t;
									saved_t = ND;
								}	
							}
							else 
								s.dir = (s.dir == LEFT ? LEFT : RIGHT);	
							break;
						case SDLK_LEFT:
							if(saved != ND) {
								if(saved != RIGHT) {
									s.dir = LEFT;
									saved = ND;
									t.dir = saved_t;
									saved_t = ND;
								}	
							}	
							else
								s.dir = (s.dir == RIGHT ? RIGHT : LEFT);
							break;
						case SDLK_w:
							if(saved_t != ND) {
								if(saved_t != DOWN) {
									t.dir = UP;
									saved_t = ND;
									s.dir = saved;
									saved = ND;
								}	
							}	
							else
								t.dir = (t.dir == DOWN ? DOWN : UP);
							break;
						case SDLK_s:
							if(saved_t != ND) {
								if(saved_t != UP) {
									t.dir = DOWN;
									saved_t = ND;
									s.dir = saved;
									saved = ND;
								}	
							}	
							else
								t.dir = (t.dir == UP ? UP : DOWN);
							break;
						case SDLK_d:
							if(saved_t != ND) {
								if(saved_t != LEFT) {
									t.dir = RIGHT;
									saved_t = ND;
									s.dir = saved;
									saved = ND;
								}	
							}	
							else
								t.dir = (t.dir == LEFT ? LEFT : RIGHT);
							break;
						case SDLK_a:
							if(saved_t != ND) {
								if(saved_t != RIGHT) {
									t.dir = LEFT;
									saved_t = ND;
									s.dir = saved;
									saved = ND;
								}	
							}	
							else
								t.dir = (t.dir == RIGHT ? RIGHT : LEFT);	
							break;	
						case SDLK_SPACE: case SDLK_KP_ENTER: case SDLK_p: case SDLK_RETURN:
							if(s.dir == ND) {
								t.dir = saved_t;
								s.dir = saved;
								saved = ND;
								saved_t = ND;
							}	
							else {
								saved = s.dir;
								saved_t = t.dir;
								t.dir = ND;
								s.dir = ND;
							}
							break;
						case SDLK_n:
							printf("\tGame %d score : %ld\n", num, score);
							UpdateHighscore(score, tflag);
							num++;
							Running = 1;
							score = 0;
							DestroySnake(&s);
							DestroySnake(&t);
							initsnake(&s);
							initsnake(&t);
							addnode(&s);
							if(tflag) {
								addnode(&t);
								s.head->x = INITIALX_TWOPLAYER_SNAKE_s;
								s.head->y = INITIALY_TWOPLAYER_SNAKE_s;
								t.head->x = INITIALX_TWOPLAYER_SNAKE_t;
								t.head->y = INITIALY_TWOPLAYER_SNAKE_t;
							}
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
		AD_DrawSnake(&ren, &s, &t, co, score, tflag);		//to update board conditions (frontend)	
		switch(CheckGame(&s, &t, co, tflag)) {
			case SNAKE_FOOD_SMALL: case SNAKE_FOOD_SMALL_s: 
				if(score >= LV4 && Running == 4) {
					s.speed *= 2;
					t.speed *= 2;
					Running = 5;
				}
				else if(score >= LV3 && Running == 3) {
					s.speed *= 2;
					t.speed *= 2;
					Running = 4;	
				}
				else if(score >= LV2 && Running == 2) {
					s.speed *= 2;
					t.speed *= 2;	
					Running = 3;
				}
				else if(score >= LV1 && Running == 1) {
					s.speed *= 2;
					t.speed *= 2;
					Running = 2;
				}	
				score += (s.speed * 5);
				addnode(&s);
				addnode(&s);
				addnode(&s);
				co = RandomBall(&s, &t, tflag);
				break;
			case SNAKE_FOOD_SMALL_t:
				if(score >= LV4 && Running == 4) {
					s.speed *= 2;
					t.speed *= 2;
					Running = 5;
				}
				else if(score >= LV3 && Running == 3) {
					s.speed *= 2;
					t.speed *= 2;
					Running = 4;	
				}
				else if(score >= LV2 && Running == 2) {
					s.speed *= 2;
					t.speed *= 2;	
					Running = 3;
				}
				else if(score >= LV1 && Running == 1) {
					s.speed *= 2;
					t.speed *= 2;
					Running = 2;
				}	
				score += (s.speed * 5);
				addnode(&t);
				addnode(&t);
				addnode(&t);
				co = RandomBall(&s, &t, tflag);
				break;	
			case SNAKE_FOOD_LARGE: case SNAKE_FOOD_LARGE_s:
				score += (s.speed * 25);
				addnode(&s);
				addnode(&s);
				addnode(&s);
				addnode(&s);
				addnode(&s);
				co = RandomBall(&s, &t, tflag);
				break;
			case SNAKE_FOOD_LARGE_t:
				score += (s.speed * 25);
				addnode(&t);
				addnode(&t);
				addnode(&t);
				addnode(&t);
				addnode(&t);
				co = RandomBall(&s, &t, tflag);
				break;	
			case SNAKE_COLLISION_WALL:
				printf("\tUpps!! The snake collided with the walls!!\n");
				s.dir = ND;
				t.dir = ND;
				Running = -1;
				break;
			case SNAKE_s_COLLISION_WALL:
				printf("\tUpps!! The red collided with the walls");	
				s.dir = ND;
				t.dir = ND;
				Running = -1;
				break;
			case SNAKE_t_COLLISION_WALL:
				printf("\tUpps!! The blue collided with the walls");	
				s.dir = ND;
				t.dir = ND;
				Running = -1;
				break;	
			case SNAKE_COLLISION_SNAKE:
				printf("\tUpps!! The snake collided with itself!!\n");
				s.dir = ND;
				t.dir = ND;
				Running = -1;
				break;
			case SNAKE_COLLISION_SNAKE_s:
				printf("\tUpps!! The red collided with itself!!\n");
				s.dir = ND;
				t.dir = ND;
				Running = -1;
				break;
			case SNAKE_COLLISION_SNAKE_t:
				printf("\tUpps!! The blue collided with itself!!\n");
				s.dir = ND;
				t.dir = ND;
				Running = -1;
				break;
			case SNAKE_s_COLLISION_SNAKE_t:
				printf("\tUpps!! The red collided with the blue!!\n");
				s.dir = ND;
				t.dir = ND;
				Running = -1;
				break;
			case SNAKE_t_COLLISION_SNAKE_s:
				printf("\tUpps!! The blue collided with the red!!\n");
				s.dir = ND;
				t.dir = ND;
				Running = -1;
				break;
			case HEAD_TO_HEAD_COLLISION:
				printf("\tUpps!! The snakes collided head to head!!\n");
				s.dir = ND;
				t.dir = ND;
				Running = -1;
				break;	
			case SNAKE_PROPAGATING:
				score += (s.speed * 0.5);	
				break;
			case SNAKE_HALTED:
				break;	
		}				
/*checks the collision of snake with walls and with snake or meals to snake and takes appropriate action
  accordingly*/	
  		AD_DrawSnake(&ren, &s, &t, co, score, tflag);		//callled twice for better animation
	}
	num == 1 ? printf("\tGame score : %ld\n", score * 10) : printf("\tGame %d score : %ld\n", num, score * 10);	
	if((score * 10) == UpdateHighscore(score * 10, tflag))
		printf("\tNew Highscore!\n");
	Display_highscore();
	TTF_CloseFont(arial);
	TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

