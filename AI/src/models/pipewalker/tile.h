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
char *tile_type_string(char *output, tile_struct *tile);
char *tile_active_string(char *output, tile_struct *tile);
char *tile_direction_string(char *output, tile_struct *tile);

char *tile_type_string2JSON(char *output, tile_struct *tile);
char *tile_direction_string2JSON(char *output, tile_struct *tile);


short int *tile_active_branches(short int tile_type, short int tile_direction);
short int tile_has_up(short int tile_type, short int tile_direction);
short int tile_has_right(short int tile_type, short int tile_direction);
short int tile_has_down(short int tile_type, short int tile_direction);
short int tile_has_left(short int tile_type, short int tile_direction);

#endif
