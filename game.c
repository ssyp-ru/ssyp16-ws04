#include "game.h"

#define DISPLAYX 51
#define DISPLAYY 21

/* 20 PUSHUPS FOR ME!!11 */
WINDOW* msg_display;
WINDOW* main_display;
WINDOW* status_bar;
void monster_turn(dungeon*);
/* Creates the message window at the right position */
void init_msg_display() {
	int x, y;
	getmaxyx(stdscr, y, x);
	int needy = y/2;
	int needx = x/2;
	needy = needy + DISPLAYY/2;
	needx = needx - DISPLAYX/2;
	msg_display = newwin(1, DISPLAYX, needy+2, needx);
}

/* Creates the main display at the right position. */
void init_main_display() {
	int x, y;
	getmaxyx(stdscr, y, x);
	main_display = newwin(DISPLAYY, DISPLAYX, y/2 - DISPLAYY/2 , x/2 - DISPLAYX/2);
}

void init_status_bar() {
	int x, y;
	getmaxyx(stdscr, y, x);
	status_bar = newwin(1, DISPLAYX, y/2 - DISPLAYY/2 - 1, x/2 - DISPLAYX/2);
}
/* Refreshes the message bar, prints the new message there. */
void message(char* string) {
	wclear(msg_display);
	wprintw(msg_display, "%s",string);
	wrefresh(msg_display);
}
/* Ends the game, frees the memory, exits curses mode. */
void leave(dungeon* d, char* msg) {
	message(msg);
	getch();
	destroy_player(d->pc);
	destroy_dungeon(d);
	delwin(msg_display);
	delwin(main_display);
	delwin(status_bar);
	endwin();
}

void start_screen() {
	move(DISPLAYY/2 - 4, 5);
	printw("Would you like to:\n");
	printw("\t[l]oad a saved game,\n");
	printw("\tPlay in [s]tory mode,\n");
	printw("\t[g]enerate a dungeon and start,\n");
	printw("\tor [q]uit?\n");
	refresh();
	int k = getch();
	clear();
	refresh();
	switch (k) {
		case 'l': {
			FILE* example = fopen("save.txt", "r");
			dungeon* d = load_full(example);
			fclose(example);
			cycle(d);
			break;
		}
		case 's': {
			player_descr* pc = make_player();
			FILE* example = fopen("story.txt", "r");
			dungeon* d = load_dungeon(example, pc);
			d->pc_coor = d->lvls[0]->upstairs;
			d->pc = pc;
			fclose(example);
			cycle(d);
			break;
		}
		case 'g': {
			player_descr* pc = make_player();
			FILE* example = fopen("descriptions.txt", "r");
			dungeon* d = create_dungeon(example);
			d->pc_coor = d->lvls[0]->upstairs;
			d->pc = pc;
			fclose(example);
			cycle(d);
			break;
		}
		case 'q':
			message("Goodbye!");
			getch();
			break;
	}
	
}




/* Starts the game, really. Inits all the crap.
	Reads dungeon from file. Creates a PC. Puts PC into the dungeon.
					 calls (at least):
							init_msgs
							init_main
							cycle */
void init() {
	srand(time(0));
	initscr();
	init_msg_display();
	init_main_display();
	init_status_bar();
	keypad(stdscr, true);
	keypad(main_display,true);
	keypad(msg_display, true);
	noecho();
	cbreak();
	curs_set(0);
	start_screen();
}

void update_status_bar(dungeon* d) {
	wclear(status_bar);
	wprintw(status_bar, "HP: %d Level: %d", d->pc->hp, d->curr_lvl);
	wrefresh(status_bar);
}

/* Tries moving the pc into the desired location on the current level.
	If there is floor in new_loc - just moves the character.
	If there is rock in the new_loc - messages (YOU BUMP INTO A ROCK)
	If there is a monster in the new_loc - attacks! */

void try_move(dungeon* d, point new_loc){
	switch(d->lvls[d->curr_lvl]->map[new_loc.x][new_loc.y]) {
			case '<':
			case '>':
			case '.':
			if (d->lvls[d->curr_lvl]->mon_map[new_loc.x][new_loc.y] == NULL){
				d->pc_coor = new_loc;
			} else {
				message("You engaged a monster!");
				monster_hit(d->lvls[d->curr_lvl]->mon_map[new_loc.x][new_loc.y], d->pc);
				if (d->lvls[d->curr_lvl]->mon_map[new_loc.x][new_loc.y]->hp < 1) {
					destroy_monster(d->lvls[d->curr_lvl]->mon_map[new_loc.x][new_loc.y]);
					d->lvls[d->curr_lvl]->mon_map[new_loc.x][new_loc.y] = NULL; 
				}
			}
			break;
		case '#':
			message("You bump into a rock!");
			break;
	}
}
						
/* Tries moving the character up a level. Only works if the character is now on the upstairs.
	Puts the character on the above level on the downstairs*/
void try_up(dungeon* d) {
	if (d->lvls[d->curr_lvl]->upstairs.x == d->pc_coor.x &&
	    d->lvls[d->curr_lvl]->upstairs.y == d->pc_coor.y){
		if (d->curr_lvl == 0){
			leave(d, "You leave this dungeon... forever.");
		}else{
			d->curr_lvl--;	//-lvl == 1 Dungeon lvl up! When == -1(ground) exit
			d->pc_coor.x = d->lvls[d->curr_lvl]->downstairs.x;
			d->pc_coor.y = d->lvls[d->curr_lvl]->downstairs.y;
			message("You went upstairs");
		}
	}else{
		message("You see no stairs leading up...");
	}
}
/* Tries moving the character down a level. Works same as try_up. */
void try_down(dungeon* d){
	if (d->lvls[d->curr_lvl]->downstairs.x == d->pc_coor.x &&
	    d->lvls[d->curr_lvl]->downstairs.y == d->pc_coor.y){
		if (d->curr_lvl < d->lvl_num){
			d->curr_lvl++;	//-lvl == 1 Dungeon lvl up! When == -1(ground) exit
			d->pc_coor.x = d->lvls[d->curr_lvl]->upstairs.x;
			d->pc_coor.y = d->lvls[d->curr_lvl]->upstairs.y;
			message("You went downstairs");
		} else {
			message("Cannot go down! Too deep...");
		}
	} else {
		message("You see no stairs leading down...");
	}
}


void redraw(dungeon* d) {
	wclear(main_display);
	int f, g;
	char** map = d->lvls[d->curr_lvl]->map;
	monster*** mon_map = d->lvls[d->curr_lvl]->mon_map;
	point pc_coor = d->pc_coor;
	wmove(main_display, 0, 0);
	for(f = 0; f < DISPLAYY; f++) {		
		for(g = 0; g < DISPLAYX; g++) {
				int mapx = g + pc_coor.x - DISPLAYX/2;
				int mapy = f + pc_coor.y - DISPLAYY/2;
				if(mapx < 0 || mapy < 0 || mapx >= d->lvls[d->curr_lvl]->mapx || mapy >= d->lvls[d->curr_lvl]->mapy) {
					wmove(main_display, f, g);
					wprintw(main_display," ");
				} else if(mon_map[mapx][mapy] != NULL) {
					wmove(main_display, f, g);
					wprintw(main_display, "%c", d->lvls[d->curr_lvl]->mon_map[mapx][mapy]->descr->pic);	
				} else { 
					wmove(main_display, f, g);
					wprintw(main_display, "%c", map[mapx][mapy]);
				}
		}
	}
	wmove(main_display, DISPLAYY/2, DISPLAYX/2);
	waddch(main_display, '@');
	wrefresh(main_display);
}

	
/* Main control cycle. The idea is that we wait for player input, and react accordingly, THEN (and only then) redraw the map once again. 
The idea is simmilar to that of the controlled HELLO_WORLD. 
CONTROLS: ARROW KEYS - movement
		  > -- go down a level
		  < -- go up a level */
void cycle(dungeon* d) {
	int k;
	point next_move;
	while(d->pc){
		redraw(d);
		update_status_bar(d);
		k = getch();
		wclear(msg_display);
		wrefresh(msg_display);
		switch(k){
			case KEY_LEFT:
				next_move.x = (d->pc_coor.x)-1;
				next_move.y = (d->pc_coor.y); 
				try_move(d, next_move);
				break;
			case KEY_RIGHT:
				next_move.x = (d->pc_coor.x)+1;
				next_move.y = (d->pc_coor.y);
				try_move(d, next_move);
				break;
			case KEY_UP:
				next_move.y = (d->pc_coor.y)-1;
				next_move.x = (d->pc_coor.x);
				try_move(d, next_move);
				break;
			case KEY_DOWN:
				next_move.y = (d->pc_coor.y)+1;
				next_move.x = (d->pc_coor.x);
				try_move(d, next_move);
				break;
			case '<':
				try_up(d);
				break;
			case '>':
				try_down(d);
				break;
			case 'Q':
				leave(d, "Fare thee well!");
				return;
			case 'S': {
				FILE* sav = fopen("save.txt", "w");
				save_full(sav, d);
				fclose(sav);
				leave(d, "Game saved! Fare thee well!");
				return;
			}	
		}
		monster_turn(d);
		if (d->pc->hp <= 0) {
			leave(d, "You die...");
			return;
		}
	}
}


void monster_turn(dungeon* d) {	
	point pc_coor = d->pc_coor;
	player_descr * target = d->pc;
	monster*** mon_map = d->lvls[d->curr_lvl]->mon_map;
	if(mon_map[pc_coor.x+1][pc_coor.y] != NULL) {
		monster * attack = d->lvls[d->curr_lvl]->mon_map[pc_coor.x+1][pc_coor.y]; 
		player_hit(target, attack);
	}
	else if(mon_map[pc_coor.x][pc_coor.y+1] != NULL)  {
		monster * attack = d->lvls[d->curr_lvl]->mon_map[pc_coor.x][pc_coor.y+1]; 
		player_hit(target, attack);

	}
	else if(mon_map[pc_coor.x-1][pc_coor.y] != NULL) {
		monster * attack = d->lvls[d->curr_lvl]->mon_map[pc_coor.x-1][pc_coor.y]; 
		player_hit(target, attack);

	}
	else if(mon_map[pc_coor.x][pc_coor.y-1] != NULL) {
		monster * attack = d->lvls[d->curr_lvl]->mon_map[pc_coor.x][pc_coor.y-1]; 
		player_hit(target, attack);

	}
}
