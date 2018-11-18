#include <stdio.h>
#include <stdlib.h>

//#include "gamepad.c"
//#include "screen.c"
//#include "snake.c"

void snake_play();
void screen_test();

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");

	//gamepad_test();
	//screen_test();
	snake_play();

	exit(EXIT_SUCCESS);
}
