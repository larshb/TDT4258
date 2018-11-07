#include <stdio.h>
#include <stdlib.h>

//#include "gamepad.c"
#include "screen.c"
//#include "framebuffer.c"

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");

	//gamepad_test();
	screen_test();
	//framebuffer_test();

	exit(EXIT_SUCCESS);
}
