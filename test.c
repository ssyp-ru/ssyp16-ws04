#include "game.h"
#include "dungeon.h"
#include "monster.h"
#include "game.h"

void test_load() {
	player_descr* pc = malloc(sizeof(player_descr));
	FILE* example = fopen("example.txt", "r");
	dungeon* d = load_dungeon(example, pc);
	destroy_dungeon(d);		
}

void test_cycle() {
	monster_descr * rat_test = malloc(sizeof(monster_descr));
	rat_test->pic = '$';
	rat_test->name = "Rat";
	rat_test->max_hp = 10;
	rat_test->dodge = 20;
	rat_test->max_roll = 100;
	rat_test->max_dmg = 10;
	rat_test->min_dmg = 1;
	
	//TEST//fix
	player_descr * player = malloc(sizeof(player_descr));
	player->max_hp = 20;
	player->hp = 20;		
	player->dodge = 20;
	player->max_roll = 100;
	player->max_dmg = 20;
	player->min_dmg = 5;	

	monster * rat1;
	rat1 = make_monster(rat_test);
		
	player_hit(player, rat1);
	monster_hit(rat1, player);
	// TODO: add test
}

void test_message() {
	initscr();
	init_msg_display();
	message("PRESS ANY KEY FOR NEXT MESSAGE");
	getch();
	message("PRESS ANY KEY FOR LAST MESSAGE");
	getch();
	message("PRESS ANY KEY TO EXIT");
	getch();
	endwin();
}

void test_gen() {
	//TODO: add test
}

void test_init() {
	init();
}





int main() {
	init();
	return 0;
}
