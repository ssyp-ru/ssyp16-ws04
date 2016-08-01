#pragma once
#include "misc.h"
typedef struct {
	//DEFF
	int max_hp;
	int hp;
	int dodge;
	//ATK
	int max_roll;
	int max_dmg;
	int min_dmg;
} player_descr;
void destroy_player(player_descr * sacrafice);
player_descr* make_player();
