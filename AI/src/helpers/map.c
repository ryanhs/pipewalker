#include <malloc.h>
#include <string.h>

#include "map.h"


map_item map_item_put(const char *key, map_item *prev){
	map_item item;
	
	strcpy(item.key, key);
	if(prev != NULL){
		prev->next = &item;
		item.prev = prev;
	}
		
	return item;
}
