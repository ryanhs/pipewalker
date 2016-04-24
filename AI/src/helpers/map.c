#include <malloc.h>
#include <string.h>

#include "map.h"


map_item *map_item_add(map_item *start, const char *key){
	map_item *item = malloc(sizeof(map_item));
	
	strcpy(item->key, key);
	item->next = NULL;
	item->prev = NULL;
	
	if(start != NULL){
		while(start->next != NULL){
			if(start->next == NULL) break;	
		start = start->next; }
		
		start->next = item;
		item->prev = start;
	}
		
	return item;
}

short int map_item_delete(map_item *start){
	if(start == NULL) return 0;
	
	map_reset(&start);
	
	int i = 0;
	map_item *tmp;
	while(start != NULL){
		i++;
		tmp = start;
		start = start->next;
		free(tmp);
	}
	
	//~ printf("\tdeleted: %d\n", i);
	
	return 1;
}

map_item *map_item_get(map_item *start, const char *key){
	map_reset(&start);
	
	while(start != NULL){
		if(strcmp(start->key, key) == 0) break;	
	start = start->next; }
	
	return start;
}

int map_size(map_item *start){
	map_reset(&start);
	
	int i = 0;
	while(start != NULL){
		i++;
	start = start->next; }
	
	return i;
}

short int map_item_exists(map_item *start, const char *key){
	return map_item_get(start, key) != NULL;
}

short int map_item_remove(map_item *start, const char *key){
	map_item *current = map_item_get(start, key);
	if(current == NULL) return 0;
	
	map_item *prev = current->prev;
	map_item *next = current->next;
	
	if(prev != NULL) prev->next = current->next;
	if(next != NULL) next->prev = current->prev;
	free(current);
	return 1;
}

void map_reset(map_item **start_pointer){
	map_item *start = *start_pointer;
	
	// back to first to search whole item
	if(start->prev != NULL){
		while(start->prev != NULL){ start = start->prev; }
	}
	
	*start_pointer = start;
}

void map_last(map_item **start_pointer){
	map_item *start = *start_pointer;
	
	// back to first to search whole item
	if(start->next != NULL){
		while(start->next != NULL){ start = start->next; }
	}
	
	*start_pointer = start;
}

void map_walk(map_item *start, void (*testWalk_callback)(map_item *)){
	map_reset(&start);
	
	while(start != NULL){
		testWalk_callback(start); 
	start = start->next; }
}

map_item *map_item_pop(map_item *start){
	map_last(&start);
	
	map_item *prev = start->prev;
	if(prev != NULL) prev->next = NULL;
	
	start->prev = NULL;
	
	return start;
}
