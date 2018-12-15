#define STARTSPEED 8
#define BLOCKSIZE 8
#define SNAKENODE BLOCKSIZE - 2
#define INITIALX 400
#define INITIALY 400
#define BIG_FOOD_PROBABILITY 5

typedef enum direction {UP, DOWN, LEFT, RIGHT, ND} direction;	//directions
typedef enum SNAKE_State {
	SNAKE_FOOD_SMALL,
	SNAKE_FOOD_LARGE, 
	SNAKE_COLLISION_WALL, 
	SNAKE_COLLISION_SNAKE,
	SNAKE_PROPAGATING,
	SNAKE_HALTED	
} State;							//the states of the snake
typedef enum type {SMALL, BIG} type;

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
};

void initsnake(snake *s);
void addnode(snake *s);
void UpdateBoardStatus(snake *s);
void AD_DrawSnake(SDL_Renderer **ren, snake *s, struct XY co);
void AD_DrawCircle(SDL_Renderer **ren, float center_x_coordinate, float center_y_coordinate, float radius, int r, int g, int b, int a);
int Random(int Range_Start, int Range_End);
struct XY RandomBall(snake *s);
State CheckGame(snake *s, struct XY co);
