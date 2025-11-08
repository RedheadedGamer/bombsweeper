#ifndef BOARD_H
#define BOARD_H

int init(int y, int x, int n);
void printmap(int mode);
void printmenu(int i);
void freeme(void);
void selected(int y, int x);
int toggle(int y, int x);
void flag(int y, int x);

#endif
