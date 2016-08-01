#include "dungeon_gen.h"
/*typedef struct {
	int x,y;
} point;*/
list* createlist(room* data){
	list* head = malloc(sizeof(list));
	head->data = data;
	head->next = NULL;
	return head;
} 
list* addtohead(room* data, list* old){
	list* head = malloc(sizeof(list));
	head->data = data;
	head->next = old;
	return head;
}
list* addtoend(room* data, list* old){
	list* curr = old;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = createlist(data);
	return old;
} 
list* getel(int n, list* head){
	int i=0;
	list* curr=head;
	if (curr == NULL) 
		return NULL;
	while (curr != NULL){
		if (i==n) 
			return curr;
		i++;
		curr = curr->next;
	}
	return NULL;
}
/* Let's focus purely on map generation first. When we are done with that, let's continue on to generationg monster maps as well */

// Chunk is square ABCD; A - right down point
//   Y	
//   |
//   |
//   |
// B |_________________________________  C
//   |                                | 
//   |	                              |
//   |                                |   
//   |                                |
//   |                                | D
// A |--------------------------------|------------------> X 
//
point get_chunkA(level_inner* l, chunk c) {
	point p;
	p.y = l->chunksize * c.y;
	p.x = l->chunksize * c.x;
	return p;
}
point get_chunkB(level_inner* l, chunk c) {
	point p;
	p.x = l->chunksize * c.x;
	p.y = l->chunksize * (c.y+1);
	return p;
}
point get_chunkC(level_inner* l, chunk c) {
	point p;
	p.x = l->chunksize * (c.x+1);
	p.y = l->chunksize * (c.y+1);
	return p;
}
point get_chunkD(level_inner* l, chunk c) {
	point p;
	p.x = l->chunksize * (c.x+1);
	p.y = l->chunksize * c.y;
	return p;
}
// allocates a room in the given x, y coordinates. Builds it. Digs it (changes the map)
void dig_room(level_inner* l, int x, int y) {
	room* r = malloc(sizeof(room));
	chunk p;
	p.y = y;
	p.x = x;
	point chunk_tl = get_chunkA(l,p); 
	point chunk_br = get_chunkC(l,p);
	r->tl.x = rand_int_range(chunk_tl.x + 1, chunk_br.x - l->minroom - 1);
	r->tl.y = rand_int_range(chunk_tl.y + 1, chunk_br.y - l->minroom - 1);
	r->br.x = rand_int_range(r->tl.x + l->minroom, chunk_br.x - 1);
	r->br.y = rand_int_range(r->tl.y + l->minroom, chunk_br.y - 1);
	if (l->rlist != NULL) {
		l->rlist = addtoend(r,l->rlist);
		l->rlist_len++; 
	}
	else {
		l->rlist = createlist(r);
		l->rlist_len++;
	}
	l->rooms[x][y] = r;
	int i, j;
	for (i = r->tl.x + 1; i < r->br.x; i++) {
	 	for(j = r->tl.y + 1; j < r->br.y; j++) {
			l->map[i][j] = '.';	
		}
	}
}
int min(int a, int b) {
	if (a<b) 
		return a;
	else
		return b;
}
int max(int a, int b) {
	if (a>b) 
		return a;
	else
		return b;
}
// connects two rooms with a tunnel (1-tile width). Assumes left is directly left from right.
void connect_lr(level_inner* l, chunk left, chunk right) {
	int y, i; // leftx -- tunnel start coordinates from the left room, rightx... and y - the y coordinate of the tunnel
	room* room_left = l->rooms[left.x][left.y];
	room* room_right = l->rooms[right.x][right.y];
	int miny = max(room_left->tl.y, room_right->tl.y);
	int maxy = min(room_left->br.y, room_right->br.y);
	if (maxy - 1 > miny + 1)
		y = rand_int_range(miny + 1, maxy - 1);
	else
		y = miny + 1;
	for (i = room_left->br.x; i <= room_right->tl.x; i++) {
		l->map[i][y] = '.';	
	}
}
void connect_tb(level_inner* l, point top, point bot) {
	int x, i; // x -- coordinate of the connection point
	room* room_top = l->rooms[top.x][top.y];
	room* room_bot = l->rooms[bot.x][bot.y];
	int minx = max(room_top->tl.x, room_bot->tl.x);
	int maxx = min(room_top->br.x, room_bot->br.x);
	if (maxx - 1 > minx + 1) 
		x = rand_int_range(minx + 1, maxx - 1);
	else
		x = minx;
	for (i = room_top->br.y; i <= room_bot->tl.y; i++) {
		l->map[x][i] = '.';	
	}
}
// 0 - North, 1 - East, 2 - South, 3 - West
chunk get_neighbour(chunk c, int dir) {
	switch (dir) {
		case 0:
			c.y--;
			break;
		case 1:
			c.x++;
			break;
		case 2:
			c.y++;
			break;
		case 3:
			c.x--;
			break;
		default:
			c.x = -1;
			c.y = -1;
	}
	return c;
}

int get_chunkx_room(level_inner* l, room* r) {
	return r->tl.x / l->chunksize;
}
int get_chunky_room(level_inner* l, room* r) {
	return r->tl.y / l->chunksize;
}

chunk get_chunk_room(level_inner* l, room* r) {
	chunk c;
	c.x = r->tl.x / l->chunksize;
	c.y = r->tl.y / l->chunksize;
	return c;
}

void make_neighbour(level_inner* l, chunk curr, int dir) {
	chunk next = curr;
	switch (dir) {
		case 0:
			next.y--;
			dig_room(l, next.x, next.y);
			connect_tb(l, next, curr);
			break;
		case 1:
			next.x++;
			dig_room(l, next.x, next.y);
			connect_lr(l, curr, next);
			break;
		case 2:
			next.y++;
			dig_room(l, next.x, next.y);
			connect_tb(l, curr, next);
			break;
		case 3:
			next.x--;
			dig_room(l, next.x, next.y);
			connect_lr(l, next, curr);
			break;	
	}

}
// builds an inner level using all this crap.
// idea stays same as yesterday.
level_inner* build_level_inner(int mapX, int mapY, int chunksize, int minroom, int room_number) {
	level_inner* l;
	int i, j, y, x, r;
	// Map Initialization
	l = malloc(sizeof(level_inner));
	l->map = malloc(sizeof(char*)*mapX);
	for (i = 0; i < mapX; i++) {
		l->map[i] = malloc(sizeof(char)*mapY);
	}
	for (i = 0; i < mapX; i++) { //fills map squares with '#'
		for (j=0; j<mapY; j++) {
			l->map[i][j] = '#';
		}
	}
 
	chunk p1, p2;
	l->chunkX = mapX / chunksize;
	l->chunkY = mapY / chunksize;
	l->chunksize = chunksize;
	// Room-map initialization
	l->rooms = malloc(sizeof(room**)*l->chunkX);
	for (i=0; i < l->chunkX; i++) {
		l->rooms[i] = malloc(sizeof(room*)*l->chunkY);
	}
	for (i = 0; i < (l->chunkX); i++) { // fiils array of rooms with NULLs
		for (j = 0; j <= l->chunkY; j++) {
			l->rooms[i][j] = NULL;
		}
	}
	// End Initialization
	l->minroom = minroom;
	l->mapX = mapX;
	l->mapY = mapY;
	l->rlist_len = 0;
	l->rlist = NULL;
	
	// Start generating rooms
	x = rand_int_range(0, l->chunkX-1); // Choosing the first chunk
	y = rand_int_range(0, l->chunkY-1);
	chunk current, next;
	current.x = x;
	current.y = y;
	dig_room(l, current.x, current.y);

	while (l->rlist_len < room_number) { 
		int start_num = rand() % l->rlist_len;
		list* tmp = getel(start_num, l->rlist);
		// cycle through directions
		int attempts = 0;
		int dir;
		int found = 0;
		while (!found) {
			current = get_chunk_room(l, tmp->data);
			dir = rand_int_range(0, 3);
			if (dir < 3)
				dir++;
			else
				dir = 0;
			for (attempts = 0; attempts < 4; attempts++) {
				chunk next = get_neighbour(current, dir);
				if (next.x >= 0 && next.y >= 0 && next.x < l->chunkX && next.y < l->chunkY && l->rooms[next.x][next.y] == NULL) {
					found = 1;
					break;
				}
			}
			if (tmp->next)
				tmp = tmp->next;
			else
				tmp = l->rlist;
		}
		make_neighbour(l, current, dir);	
	}	
	return l;
}

void print_level(level_inner* l) {
	int x, y;
	for (y = 0; y < l->mapY; y++) {
		for(x = 0; x < l->mapX; x++) {
			printf("%c", l->map[x][y]);
		}
		printf("\n");
	}

}

point rand_in_room(room* r) {
	point p;
	p.x = rand_int_range(r->tl.x+1, r->br.x-1);
	p.y = rand_int_range(r->tl.y+1, r->br.y-1);
	return p;
}

void add_stairs(level* l, level_inner* levin) {
	list* tmp = getel(rand()%levin->rlist_len, levin->rlist);
	point up, down;
	up = rand_in_room(tmp->data);
	l->upstairs = up;
	l->map[up.x][up.y] = '<';

	//choose the room for the next thing
	tmp = getel(rand()%levin->rlist_len, levin->rlist);
	down = rand_in_room(tmp->data);
	if (up.x == down.x && up.y == down.y) {
		if (tmp->next)
			tmp = tmp->next;
		else
			tmp = levin->rlist;
		
		down = rand_in_room(tmp->data);
	}
	l->downstairs = down;
	l->map[down.x][down.y] = '>';

}

void spawn_monster(level_inner* lvl, monster_descr** descr, monster*** mon_map, int descr_num, int monster_num){
	int  x, y;
	point target;
	room* r;
	list* tmp;
	for (; monster_num > 0; monster_num--){		
		tmp = getel(rand()%lvl->rlist_len, lvl->rlist);
		r = tmp->data;

		int i;
		i = 0;
		do {
			target = rand_in_room(r);
			i++;
		} while (i < 5 && mon_map[target.x][target.y] != NULL);
		if (mon_map[target.x][target.y] != NULL)
			destroy_monster(mon_map[target.x][target.y]);
		mon_map[target.x][target.y] = make_monster(descr[rand()%descr_num]);
	}
}

level* build_level(level_inner* levin, monster_descr** descr, int descr_num) {
	int i;	
	level* l = malloc(sizeof(level));
	l->map = levin->map;
	l->mapx = levin->mapX;
	l->mapy = levin->mapY;
	l->mon_map = malloc(sizeof(monster**)*l->mapx);
	for (i = 0; i < l->mapx; i++)
		l->mon_map[i] = calloc(l->mapy, sizeof(monster*));
	add_stairs(l, levin);
	spawn_monster(levin, descr, l->mon_map, descr_num, rand_int_range(3, 6));
	return l;
}

dungeon* build_dungeon(monster_descr** descrs, int descr_num, int depth) {
	int i, j, k;
	dungeon* d = malloc(sizeof(dungeon));
	d->lvls = malloc(sizeof(level*)*depth);
	d->descriptions = descrs;
	d->descr_num = descr_num;
	for (i = 0; i < depth; i++) {
		level_inner* levin = build_level_inner(100, 100, 10, 7, rand_int_range(4, 8));
		d->lvls[i] = build_level(levin, descrs, descr_num);
		for (j = 0; j < levin->chunkX; j++) {
			for (k = 0; k < levin->chunkY; k++) {
				if (levin->rooms[j][k])
					free(levin->rooms[j][k]);
			}
			free(levin->rooms[j]);
		}
		free(levin->rooms);
		free(levin);
	}
	d->lvl_num = depth;
	return d;
}

dungeon* create_dungeon(FILE* descr_file) {
	// read all descriptions!
	int descr_num, i;
	fscanf(descr_file, "%d\n", &descr_num);
	monster_descr** descrs = malloc(sizeof(monster_descr*)*descr_num);
	for (i = 0; i < descr_num; i++) {
		descrs[i] = read_descr(descr_file);
	}
	dungeon* d = build_dungeon(descrs, descr_num, 5);
	return d;
}
