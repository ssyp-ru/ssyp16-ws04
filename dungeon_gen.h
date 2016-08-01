#include "dungeon.h"


/* I'm not entirely sure about these, so let's not use them! yet.
dungeon* d build_dungeon();

level* build_level(); */
typedef struct {
	point tl; // top left
	point br; // bottom right
} room;
typedef struct list_t{
	room* data;
	struct list_t* next;
}list;
typedef point chunk;

typedef struct {
	int mapX, mapY;
	char** map;
	list* rlist;
	room*** rooms;
	int chunkX, chunkY;
	int minroom;
	//TODO: add room list itself
	//room list length
	int rlist_len;
	int chunksize;
} level_inner;

level_inner* build_level_inner(int mapX, int mapY, int chunksize, int minroom, int room_number);
void print_level(level_inner* l);
dungeon* create_dungeon(FILE* descr_file);
