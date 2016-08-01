#include "player.h"

void destroy_player(player_descr * sacrafice){
	free(sacrafice);
}
player_descr* make_player(){
	player_descr* play = malloc(sizeof(player_descr));
	play->max_hp=100;
	play->hp=100;
	play->dodge=20;
	play->max_roll=100;
	play->min_dmg=10;
	play->max_dmg=150;
	return play;
}
