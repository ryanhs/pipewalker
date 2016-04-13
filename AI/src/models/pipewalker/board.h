#ifndef _BOARD_H
#define _BOARD_H

#include "../../../lib/cJSON/cJSON.h"
#include "tile.h"

typedef struct{
	short int size;
	tile_struct ***data;
	
	
	int result;
	int clientConnected;
	int pipeConnected;
	
	short int isAllClientOK;
} board_struct;

board_struct* board_create(int size);
void board_destroy(board_struct *board);

tile_struct *board_get_tile(board_struct *board, short int row, short int cell);
tile_struct *board_get_up_tile(board_struct *board, short int row, short int cell);
tile_struct *board_get_left_tile(board_struct *board, short int row, short int cell);
tile_struct *board_get_down_tile(board_struct *board, short int row, short int cell);
tile_struct *board_get_right_tile(board_struct *board, short int row, short int cell);

void board_parseJSON(board_struct *board, cJSON *JSON);
void board_evaluator(board_struct *board);

#endif