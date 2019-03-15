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
		fprintf(stderr, "Could not find .snake (highscores file)\n");
		return -1;
	}
	read(fd, &sscore, sizeof(long int));
	read(fd, &tscore, sizeof(long int));
	lseek(fd, SEEK_SET, 0);
	if(tflag) {
		retscore = tscore;
		if(score > retscore) {
			lseek(fd, sizeof(long int), SEEK_SET);
			write(fd, &score, sizeof(long int));
		}	
	}		
	else { 	
		retscore = sscore;	
		if(score > retscore)
			write(fd, &score, sizeof(long int));
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

struct XY RandomBall(snake *s, snake *t, int tflag) {
	int x, y;
	node *n, *m;
	while(1) {
		n = s->head;
		m = t->head;
		x = Random(1, 800 / (2 * BLOCKSIZE) - 1);
		y = Random(2, 800 / (2 * BLOCKSIZE) - 1);
		x *= (BLOCKSIZE * 2);
		y *= (BLOCKSIZE * 2);
		while(n) {
			if(n->x == x && n->y == y)
				break;
			n = n->next;	
		}
		if(tflag) {
			while(m) {
				if(m->x == x && m->y == y)
					break;
				m = m->next;	
			}
		}
		else 
			m = NULL;	
		if(!(n || m))
			break;
	}
	struct XY ret;
	ret.x = x;
	ret.y = y;
	ret.t = Random(0, BIG_FOOD_PROBABILITY) ? SMALL : BIG;
	return ret;
}

State CheckGame(snake *s, snake *t, struct XY co, int tflag) {
	if(s->dir == ND)
		return SNAKE_HALTED;
	switch(SnakeCollisionTest(s, t, tflag)) {
		case 0:
			break;
		case 1:
			return SNAKE_COLLISION_SNAKE;
			break;
		case 2:
			return SNAKE_COLLISION_SNAKE_s;
			break;
		case 3:
			return SNAKE_t_COLLISION_SNAKE_s;
			break;
		case 4:
			return SNAKE_s_COLLISION_SNAKE_t;
			break;
		case 5:
			return SNAKE_COLLISION_SNAKE_t;
			break;
		case 6:
			return HEAD_TO_HEAD_COLLISION;
			break;	
		default:
			break;			
	}	
	if(!tflag) {
		if((s->head->x <= 8 || s->head->x >= 792 || s->head->y <= 8 || s->head->y >= 792))
			return SNAKE_COLLISION_WALL;
		if((s->head->x == co.x && s->head->y == co.y) || (s->head->x == co.x && s->head->y == co.y)) {
			if(co.t == SMALL)
				return SNAKE_FOOD_SMALL;
			else 
				return SNAKE_FOOD_LARGE;		
		}
	}
	else {
		if((s->head->x <= 8 || s->head->x >= 792 || s->head->y <= 8 || s->head->y >= 792))
			return SNAKE_COLLISION_WALL;
		if((s->head->x == co.x && s->head->y == co.y) || (s->head->x == co.x && s->head->y == co.y)) {
			if(co.t == SMALL)
				return SNAKE_FOOD_SMALL_s;
			else 
				return SNAKE_FOOD_LARGE_s;		
		}
		if((t->head->x <= 8 || t->head->x >= 792 || t->head->y <= 8 || t->head->y >= 792))
			return SNAKE_COLLISION_WALL;
		if((t->head->x == co.x && t->head->y == co.y) || (t->head->x == co.x && t->head->y == co.y)) {
			if(co.t == SMALL)
				return SNAKE_FOOD_SMALL_t;
			else 
				return SNAKE_FOOD_LARGE_t;		
		}
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

int SnakeCollisionTest(snake *s, snake *t, int tflag) {
	static int i = 1;
	if(!tflag) {
		if(i) {
			node *n = s->head->next;
			if(n) {
				n = n->next;
				int x = s->head->x, y = s->head->y;
				while(n) {
					if(n->x == x && n->y == y) {
						i = 0;
						return 1;
					}	
					n = n->next;
				}
			}
		}
	}
	else {
		if(i) {
			node *n = s->head->next, *m = t->head->next;
			int x = s->head->x, y = s->head->y, xt = t->head->x, yt = t->head->y;
			while(n) {
				if(n->x == x && n->y == y) {
					i = 0;
					return 2;			//snake s collided with itself
				}
				if(n->x == xt && n->y == yt) {
					i = 0;
					return 3;			//snake t collided with snake s 	
				}
				n = n->next;
			}
			while(m) {
				if(m->x == x && m->y == y) {
					i = 0;
					return 4;			//snake s collided with snake t
				}	
				if(m->x == xt && m->y == yt) {
					i = 0;
					return 5;			//snake t collided with itself
				}
				m = m->next;
			}
			if(s->head == t->head)
				return 6;				//head to head collision
		}
	}
	return 0;		
}

