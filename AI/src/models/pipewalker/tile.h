#ifndef _TILE_H
#define _TILE_H

#include "../../../lib/cJSON/cJSON.h"

#define TILE_BLANK		0
#define TILE_PIPE_1		1
#define TILE_PIPE_2		2
#define TILE_PIPE_3		3
#define TILE_CLIENT		4
#define TILE_SOURCE		5

#define TILE_UP			6
#define TILE_RIGHT		7
#define TILE_DOWN		8
#define TILE_LEFT		9

#define TILE_ACTIVE		10
#define TILE_INACTIVE	11



typedef struct{
	short int type;
	short int direction;
	short int active;
} tile_struct;

char *tile_to_string(char *output, tile_struct *tile);

#endif
