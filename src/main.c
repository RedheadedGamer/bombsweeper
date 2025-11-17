/* W Pretorius */

#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include "board.h"

struct termios old_tio;

void set_input_raw() 
{
	struct termios new_tio;

	tcgetattr(STDIN_FILENO, &old_tio);

	new_tio = old_tio;

	new_tio.c_lflag &= ~(ICANON | ECHO);
	new_tio.c_cc[VMIN] = 1;
	new_tio.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void reset_input() 
{
	tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

int getch() 
{
	char c;
	if (read(STDIN_FILENO, &c, 1) == -1) {
		return 0;
	}
	return c;
}

int start_game(int *h, int *w)
{
	int i = -1, inmenu = 1;
	char c;


	while (inmenu) {
		printmenu(i);

		c = getch();
		if (c == 's') {
			i++;
		} else if (c == 'w') {
			i--;
		} else if (c == ' ') {
			if (i > -1 &&  i < 4) {
				inmenu = 0; 
			}
		} else if (c == 'q') {
			return 1;
		}
		
		if (i > 3) {
			i = 3;
		} else if (i < 0) {
			i = 0;
		}
	}

	switch (i) {
		case 0:
			init(9, 9, 10);
			*h = 9;
			*w = 9;
			break;
		case 1:
			init(15, 15, 40);
			*h = 15;
			*w = 15;
			break;
		case 2:
			init(16, 30, 99);
			*h = 16;
			*w = 30;
			break;
		case 3:
			init(20, 30, 150);
			*h = 20;
			*w = 30;
			break;
	}

	return 0;
}



int main(int argc, char *argv[])
{
	int h = 8, w = 8, first = 1;
	int posy = 0, posx = 0;
	char game = 1;
	char c;


	set_input_raw();
	
	if (start_game(&h, &w) == 1) {
		return 0;
	}

	selected(posy, posx);
	printmap();

	while (game) {
		printmap();

		c = getch();
		if (c == 'q') {
			game = 0;
		} else if (c == 'a') {
			selected(posy, posx);
			posx--;
			
			if (posx < 0) {
				posx = w - 1;
			}
			
			selected(posy, posx);
		} else if (c == 'd') {
			selected(posy, posx);
			posx++;

			if (posx >= w) {
				posx = 0;
			}

			selected(posy, posx);
		} else if (c == 'w') {
			selected(posy, posx);
			posy--;
			
			if (posy < 0) {
				posy = h - 1;
			}
			
			selected(posy, posx);
		} else if (c == 's') {
			selected(posy, posx);
			posy++;

			if (posy >= h) {
				posy = 0;
			}

			selected(posy, posx);
		} else if (c == ' ') {
			if (first) {
				bomb(posy, posx);
				start_timer();
				first = 0;
			}
			if (toggle(posy, posx) == 1) {
				game = 0;
			};
		} else if (c == 'f') {
			flag(posy, posx);
		}

	}

	
	selected(posy, posx);
	printmap();

	freeme();

	reset_input();

	return EXIT_SUCCESS;
}

