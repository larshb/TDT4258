#include "screen.h"

#include <stdint.h> // uint8_t ...
#include <stdlib.h> // random()
#include <stdio.h>  // sprinft()
#include <malloc.h> // malloc()
#include <time.h>   // time()
#include <unistd.h> // usleep()

#define SNAKE_BLOCKSIZE 16
#define SNAKE_XMAX (SCREEN_WIDTH / SNAKE_BLOCKSIZE)
#define SNAKE_YMAX (SCREEN_HEIGHT / SNAKE_BLOCKSIZE - 1)
/*                                                    |
 *      Leave some room on bottom for other stuff ----+
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

food_t food; /* FIXME Should not need to be a global */

inline void square_draw(coords_t* c, uint16_t color) {
	draw_rectangle(c->x*SNAKE_BLOCKSIZE, c->y*SNAKE_BLOCKSIZE,
				   c->x*SNAKE_BLOCKSIZE+SNAKE_BLOCKSIZE-1, c->y*SNAKE_BLOCKSIZE+SNAKE_BLOCKSIZE-1, color);
}

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
			? (c.x==(SNAKE_XMAX-1)?0:c.x+1)
			: (c.x==0?(SNAKE_XMAX-1):c.x-1)
			)
		);
	new_head->coords.y = (
		(*d == LEFT || *d == RIGHT)
		? c.y
		: (*d == DOWN
			? (c.y==(SNAKE_YMAX-1)?0:c.y+1)
			: (c.y==0?(SNAKE_YMAX-1):c.y-1)
			)
		);
	snake->head = new_head;
	square_draw(&new_head->coords, RED);

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

/* Trim snake's tail */
int snake_trim(snake_t* snake) {
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

	square_draw(&snake->tail->coords, BLACK);

	/* Quickfix just in case food spawns under snake */
	if (snake->tail->coords.x == food.x && snake->tail->coords.y == food.y)
		square_draw(&food, YELLOW);

	snake->tail = new_tail;

	return 0;
}

int snake_move(snake_t* snake) {
	snake_trim(snake);
	return (snake_grow(snake) == 0) ? 0 : -1;
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
	food->x = random()%SNAKE_XMAX;
	food->y = random()%SNAKE_YMAX;
	square_draw(food, YELLOW);
}

int gamepad_read();
void screen_print(char*, uint8_t, uint8_t);

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

	//draw_rectangle(0,0,320,240, WHITE); /* Christmas mode */

	uint16_t score = 0;
	char score_str[16] = {0};
	sprintf(score_str, "Score: 00000");

	/* Init */
	srand(time(NULL));
	food_move(&food);
	snake_t snake;

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
	uint8_t delay_factor = 10;
	while (1) {
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

		/* Cheats */

		/* Speed */
		if (btns & MSK_SW5) {
			/* Slow down */
			delay_factor++;
		}	
		if (btns & MSK_SW7) {
			/* Speed up */
			delay_factor=delay_factor==0?0:delay_factor-1;
		}

		/* Length */
		if (btns & MSK_SW6) {
			snake_grow(&snake);
		}
		else if (btns & MSK_SW8) {
			snake_trim(&snake);
		}

		/* Eat food if head in same location
		 * FIXME Optimize by only trimming tail if "not eating"
		 */
		if (snake.head->coords.x == food.x &&
			snake.head->coords.y == food.y)
		{
			sprintf(score_str, "Score: %05d", score++);
			if (snake_grow(&snake) != 0) {
				return -1;
			}
			food_move(&food);
		}
		else
		{
			if (snake_move(&snake) != 0) {
				return -2;
			}
		}

		screen_print(score_str, 28, 29);
		
		usleep((int)1e4*delay_factor);

		btns = gamepad_read();
	}
	return 0;
}
