#ifndef BOARD_H
#define BOARD_H

int init(int y, int x, int n);
void printmap(void);
void start_timer(void);
void printmenu(int i);
void freeme(void);
void selected(int y, int x);
int toggle(int y, int x);
void flag(int y, int x);
void bomb(int y, int x);

#endif
