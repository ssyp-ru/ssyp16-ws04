#pragma once
#include <stdlib.h>
#include <ncurses.h>
// A simple point structure
typedef struct {
	int x, y;
} point;

/* Generates a random integer in this range */
int rand_int_range(int min, int max);
