#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define STARTSPEED 8
#define BLOCKSIZE 8
#define INITIALX 400
#define INITIALY 400

typedef enum direction {UP, DOWN, LEFT, RIGHT, ND} direction;

typedef struct node {
	int x, y;
	direction dir;
	struct node *next, *prev;
} node;

typedef struct snake {
	int speed, length;
	direction dir;
	node *head, *tail;
} snake;

void initsnake(snake *s);
void addnode(snake *s);
void UpdateBoardStatus(snake *s);
void AD_DrawSnake(SDL_Renderer **ren, snake *s);
void AD_DrawCircle(SDL_Renderer **ren, float center_x_coordinate, float center_y_coordinate, float radius, int r, int g, int b, int a);

void initsnake(snake *s) {
	s->head = s->tail = NULL;
	s->speed = STARTSPEED;
	s->length = 0;
	s->dir = ND;
}

void addnode(snake *s) {
	s->length++;
	if(s->head == NULL) {
		s->head = (node *)malloc(sizeof(node));
		s->tail = s->head;
		s->head->dir = s->dir;
		s->head->x = INITIALX;
		s->head->y = INITIALY;
		s->head->next = NULL;					/*******************************/
		s->head->prev = NULL;					/*     ADNESH'S SNAKE GAME     */
		return;							/*******************************/
	}
	node *n = (node *)malloc(sizeof(node));
	n->next = NULL;
	n->prev = NULL;
	node *tmp = s->head;
	while(tmp->next) 
		tmp = tmp->next;
	tmp->next = n;
	tmp->next->dir = tmp->dir;
	switch(tmp->dir) {
		case UP:
			tmp->next->x = tmp->x;
			tmp->next->y = tmp->y + BLOCKSIZE;
			break;
		case DOWN:
			tmp->next->x = tmp->x;
			tmp->next->y = tmp->y - BLOCKSIZE;
			break;
		case RIGHT:
			tmp->next->x = tmp->x - BLOCKSIZE;
			tmp->next->y = tmp->y;
			break;
		case LEFT:
			tmp->next->x = tmp->x + BLOCKSIZE;
			tmp->next->y = tmp->y;
			break;
		default:
			break;				
	}
	tmp->next->prev = tmp;
	s->tail = n;
}

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

void AD_DrawSnake(SDL_Renderer **ren, snake *s) {
	node *n;
	int i;
	for(i = 0; i < BLOCKSIZE / STARTSPEED; i++) {
		SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0);
		SDL_RenderClear(*ren);
		n = s->tail;
		while(n != s->head) {
			AD_DrawCircle(ren, n->x, n->y, BLOCKSIZE, 255, 255, 255, 0);
			switch(n->dir) {
				case UP:
					n->y -= STARTSPEED;
					break;
				case DOWN:
					n->y += STARTSPEED;
					break;
				case RIGHT:
					n->x += STARTSPEED;
					break;
				case LEFT:
					n->x -= STARTSPEED;
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
				s->head->y -= STARTSPEED;
				break;
			case DOWN:
				s->head->y += STARTSPEED;
				break;
			case RIGHT:
				s->head->x += STARTSPEED;
				break;
			case LEFT:
				s->head->x -= STARTSPEED;
				break;
			default:
				break;				
		}
		MakeBoard(ren);
		SDL_RenderPresent(*ren);
		SDL_Delay(1000 / 60);
	}
	for(n = s->tail; n != s->head; n = n->prev) 
		n->dir = n->prev->dir;	
	s->head->dir = s->dir;	
}

int main() {
	snake s;
	initsnake(&s);
	s.dir = UP;
	addnode(&s);	//first node (mouth of the snake)
	for(int i = 0; i < 500; i++)
		addnode(&s);
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
	SDL_Event e;
	int Running = 1;
	int go = 0;
	while(Running) {
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT:
					Running = 0;
					break;	
				case SDL_KEYDOWN:
					go = 1;
					switch(e.key.keysym.sym) {
						case SDLK_UP:
							s.dir = UP;
							break;
						case SDLK_DOWN:
							s.dir = DOWN;
							break;
						case SDLK_RIGHT:
							s.dir = RIGHT;
							break;
						case SDLK_LEFT:
							s.dir = LEFT;
							break;
						default:
							break;		
					}	
				default:
					break;
			}
		}
		if(go) { 	
//		UpdateBoardStatus(&s);		//to update board conditions (backend)
		AD_DrawSnake(&ren, &s);		//to update borad conditions (frontend)			
//		Running = CheckGame(&s);	//checks the collision of snake with walls and with snake
		printf("X - %d, Y - %d\n", s.head->x, s.head->y);
		}
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(window);
	return 0;
}

