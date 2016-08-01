#pragma once
/* Handles display, controls and game initialization. */
#include "dungeon.h"
#include "dungeon_gen.h"
#include <ncurses.h>

/* Initializes all the required stuff, ncurses, colors. Reads dungeon from file using load_dungeon. Generates a player_descr. Starts the cycle. */
void init();

/* The main cycle of movement, attacks and redraw. Simmilar to controlled HELLO WORLD */
void cycle(dungeon* d);

/* Redraws the screen*/
void redraw(dungeon* d);

/* Prints a string below the main screen. The string is removed on the nextmessage. HINT: use global variable WINDOW* msg_win or something like that.*/


void init_msg_display();

/* Creates the main display at the right position. */
void init_main_display();

/* Refreshes the message bar, prints the new message there. */
void message(char* string);

/* Ends the game, frees the memory, exits curses mode. */
void leave(dungeon* d, char* msg);
