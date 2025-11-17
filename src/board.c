/* W Pretorius */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int **map, **covered;
static int h, w, nb, gen, nflags, rb;
static clock_t start;

void showbomb();

void freeme(void)
{
	int i;

	for (i = 0; i < h; i++) {
		free(map[i]);
		free(covered[i]);
	}

	free(map);
}

void clearmap(void)
{
	int i, j;

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			map[i][j] = 0;
			covered[i][j] = 1;
		}
	}
}


void printmenu(int i)
{
	printf("\033[2J");
	printf("\033[H");
	
	printf("\x1b[1mPLEASE SELECT LEVEL:\x1b[0m\n");
	if (i == 0) {
		printf("\t>Easy<\n");
	} else {
		printf("\t Easy\n");
	}

	if (i == 1) {
		printf("\t>Medium<\n");
	} else {
		printf("\t Medium\n");
	}

	if (i == 2) {
		printf("\t>Hard<\n");
	} else {
		printf("\t Hard\n");
	}

	if (i == 3) {
		printf("\t>Insane<\n");
	} else {
		printf("\t Insane\n");
	}
}


void bomb(int posy, int posx)
{
	int i = 0;

	srand(time(NULL));
	printf("%d, %d left for clear\n", posy, posx);

	while (i < nb) {
		int x, y;
		x = rand() % w;
		y = rand() % h;
		
		if ((x >= posx - 1) && (x <= posx + 1) && (y >= posy - 1) && (y <= posy + 1)) {
			printf("%d, %d left for clear\n", y, x);
			continue;
		}

		if (map[y][x] == 9) {
			continue;
		}
		map[y][x] = 9;
		
		if (y > 0) {
			if (map[y - 1][x] != 9) {
				map[y - 1][x]++;
			}
		}
		
		if (y < h - 1) {
			if (map[y + 1][x] != 9) {
				map[y + 1][x]++;
			}
		}

		if (x > 0) {
			if (map[y][x - 1] != 9) {
				map[y][x - 1]++;
			}
		}

		if (x < w - 1) {
			if (map[y][x + 1] != 9) {
				map[y][x + 1]++;
			}
		}
		
		if (y > 0 && x > 0) {
			if (map[y - 1][x - 1] != 9) {
				map[y - 1][x - 1]++;
			}
		}
		
		if (y < h - 1 && x < w - 1) {
			if (map[y + 1][x + 1] != 9) {
				map[y + 1][x + 1]++;
			}
		}
		
		if (y < h - 1 && x > 0) {
			if (map[y + 1][x - 1] != 9) {
				map[y + 1][x - 1]++;
			}
		}
		
		if (y > 0 && x < w - 1) {
			if (map[y - 1][x + 1] != 9) {
				map[y - 1][x + 1]++;
			}
		}
		i++;
	}
}


void start_timer()
{
	start = clock();
}


int init(int y, int x, int n)
{
	int i, j;

	h = y;
	w = x;
	nb = n;
	nflags = n;
	rb = nb;
	map = (int **)malloc(h * sizeof(int *));
	covered = (int **)malloc(h * sizeof(int *));

	if (map == NULL) {
		return EXIT_FAILURE;
	}

	for (i = 0; i < h; i++) {
		map[i] = (int *)malloc(w * sizeof(int));

		if (map[i] == NULL) {
			for (j = 0; j < i; j++) {
				free(map[j]);
			}

			free(map);
			free(covered);
			return EXIT_FAILURE;
		}
	}
	
	for (i = 0; i < h; i++) {
		covered[i] = (int *)malloc(w * sizeof(int));

		if (covered[i] == NULL) {
			for (j = 0; j < i; j++) {
				free(covered[j]);
			}
			for (j = 0; j < h; j++) {
				free(map[j]);
			}

			free(map);
			free(covered);
			return EXIT_FAILURE;
		}
	}

	clearmap();

	return EXIT_SUCCESS;
}


void selected(int y, int x)
{
	if (covered[y][x] > 2) {
		covered[y][x] -= 3;
	} else {
		covered[y][x] += 3;
	}
}


int toggle(int y, int x)
{
	if (covered[y][x] % 3 == 1) {
		covered[y][x]--;
	}

	if (map[y][x] == 0) {
		if (y > 0) {
			if (covered[y - 1][x] % 3 == 1) {
				toggle(y - 1, x);
			}
		}

		if (y < h - 1) {
			if (covered[y + 1][x] % 3 == 1) {
				toggle(y + 1, x);
			}
		}

		if (x > 0) {
			if (covered[y][x - 1] % 3 == 1) {
				toggle(y, x - 1);
			}
		}

		if (x < w - 1) {
			if (covered[y][x + 1] % 3 == 1) {
				toggle(y, x + 1);
			}
		}

		if (y > 0 && x > 0) {
			if (covered[y - 1][x - 1] % 3 == 1) {
				toggle(y - 1, x - 1);
			}
		}

		if (y < h - 1 && x < w - 1) {
			if (covered[y + 1][x + 1] % 3 == 1) {
				toggle(y + 1, x + 1);
			}
		}

		if (y < h - 1 && x > 0) {
			if (covered[y + 1][x - 1] % 3 == 1) {
				toggle(y + 1, x - 1);
			}
		}

		if (y > 0 && x < w - 1) {
			if (covered[y - 1][x + 1] % 3 == 1) {
				toggle(y - 1, x + 1);
			}
		}
	}
	
	if (map[y][x] == 9) {
		showbomb();
		return 1;
	}
	
	return 0;
}


void showbomb()
{
	int i, j;

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			if (map[i][j] == 9) {
				if (covered[i][j] == 2) {
					covered[i][j] = 10;
				} else {
					covered[i][j] = 0;
				}
			}
		}
	}
}


void flag(int y, int x)
{
	if (covered[y][x] % 3 == 2) {
		if (map[y][x] == 9) {
			rb++;
		}
		covered[y][x]--;
		nflags++;
	} else if (covered[y][x] % 3 == 1) {
		if (map[y][x] == 9) {
			rb--;
		}
		covered[y][x]++;
		nflags--;
	}
}


void printmap(void)
{
	int i, j;
	char *ch[8] = {"１","２","３","４","５","６","７","８"};
	clock_t end;

	printf("\033[2J");
	printf("\033[H");
	
	printf("W:↑, A:←, S:↓, D:→, Q: quit, Space: toggle, F: Flag\n");

	printf("╔═");
	for (i = 0; i < w; i++) {
		printf("══");
	}
	printf("═╗\n");

	for (i = 0; i < h; i++) {
		
		printf("║ ");

		for (j = 0; j < w; j++) {
			if (covered[i][j] == 1) {		/*hidden*/
				printf("\x1b[7m  \x1b[0m"); 
			} else if (covered[i][j] == 2) {		/*flag*/
				printf("\x1b[1;47;34m()\x1b[0m");
			} else if (covered[i][j] == 10) {
				printf("\x1b[1;34m◢◣\x1b[0m");
			} else if (covered[i][j] > 2) {		/*selected*/
				
				if (covered[i][j] == 5) {		/* flag selected */	
					printf("\x1b[1;47;34m[]\x1b[0m");
				} else if (covered[i][j] == 4) {	/* hidden selected*/
					printf("\x1b[1;47m[]\x1b[0m");
				} else if (map[i][j] == 9) {
					printf("\x1b[1;31m◢◣\x1b[0m");
				} else if (map[i][j] > 0) {			/* numbers shown selected */
					printf("\x1b[1m>%d\x1b[0m", map[i][j]);
				} else {
					printf("[]");
				} 
			} else {
				if (map[i][j] < 9 && map[i][j] > 0) {
					printf("%s", ch[map[i][j] - 1]);
				} else if (map[i][j] == 0) {
					printf("  ");
				} else if (map[i][j] == 9) {
					printf("\x1b[1;31m◢◣\x1b[0m");
				}
			}
		}
		printf(" ║\n");
	}
	
	printf("╚═");
	for (i = 0; i < w; i++) {
		printf("══");
	}

	printf("═╝\n");

	end = clock();

	printf("Number of Flags left: %d, Time: %.3ld\n", nflags, (end - start) / CLOCKS_PER_SEC);
}

/* BS&T */
