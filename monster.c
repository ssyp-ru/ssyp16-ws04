#include "monster.h"
monster * make_monster(monster_descr * type){
	monster * out;
	out = malloc(sizeof(monster));
	out->hp = type->max_hp;
	out->descr = type;

	return out;
}

void destroy_monster(monster * sacrafice){
	free(sacrafice);
	//+ Player XP
}


void player_hit(player_descr * target, monster * attack){
	
	if (rand() % (attack->descr->max_roll) > (target->dodge)){
		int dmg;
		dmg = attack->descr->min_dmg;
		dmg += rand()%((attack->descr->max_dmg) - (attack->descr->min_dmg));
		target->hp = (target->hp)-dmg;
		message("You were hit!");
	}else 
		message("You dodged");
}

void monster_hit(monster * target, player_descr * attack){

	if (rand() % (attack->max_roll) > (target->descr->dodge)){
		int dmg;
		dmg = attack->min_dmg;
		dmg += rand()%((attack->max_dmg) - (attack->min_dmg));
		target->hp = (target->hp)-dmg;
		message("You hit!");
	}else
		message("The monster dodged");
}
