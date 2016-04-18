#ifndef _MAP_H
#define _MAP_H

typedef struct{
	char key[5000];
	
	void *valueVoid1;
	int valueInt1;
	double valueDouble1;
	char valueString1[255];
	
	void *valueVoid2;
	int valueInt2;
	double valueDouble2;
	char valueString2[255];
	
	void *valueVoid3;
	int valueInt3;
	double valueDouble3;
	char valueString3[255];
	
	void *next;
	void *prev;
} map_item;


map_item *map_item_add(map_item *start, const char *key);
short int map_item_remove(map_item *start, const char *key);
short int map_item_delete(map_item *start);

int map_size(map_item *);
void map_reset(map_item **);
void map_last(map_item **);
void map_walk(map_item *start, void (*testWalk_callback)(map_item *));
map_item *map_item_pop(map_item *start);

map_item *map_item_get(map_item *start, const char *key);
short int map_item_exists(map_item *start, const char *key);

#endif
