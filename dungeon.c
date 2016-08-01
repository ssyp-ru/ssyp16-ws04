#include "dungeon.h"

/* OLD: Reads a single map*/
/*char** map_read(FILE* input){
	int i, j, maxx, maxy;
	fscanf(input,"%d %d", maxx, maxy);
	char** map = malloc(sizeof(char*)*maxx);
	for(i = 0;i < maxx;i++){
		map[i] = malloc(sizeof(char)*maxy);
	}	 
	fscanf(input,"\n");
	for(i = 0;i < maxx;i++){
		for(j = 0;j < maxy;j++){
			fscanf(input,"%c", &map[i][j]);
		}
		fscanf(input,"\n");
	}
	return map;
}*/

/* NEW: reads a single level, allocates the memory and all that jazz */
level* level_read(FILE* input, monster_descr** descriptions, int descr_num){
	char m;
	int i, j, maxx, maxy,  k;
	fscanf(input,"%d %d",&maxx, &maxy);
	fscanf(input,"\n");
	level* dung = malloc(sizeof(level));
	dung->mon_map = malloc(sizeof(monster**)*maxx);
	for(i = 0;i < maxx; i++){
		dung->mon_map[i] = calloc(maxy, sizeof(monster*));
	}
	dung->map = malloc(sizeof(char*)*maxx);
	for(i = 0;i < maxx; i++){
		dung->map[i] = malloc(sizeof(char)*maxy);
	}
	dung->mapx = maxx;
	dung->mapy = maxy;
	for(i = 0; i < maxy; i++){
		for(j = 0; j < maxx; j++){
			fscanf(input,"%c", &m);
			if(m != '.' && m != '#' && m != '/' && m != '>' && m != '<'){
				k = 0;
				while(m != descriptions[k]->pic){
					k++;
				}
				dung->mon_map[j][i] = make_monster(descriptions[k]);
				dung->map[j][i] = '.';
			}
			else {
				if (m == '>') {
					dung->downstairs.x = j;
					dung->downstairs.y = i;
				} else if (m == '<') {
					dung->upstairs.x = j;
					dung->upstairs.y = i;
				}
				dung->map[j][i] = m;
			}
		}
		fscanf(input,"\n");
	}
	return dung;

}

/* Destroys the level, frees the memory taken up by it and all it's contents.*/
void destroy_level(level* l){
	int i;
	for(i = 0;i < l->mapx;i++){
		free(l->map[i]);
	}
	free(l->map);
	for(i = 0;i < l->mapx;i++){
		free(l->mon_map[i]);
	}
	free(l->mon_map);
	free(l);
}

/* Reads a single monster description */
monster_descr* read_descr(FILE* input){
	monster_descr* monster = malloc(sizeof(monster_descr));
	monster->name = malloc(sizeof(char)*20);
	fscanf(input,"%c %s %d %d %d %d %d", &monster->pic, monster->name, &monster->max_hp, 
			&monster->dodge, &monster->max_roll, &monster->max_dmg, &monster->min_dmg);
	fscanf(input,"\n");	
	return monster;
}

void destroy_dungeon(dungeon* d) {
	int i;
	for(i = 0; i < d->descr_num; i++) {
		free(d->descriptions[i]->name);
		free(d->descriptions[i]);
	}
	free(d->descriptions);
	for(i = 0; i < d->lvl_num; i++) {
		destroy_level(d->lvls[i]);
	}
	free(d->lvls);
	free(d);
}
dungeon* load_dungeon(FILE* input, player_descr* pc) {	
	int i, mon, lvl;
	fscanf(input,"DUNGEON\n");
	fscanf(input,"DESCRIPTIONS %d\n", &mon);
	dungeon* dung = malloc(sizeof(dungeon));
	dung->descriptions = malloc(sizeof(monster_descr*)*mon);
	for(i = 0;i < mon;i++){
		dung->descriptions[i] = read_descr(input);
	}
	fscanf(input,"\n");
	fscanf(input,"LEVELS %d", &lvl);
	dung->lvls = malloc(sizeof(level*)*lvl);
	for(i = 0;i < lvl;i++){
		dung->lvls[i] = level_read(input, dung->descriptions, lvl);
	}
	dung->descr_num = mon;
	dung->lvl_num = lvl;
	dung->pc = pc;
	return dung;
}
dungeon* load_full(FILE* input){
	dungeon* d = load_dungeon(input, NULL);
	player_descr* pc = malloc(sizeof(player_descr));
	fscanf(input,"PLAYER\n"); 
	fscanf(input, "%d %d %d %d %d %d %d", &pc->max_hp, &pc->hp, &pc->dodge, &pc->max_roll, &pc->max_dmg, &pc->min_dmg, &d->curr_lvl);
	fscanf(input, "%d %d", &d->pc_coor.x, &d->pc_coor.y);
	d->pc = pc;
	return d;
}


void write_level(FILE* save,level* l){
	char m;
	int i, j, mon, dun, k;
	fprintf(save,"%d %d\n",l->mapx, l->mapy);
	for(i = 0;i < l->mapy;i++){
		for(j = 0;j < l->mapx;j++){
			if(l->mon_map[j][i] != NULL){
				fprintf(save,"%c",l->mon_map[j][i]->descr->pic);
			}
			else{
				fprintf(save,"%c",l->map[j][i]);
			}
		}
		fprintf(save,"\n");
	}
}

void write_descr(FILE* save, monster_descr* m){
	fprintf(save,"%c %s %d %d %d %d %d", m->pic, m->name, m->max_hp, m->dodge, m->max_roll, m->max_dmg, m->min_dmg);
	fprintf(save,"\n");	
}
void save_dungeon(FILE* save, dungeon* d){
	int i;
	fprintf(save,"DUNGEON\n");
	fprintf(save,"DESCRIPTIONS %d\n", d->descr_num);
	for(i = 0;i < d->descr_num;i++){
		write_descr(save, d->descriptions[i]);
	}
	fprintf(save,"\n");
	fprintf(save,"LEVELS %d\n",d->lvl_num);
for(i = 0;i < d->lvl_num;i++){
		write_level(save, d->lvls[i]);
		fprintf(save,"\n");
	}
}

void save_full(FILE* save, dungeon* d){
	save_dungeon(save, d);
	player_descr* pc = d->pc;
	fprintf(save,"PLAYER\n");
	fprintf(save,"%d %d %d %d %d %d %d\n",pc->max_hp, pc->hp, pc->dodge, pc->max_roll, pc->max_dmg, pc->min_dmg, d->curr_lvl);
	fprintf(save, "%d %d", d->pc_coor.x, d->pc_coor.y);
}




