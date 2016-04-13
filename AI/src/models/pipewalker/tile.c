#include <malloc.h>
#include <string.h>

#include "tile.h"
#include "../../../lib/safeTo/safeTo.h"

char *tile_to_string(char *output, tile_struct *tile){
	sprintf(output, "%s/%s/%s",
		(
			tile->type == TILE_BLANK ? "Blank" : 
			(tile->type == TILE_PIPE_1 ? "Pipe-1" : 
			(tile->type == TILE_PIPE_2 ? "Pipe-2" : 
			(tile->type == TILE_PIPE_3 ? "Pipe-3" : 
			(tile->type == TILE_SOURCE ? "Source" : 
			(tile->type == TILE_CLIENT ? "Client" : "-")))))
			
		),
		(
			tile->active == TILE_ACTIVE ? "Active" : 
			(tile->active == TILE_INACTIVE ? "InActive" : "-")
		),
		(
			tile->direction == TILE_UP ? "Up" : 
			(tile->direction == TILE_RIGHT ? "Right" : 
			(tile->direction == TILE_DOWN ? "Down" : 
			(tile->direction == TILE_LEFT ? "Left" : "-")))
		)
	);
	
	return output;
}

