#define STARTSPEED 2
#define BLOCKSIZE 8
#define SNAKENODE BLOCKSIZE - 2
#define INITIALX 400
#define INITIALY 400
#define INITIALX_TWOPLAYER_SNAKE_s 416
#define INITIALY_TWOPLAYER_SNAKE_s 400
#define INITIALX_TWOPLAYER_SNAKE_t 384
#define INITIALY_TWOPLAYER_SNAKE_t 400
#define BIG_FOOD_PROBABILITY 7
#define LV1 250
#define LV2 1000
#define LV3 7000
#define LV4 20000

typedef enum direction {UP, DOWN, LEFT, RIGHT, ND} direction;	//directions
typedef enum SNAKE_State {
	SNAKE_FOOD_SMALL,					//snake eating small food (single player)
	SNAKE_FOOD_LARGE, 					//snake eating large food (single player)
	SNAKE_FOOD_SMALL_s,					//snake s eating small food
	SNAKE_FOOD_SMALL_t,					//snake t eating small food
	SNAKE_FOOD_LARGE_s,					//snake s eating large food
	SNAKE_FOOD_LARGE_t,					//snake t eating large food
	SNAKE_COLLISION_WALL, 					//snake collided with wall (single player)
	SNAKE_s_COLLISION_WALL,					//snake s collision with wall
	SNAKE_t_COLLISION_WALL,					//snake t collision with wall
	SNAKE_COLLISION_SNAKE,					//single player
	SNAKE_COLLISION_SNAKE_s,				//snake s collided with itself
	SNAKE_COLLISION_SNAKE_t,				//snake t collided with itself
	SNAKE_s_COLLISION_SNAKE_t,				//snake s collided with snake t
	SNAKE_t_COLLISION_SNAKE_s,				//snake t collided with snake s
	HEAD_TO_HEAD_COLLISION,					//head to head collision of two snakes
	SNAKE_PROPAGATING,					//no event
	SNAKE_HALTED						//game paused, not started, etc
} State;							//the states of the snake

typedef enum type {SMALL, BIG} type;				//food type

typedef struct node {
	int x, y;						//x and y coordinates of the block
	direction dir;						//direction of the block
	struct node *next, *prev;	
} node;

typedef struct snake {
	int speed;						//speed of the snake
	direction dir;						//diretion of the head of the snake
	node *head, *tail;
} snake;

struct XY {
	int x;							
	int y;
	type t;
};								//food structure containing 
								//x and y coordinates of food
								//the random type of 
								//food (small and large food)

void initsnake(snake *s);
void addnode(snake *s);
void UpdateBoardStatus(snake *s);
void AD_DrawSnake(SDL_Renderer **ren, snake *s, snake *t, struct XY co, long int score, int tflag);
void AD_DrawCircle(SDL_Renderer **ren, float center_x_coordinate, float center_y_coordinate, float radius, int r, int g, int b, int a);
int Random(int Range_Start, int Range_End);
struct XY RandomBall(snake *s, snake *t, int tflag);
State CheckGame(snake *s, snake *t, struct XY co, int tflag);
int SnakeCollisionTest(snake *s, snake *t, int tflag);
void DestroySnake(snake *s);
void Display_help(void);
void Display_highscore(void);
void Display_controls(void);
long int UpdateHighscore(long int score, int tflag);
void Reset_highscore(void);

