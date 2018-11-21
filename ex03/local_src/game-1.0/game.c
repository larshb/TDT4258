#include <stdio.h>
#include <stdlib.h>

void font_init();
void screen_init();
void screen_clear();
int gamepad_init();

int snake_play();
void animate_diamond();
void darth_plagueis();

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");

	screen_init();
	//animate_diamond(); /* FIXME Slow af */

	gamepad_init();
	font_init();

	screen_clear();
	if( snake_play()!=0 ) {
		puts(" __   __                        _                                                  _    __   _ ");
		puts(" \\ \\ / /  ___   _  _     __ _  | |_   ___     _  _   ___   _  _   _ _   ___  ___  | |  / _| | |");
		puts("  \\ V /  / _ \\ | || |   / _` | |  _| / -_)   | || | / _ \\ | || | | '_| (_-< / -_) | | |  _| |_|");
		puts("   |_|   \\___/  \\_,_|   \\__,_|  \\__| \\___|    \\_, | \\___/  \\_,_| |_|   /__/ \\___| |_| |_|   (_)");
		puts("                                              |__/                                             ");
	}

	darth_plagueis();

	exit(EXIT_SUCCESS);
}
