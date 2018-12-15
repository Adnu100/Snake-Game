#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "snake.h"

int Random(int Range_Start, int Range_End) {
	srand((unsigned int)(time(NULL) + rand() + rand()));
	return (rand() % (1 + (Range_End > Range_Start ? Range_End : Range_Start) - (Range_Start < Range_End ? Range_Start : Range_End)) + (Range_Start < Range_End ? Range_Start : Range_End));
}

void initsnake(snake *s) {
	s->head = s->tail = NULL;
	s->speed = STARTSPEED;
	s->dir = ND;
}

void addnode(snake *s) {
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

struct XY RandomBall(snake *s) {
	int x, y;
	node *n;
	while(1) {
		n = s->head;
		x = Random(1, 800 / (2 * BLOCKSIZE) - 1);
		y = Random(2, 800 / (2 * BLOCKSIZE) - 1);
		x *= (BLOCKSIZE * 2);
		y *= (BLOCKSIZE * 2);
		y -= BLOCKSIZE;
		while(n) {
			if(n->x == x && n->y == y)
				break;
			n = n->next;	
		}
		if(!n)
			break;
	}
	struct XY ret;
	ret.x = x;
	ret.y = y;
	ret.t = Random(0, BIG_FOOD_PROBABILITY) ? SMALL : BIG;
	return ret;
}

State CheckGame(snake *s, struct XY co) {
	if(s->dir == ND)
		return SNAKE_HALTED;
	int x = s->head->x;
	int y = s->head->y;
	node *n = s->head->next;
	while(n) {
		if(n->x == x && n->y == y)
			return SNAKE_COLLISION_SNAKE;
		n = n->next;		
	}
	if((s->head->x == 8 || s->head->x == 792 || s->head->y == 8 || s->head->y == 792))
		return SNAKE_COLLISION_WALL;
	if(s->head->x == co.x && s->head->y == co.y) {
		if(co.t == SMALL)
			return SNAKE_FOOD_SMALL;
		else 
			return SNAKE_FOOD_LARGE;		
	}	
	return SNAKE_PROPAGATING;	
}
