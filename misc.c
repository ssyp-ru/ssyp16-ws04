#include "misc.h"
int rand_int_range(int min, int max) {
	return min+rand()%(max-min);
}
