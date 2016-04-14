#ifndef _MAP_H
#define _MAP_H

typedef struct{
	char key[255];
	
	void *valueVoid;
	int valueInt;
	double valueDouble;
	char valueString[255];
	
	void *next;
	void *prev;
} map_item;


map_item map_item_put(const char *key, map_item *prev);
// #define map_item_add(key, prev)		 map_item_put(key, prev);

#endif
