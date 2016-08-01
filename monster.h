#pragma once
#include "player.h"
typedef struct {
	char pic;
	char * name;
	//DEFF
	int max_hp;
	int dodge;
	//ATK
	int max_roll;
	int max_dmg;
	int min_dmg;
} monster_descr;

typedef struct {
	int hp;
	monster_descr * descr;
} monster;

/* Makes monster from description. Allocates memory. */
monster* make_monster(monster_descr * type);

/* Both free up memory taken up by the structure. 
   THEY DO NOT FREE MONSTER_DESCR    */
void destroy_monster(monster * sacrafice);

/* Process hits from player to monster and vice versa */
void player_hit(player_descr * target, monster * attack);
void monster_hit(monster * target, player_descr * attack);
