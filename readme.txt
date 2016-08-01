This is the readme file for the workshop #4's project from ssyp 2016.


-----Installation and Laucnh-----
The game has to be run from terminal, and the terminal screen size should be no less than 52x40.
This program requires ncurses to compile. It can be found in the package 'libncurses-dev'.
To compile, run gcc on all the .c files in this directory and use -lncurses flag, e.g.:
	gcc *.c -o game -lncurses

To run, use ./game, or whatever name you gave to the game while compiling.

-----Gameplay-----
First, you will have to choose from the given options to start a story-mode game, a random dungeon mode game, or load a saved game.
Game symbols:
'@' - the character
'#' - Stone wall
'.' - Floor
'>' - Downwards stairs
'<' - Upwards stairs
Other symbols - monsters.
The character can only walk on the floor, and never through walls. If a monster is adjacent to the character, it will attack him.
If the character tries moving into a tile with a monster, he will attack the monster.
The character can use the stairs to go up and down levels.
Going up from the first level will result in a game over, and the character won't be able to bring himself to go down from the lowest level.


-----Controls-----
Arrow keys - movement or attacking
> and < keys - moving up and down levels
S - save the game and exit
Q - exit the game without saving

-----Package Contents-----
Source code and header files:
dungeon_gen 	-- generates dungeons and new levels, spawns monsters.
misc 		-- contains a couple of miscellaneous structures and functions.
dungeon 	-- handles dungeon storage and loading from files.
game 		-- handles main gameplay and display.
player 		-- contains player descriptions and handling it.
monster 	-- contains monster descriptions and handles monster and player attacks.
test.c		-- calls the functions and starts the game (feel free to rename).

Text files:
story.txt		-- default 'story mode' dungeon.
save.txt		-- saved game.
descriptions.txt	-- monster descriptions used for generating random dungeons.
format.txt 		-- explains the format used for these files.


-----Known Bugs and issues——
Leaving the first level may result in a segmentation fault error.
The monster AI is non-existant






