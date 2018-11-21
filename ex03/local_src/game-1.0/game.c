#include <stdio.h>
#include <stdlib.h>

int snake_play();
void screen_test();
void darth_plagueis();

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");

	// screen_test();
	// exit(0);

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
