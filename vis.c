#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "snake.h"

void MakeBoard(SDL_Renderer **ren) {
	int i;
	int b = BLOCKSIZE * 1.5;
	SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0);		
	for(i = BLOCKSIZE / 2; i < b; i++) {
		SDL_RenderDrawLine(*ren, i, 0, i, 800);
		SDL_RenderDrawLine(*ren, 800 - i, 0, 800 - i, 800);
		SDL_RenderDrawLine(*ren, 0, i, 800, i);
		SDL_RenderDrawLine(*ren, 0, 800 - i, 800, 800 - i);
	}
	SDL_SetRenderDrawColor(*ren, 255, 255, 255, 0);
	for(i = 0; i < BLOCKSIZE / 2; i++) {
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

void DrawSnakeBody(SDL_Renderer **ren, int center_x, int center_y, int side) {
	int x = center_x + side;
	int y1 = center_y - side;
	int y2 = center_y + side;
	for(int i = center_x - side; i < x; i++)
		SDL_RenderDrawLine(*ren, i, y1, i, y2);
}

extern TTF_Font *arial;
extern SDL_Rect R1;
extern SDL_Color white;

void AD_DrawSnake(SDL_Renderer **ren, snake *s, snake *t, struct XY co, long int score, int tflag) {
	if(s->dir == ND)
		return;
	if(t->dir == ND)
		return;	
	SDL_Surface *sur;
	SDL_Texture *tex_newgame;
	char scoretext[64];
	sprintf(scoretext, "score : %16ld", score * 10);
	sur = TTF_RenderText_Solid(arial, scoretext, white);
	tex_newgame = SDL_CreateTextureFromSurface(*ren, sur);
	SDL_FreeSurface(sur);
	node *n;
	int i;
	float multiplier;
	int sel = Random(10, 20);
	multiplier = sel / 10;
	for(i = 0; i < BLOCKSIZE / s->speed; i++) {
		SDL_SetRenderDrawColor(*ren, 0, 100, 0, 200);
		SDL_RenderClear(*ren);
		SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0);
		SDL_RenderCopy(*ren, tex_newgame, NULL, &R1);
		n = s->tail;
		while(n != s->head) {
			DrawSnakeBody(ren, n->x, n->y, SNAKENODE);
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
		if(tflag) {
			SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0);
			n = t->tail;
			while(n != t->head) {
				DrawSnakeBody(ren, n->x, n->y, SNAKENODE);
				switch(n->dir) {
					case UP:
						n->y -= t->speed;
						break;
					case DOWN:
						n->y += t->speed;
						break;
					case RIGHT:
						n->x += t->speed;
						break;
					case LEFT:
						n->x -= t->speed;
						break;
					default:
						break;				
				}
				n = n->prev;
			}
			AD_DrawCircle(ren, n->x, n->y, BLOCKSIZE * 1.5, 255, 0, 0, 0);
			switch(t->head->dir) {
				case UP:
					t->head->y -= t->speed;
					break;
				case DOWN:
					t->head->y += t->speed;
					break;
				case RIGHT:
					t->head->x += t->speed;
					break;
				case LEFT:
					t->head->x -= t->speed;
					break;
				default:
					break;				
			}
		}
		MakeBoard(ren);
		if(co.t == SMALL)
			AD_DrawCircle(ren, co.x, co.y, BLOCKSIZE, 255, 255, 0, 0);
		if(co.t == BIG) 
			AD_DrawCircle(ren, co.x, co.y, BLOCKSIZE * multiplier, Random(0, 255), Random(0, 255), Random(0, 255), 0);
		SDL_RenderPresent(*ren);
		SDL_Delay(1000 / 60);
	}
	SDL_SetRenderDrawColor(*ren, 0, 100, 0, 200);
	SDL_RenderClear(*ren);
	SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0);
	SDL_RenderCopy(*ren, tex_newgame, NULL, &R1);
	n = s->tail;
	while(n != s->head) {
		DrawSnakeBody(ren, n->x, n->y, SNAKENODE);
		switch(n->dir) {
			case UP:
				n->y -= BLOCKSIZE % s->speed;
				break;
			case DOWN:
				n->y += BLOCKSIZE % s->speed;
				break;
			case RIGHT:
				n->x += BLOCKSIZE % s->speed;
				break;
			case LEFT:
				n->x -= BLOCKSIZE % s->speed;
				break;
			default:
				break;				
		}
		n = n->prev;
	}
	AD_DrawCircle(ren, n->x, n->y, BLOCKSIZE * 1.5, 255, 0, 0, 0);
	switch(s->head->dir) {
		case UP:
			s->head->y -= BLOCKSIZE % s->speed;
			break;
		case DOWN:
			s->head->y += BLOCKSIZE % s->speed;
			break;
		case RIGHT:
			s->head->x += BLOCKSIZE % s->speed;
			break;
		case LEFT:
			s->head->x -= BLOCKSIZE % s->speed;
			break;
		default:
			break;				
	}
	if(tflag) {
		n = t->tail;
		SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0);
		while(n != t->head) {
			DrawSnakeBody(ren, n->x, n->y, SNAKENODE);
			switch(n->dir) {
				case UP:
					n->y -= BLOCKSIZE % t->speed;
					break;
				case DOWN:
					n->y += BLOCKSIZE % t->speed;
					break;
				case RIGHT:
					n->x += BLOCKSIZE % t->speed;
					break;
				case LEFT:
					n->x -= BLOCKSIZE % t->speed;
					break;
				default:
					break;				
			}
			n = n->prev;
		}
		AD_DrawCircle(ren, n->x, n->y, BLOCKSIZE * 1.5, 255, 0, 0, 0);
		switch(t->head->dir) {
			case UP:
				t->head->y -= BLOCKSIZE % t->speed;
				break;
			case DOWN:
				t->head->y += BLOCKSIZE % t->speed;
				break;
			case RIGHT:
				t->head->x += BLOCKSIZE % t->speed;
				break;
			case LEFT:
				t->head->x -= BLOCKSIZE % t->speed;
				break;
			default:
				break;				
		}
	}
	MakeBoard(ren);
	if(co.t == SMALL)
		AD_DrawCircle(ren, co.x, co.y, BLOCKSIZE, 255, 255, 0, 0);
	if(co.t == BIG) 
		AD_DrawCircle(ren, co.x, co.y, BLOCKSIZE * multiplier, Random(0, 255), Random(0, 255), Random(0, 255), 0);
	SDL_RenderPresent(*ren);
	SDL_Delay(1000 / 60);
	for(n = s->tail; n != s->head; n = n->prev) 
		n->dir = n->prev->dir;	
	s->head->dir = s->dir;
	if(tflag) {
		for(n = t->tail; n != t->head; n = n->prev)
			n->dir = n->prev->dir;
		t->head->dir = t->dir;	
	}	
	SDL_DestroyTexture(tex_newgame);
}


