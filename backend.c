#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <SDL2/SDL.h>
#include "snake.h"

void Display_help(void) {
	int fd;
	char b;
	fd = open("snake_help.txt", O_RDONLY);
	if(fd == -1) {
		fprintf(stderr, "Could not find help.txt\n");
		return;
	}	
	while(read(fd, &b, sizeof(char)))
		if(b != '>' && b != '<')
			printf("%c", b);
	close(fd);
}

void Display_highscore(void) {
	int fd;
	long int sscore = 0, tscore = 0;
	fd = open(".snake", O_RDONLY);
	if(fd == -1) {
		printf("\tHighscore (single player): %ld\n\tHighscore (double player): %ld\n", sscore, tscore);
		return;
	}
	read(fd, &sscore, sizeof(long int));
	read(fd, &tscore, sizeof(long int));
	printf("\tHighscore (single player): %ld\n\tHighscore (double player): %ld\n", sscore, tscore);
}

long int UpdateHighscore(long int score, int tflag) {
	int fd;
	long int sscore, tscore, retscore;
	fd = open(".snake", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd == -1) {
		fprintf(stderr, "Could not find help.txt\n");
		return -1;
	}
	read(fd, &sscore, sizeof(long int));
	read(fd, &tscore, sizeof(long int));
	lseek(fd, SEEK_SET, 0);
	if(tflag) {
		retscore = tscore;
		if(score > retscore)
			write(fd, &score, sizeof(long int));
	}		
	else { 	
		retscore = sscore;	
		if(score > retscore) {
			lseek(fd, SEEK_SET, sizeof(long int));
			write(fd, &score, sizeof(long int));
		}
	}	
	return (score > retscore ? score : retscore);
}

void Display_controls(void) {
	int fd;
	char b;
	fd = open("snake_help.txt", O_RDONLY);
	if(fd == -1) {
		fprintf(stderr, "Could not find help.txt\n");
		return;
	}
	while(b != '>')
		read(fd, &b, sizeof(char));
	while(b != '<') {
		read(fd, &b, sizeof(char));
		printf("%c", b);
	}	
	close(fd);
}

void Reset_highscore(void) {
	int fd;
	fd = open(".snake", O_WRONLY);
	if(fd == -1)
		return;
	long int i = 0;
	write(fd, &i, sizeof(long int));
	write(fd, &i, sizeof(long int));
	close(fd);
}

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
	if(n == NULL) {
		fprintf(stderr, "Not enough RAM!\n");
		exit(6);
	}	
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

int collision = 0;

State CheckGame(snake *s, struct XY co) {
	if(s->dir == ND)
		return SNAKE_HALTED;
	if(collision == 1)
		return SNAKE_COLLISION_SNAKE;
	if((s->head->x <= 8 || s->head->x >= 792 || s->head->y <= 8 || s->head->y >= 792))
		return SNAKE_COLLISION_WALL;
	if((s->head->x == co.x && s->head->y == co.y - BLOCKSIZE) || (s->head->x == co.x - BLOCKSIZE && s->head->y == co.y)) {
		if(co.t == SMALL)
			return SNAKE_FOOD_SMALL;
		else 
			return SNAKE_FOOD_LARGE;		
	}	
	return SNAKE_PROPAGATING;	
}

void DestroyAllNodes(node *n); //just like a private function. need not to be displayed in the header
 
void DestroySnake(snake *s) {
	node *n = s->head;
	DestroyAllNodes(n);
}

void DestroyAllNodes(node *n) {
	if(n == NULL)
		return;
	DestroyAllNodes(n->next);
	free(n);	
}

void SnakeCollisionTest(snake *s) {
	static int i = 1;
	if(i) {
		node *n = s->head->next;
		if(n) {
			n = n->next;
			int x = s->head->x, y = s->head->y;
			while(n) {
				if(n->x == x && n->y == y) {
					i = 0;
					collision = 1;
					return;
				}	
				n = n->next;
			}
		}
	}	
}

