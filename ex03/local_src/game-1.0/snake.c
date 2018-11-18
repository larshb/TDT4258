#include "screen.h"

#include <stdint.h> // uint8_t ...
#include <stdlib.h> // random()
#include <malloc.h> // malloc()
#include <time.h>   // time()

/* TODO
 * Consider adding cheats
 */

typedef struct {
	int8_t x;
	int8_t y;
} coords_t;

typedef coords_t food_t;

/* "Body parts" */
typedef struct node {
	coords_t coords;
	struct node* next;
} node_t;

typedef enum { UP, DOWN, LEFT, RIGHT } dir_t;

typedef struct snake {
	node_t* head;
	node_t* tail;
	dir_t dir;
} snake_t;

/* Add new snake-head according to moving direction */
int snake_grow(snake_t* snake) {
	/* Add new head according to direction, and wrap around screen edges */
	dir_t* d = &snake->dir;
	coords_t c = snake->head->coords;
	node_t* new_head = malloc(sizeof(node_t));
	new_head->next = snake->head;
	new_head->coords.x = (
		(*d == UP || *d == DOWN)
		? c.x
		: (*d == RIGHT
			? (c.x==19?0:c.x+1) // MAGIC NUMBER (20*16 screen width)
			: (c.x==0?19:c.x-1) // MAGIC NUMBER
			)
		);
	new_head->coords.y = (
		(*d == LEFT || *d == RIGHT)
		? c.y
		: (*d == DOWN
			? (c.y==14?0:c.y+1) // MAGIC NUMBER (15*16 screen height)
			: (c.y==0?14:c.y-1) // MAGIC NUMBER
			)
		);
	snake->head = new_head;

	/* Check for collision */
	node_t* node = snake->head->next;
	while (node->next != NULL) {
		if (node->coords.x == snake->head->coords.x &&
			node->coords.y == snake->head->coords.y)
		{
			return -1;
		}
		node = node->next;
	}

	return 0;
}

int snake_move(snake_t* snake) {
	if (snake_grow(snake) != 0) {
		return -1;
	}

	/* Trim tail */

	/* Find next to last node
	 * FIXME Consider adding pointer to previous node
	 * so the following scan is not needed
	 */
	node_t* new_tail = snake->head;
	while(new_tail->next != snake->tail) {
		new_tail = new_tail->next;
	}

	new_tail->next = NULL;

	/* FIXME memory leak? */
	//free(snake->tail);

	snake->tail = new_tail;

	return 0;
}

inline void square_draw(coords_t* c, uint16_t color) {
	draw_rectangle(c->x*16, c->y*16, c->x*16+15, c->y*16+15, color);
}

void snake_draw(snake_t* snake) {
	/* FIXME Optimize redrawing only head and tail */
	node_t* node = snake->head;
	while (node != NULL) {
		coords_t* c = &node->coords;
		square_draw(c, RED);
		node = node->next;
	}
}

void snake_turn(snake_t* snake, dir_t dir) {
	switch (dir) {
		case    UP: if (snake->dir !=  DOWN) snake->dir =    UP; break;
		case  DOWN: if (snake->dir !=    UP) snake->dir =  DOWN; break;
		case  LEFT: if (snake->dir != RIGHT) snake->dir =  LEFT; break;
		case RIGHT: if (snake->dir !=  LEFT) snake->dir = RIGHT; break;
	}
}

void food_move(food_t* food) {
	food->x = random()%20;
	food->y = random()%15;
}

void food_draw(food_t* food) {
	square_draw(food, YELLOW);
}

int gamepad_init();
int gamepad_read();

/* Bitfield masks for buttons */
enum btn_msk {
	MSK_SW1 = (1 << 0),
	MSK_SW2 = (1 << 1),
	MSK_SW3 = (1 << 2),
	MSK_SW4 = (1 << 3),
	MSK_SW5 = (1 << 4),
	MSK_SW6 = (1 << 5),
	MSK_SW7 = (1 << 6),
	MSK_SW8 = (1 << 7)
};

int snake_play() {

	gamepad_init();
	screen_init();
	screen_clear();

	srand(time(NULL));   // Initialization, should only be called once.

	food_t food;
	food_move(&food);

	/* Init */
	snake_t snake;
	//snake.grow = snake_grow;

	node_t tail = {
		.coords = {5,5},
		.next = NULL
	};
	node_t head = {
		.coords = tail.coords,
		.next = &tail
	};
	snake.head = &head;
	snake.tail = &tail;
	snake.dir = RIGHT;

	int i;
	for (i = 0; i < 5; i++) {
		snake_grow(&snake);	
	}

	uint8_t btns = 0;
	while (1) {//(!(btns & MSK_SW4)) { //Exit

		if (btns & MSK_SW1) {
			snake_turn(&snake, LEFT);
		}
		if (btns & MSK_SW2) {
			snake_turn(&snake, UP);
		}
		if (btns & MSK_SW3) {
			snake_turn(&snake, RIGHT);
		}
		if (btns & MSK_SW4) {
			snake_turn(&snake, DOWN);
		}

		/* Eat food if head in same location
		 * FIXME Optimize by only trimming tail if "not eating"
		 */
		if (snake.head->coords.x == food.x &&
			snake.head->coords.y == food.y)
		{
			food_move(&food);
			if (snake_grow(&snake) != 0) {
				return -1;
			}
		}
		else
		{
			if (snake_move(&snake) != 0) {
				return -2;
			}
		}
		
		
		screen_clear(); // Not optimal
		food_draw(&food);
		snake_draw(&snake);
		screen_refresh();
		//usleep((int)1e5);
		btns = gamepad_read();
	}
	return 0;
}
