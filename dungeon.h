#pragma once
#include "player.h"
#include "monster.h"
typedef struct {
	/* Tile-based map of the dungeon */
	char** map;
	
	/* Map size in x and y dimensions */
	int mapx, mapy;

	/* Up and down stairs locations */
	point upstairs, downstairs;

	/* Monster map (or monster layer)		mon_map[i][j] == NULL <=> no monster at i,j */
	monster*** mon_map;
} level;

typedef struct {
	/* Pointer array of all levels i n the dungeon. */
	level** lvls;

	/* Total number of levels */
	int lvl_num;

	/* Current level (where the player is). */
	int curr_lvl;

	/* player coordinates on the current level */
	point pc_coor;

	/* The player himself */
	player_descr* pc;

	/* All used monster descriptions */
	monster_descr** descriptions;

	/* Total monster description number */
	int descr_num;	
} dungeon;

/* Loads the dungeon from input, allocates ALL the required memory, except for player_descr -- this is done elsewhere */
dungeon* load_dungeon(FILE* input, player_descr* pc);

/* Frees all the memory allocated in the load_dungeon function. DOES NOT FREE THE PLAYER_DESCR!!!!*/
void destroy_dungeon(dungeon* d);

level* level_read(FILE* input, monster_descr** descriptions, int descr_num);

void destroy_level(level* l);

monster_descr* read_descr(FILE* input);

dungeon* load_dungeon(FILE* input, player_descr* pc);

dungeon* load_full(FILE* input);

void write_level(FILE* save,level* l);

void write_descr(FILE* save, monster_descr*m);

void save_dungeon(FILE* save, dungeon* d);

void save_full(FILE* save, dungeon* d);


