#include <malloc.h>
#include <string.h>

#include "tile.h"
#include "../../../lib/safeTo/safeTo.h"


char *tile_type_string2JSON(char *output, tile_struct *tile){
	sprintf(output, "%s",
		(
			tile->type == TILE_BLANK ? "blank" : 
			(tile->type == TILE_PIPE_1 ? "pipe-1" : 
			(tile->type == TILE_PIPE_2 ? "pipe-2" : 
			(tile->type == TILE_PIPE_3 ? "pipe-3" : 
			(tile->type == TILE_SOURCE ? "source" : 
			(tile->type == TILE_CLIENT ? "client" : "-")))))
		)
	);
	
	return output;
}

char *tile_direction_string2JSON(char *output, tile_struct *tile){
	sprintf(output, "%s",
		(
			tile->direction == TILE_UP ? "up" : 
			(tile->direction == TILE_RIGHT ? "right" : 
			(tile->direction == TILE_DOWN ? "down" : 
			(tile->direction == TILE_LEFT ? "left" : "-")))
		)
	);
	
	return output;
}

char *tile_active_string(char *output, tile_struct *tile){
	sprintf(output, "%s",
		(
			tile->active == TILE_ACTIVE ? "active" : 
			(tile->active == TILE_INACTIVE ? "inActive" : "-")
		)
	);
	
	return output;
}

char *tile_type_string(char *output, tile_struct *tile){ return tile_type_string2JSON(output, tile); }
char *tile_direction_string(char *output, tile_struct *tile){ return tile_direction_string2JSON(output, tile); }


char *tile_to_string(char *output, tile_struct *tile){
	char tmp[64];
	
	memset(tmp, 0, 64);
	sprintf(output, "%s/", tile_type_string2JSON(tmp, tile));
	
	memset(tmp, 0, 64);
	sprintf(output, "%s%s/", output, tile_active_string(tmp, tile));
	
	memset(tmp, 0, 64);
	sprintf(output, "%s%s", output, tile_type_string2JSON(tmp, tile));
	
	return output;
}


short int *tile_active_branches(short int tile_type, short int tile_direction){
	short int hasUp = 0;
	short int hasRight = 0;
	short int hasDown = 0;
	short int hasLeft = 0;
	short int i = 0;
	
	if(tile_type == TILE_SOURCE || tile_type == TILE_CLIENT){
		if(tile_direction == TILE_UP){
			hasUp = 1; i++;
		}
		if(tile_direction == TILE_RIGHT){
			hasRight = 1; i++;
		}
		if(tile_direction == TILE_DOWN){
			hasDown = 1; i++;
		}
		if(tile_direction == TILE_LEFT){
			hasLeft = 1; i++;
		}
	}
	else if(tile_type == TILE_PIPE_1){
		if(tile_direction == TILE_UP){
			hasUp = 1; i++;
			hasDown = 1; i++;
		}
		if(tile_direction == TILE_RIGHT){
			hasLeft = 1; i++;
			hasRight = 1; i++;
		}
		if(tile_direction == TILE_DOWN){
			hasUp = 1; i++;
			hasDown = 1; i++;
		}
		if(tile_direction == TILE_LEFT){
			hasLeft = 1; i++;
			hasRight = 1; i++;
		}
	}
	else if(tile_type == TILE_PIPE_2){
		if(tile_direction == TILE_UP){
			hasUp = 1; i++;
			hasRight = 1; i++;
		}
		if(tile_direction == TILE_RIGHT){
			hasRight = 1; i++;
			hasDown = 1; i++;
		}
		if(tile_direction == TILE_DOWN){
			hasDown = 1; i++;
			hasLeft = 1; i++;
		}
		if(tile_direction == TILE_LEFT){
			hasLeft = 1; i++;
			hasUp = 1; i++;
		}
	}
	else if(tile_type == TILE_PIPE_3){
		if(tile_direction == TILE_UP){
			hasUp = 1; i++;
			hasRight = 1; i++;
			hasDown = 1; i++;
		}
		if(tile_direction == TILE_RIGHT){
			hasRight = 1; i++;
			hasDown = 1; i++;
			hasLeft = 1; i++;
		}
		if(tile_direction == TILE_DOWN){
			hasUp = 1; i++;
			hasDown = 1; i++;
			hasLeft = 1; i++;
		}
		if(tile_direction == TILE_LEFT){
			hasUp = 1; i++;
			hasRight = 1; i++;
			hasLeft = 1; i++;
		}
	}
	
	if(i == 0) return NULL;
	
	short int *arr = calloc(i, sizeof(short int));
	short int *tmp = arr;
	i = 0;
	
	if(hasLeft){ *(tmp + i) = TILE_LEFT; i++; }
	if(hasUp){ *(tmp + i) = TILE_UP; i++; }
	if(hasRight){ *(tmp + i) = TILE_RIGHT; i++; }
	if(hasDown){ *(tmp + i) = TILE_DOWN; i++; }
	
	return arr;
}

short int tile_has_direction(short int tile_type, short int tile_direction, short int comparator){
	short int *branches = tile_active_branches(tile_type, tile_direction);
	short int *tmpFree = branches;
	
	while(*branches){
		if(*branches == comparator) break;
		branches++;
	}
	
	short int tmp = *branches;
	free(tmpFree);
	return tmp == comparator ? 1 : 0;
}
short int tile_has_up(short int tile_type, short int tile_direction){
	return tile_has_direction(tile_type, tile_direction, TILE_UP);
}
short int tile_has_right(short int tile_type, short int tile_direction){
	return tile_has_direction(tile_type, tile_direction, TILE_RIGHT);
}
short int tile_has_down(short int tile_type, short int tile_direction){
	return tile_has_direction(tile_type, tile_direction, TILE_DOWN);
}
short int tile_has_left(short int tile_type, short int tile_direction){
	return tile_has_direction(tile_type, tile_direction, TILE_LEFT);
}
