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
tile_struct *board_get_right_tile(board_struct *board, short int row, short int cell);
tile_struct *board_get_down_tile(board_struct *board, short int row, short int cell);
tile_struct *board_get_left_tile(board_struct *board, short int row, short int cell);

short int *board_get_up_coordinate(board_struct *board, short int row, short int cell);
short int *board_get_right_coordinate(board_struct *board, short int row, short int cell);
short int *board_get_down_coordinate(board_struct *board, short int row, short int cell);
short int *board_get_left_coordinate(board_struct *board, short int row, short int cell);

void board_parseJSON(board_struct *board, cJSON *JSON);
void board_reconnectAll(board_struct *board);
void board_evaluator(board_struct *board);

board_struct *board_clone(board_struct *board);
cJSON *board_JSON(board_struct *board);

short int *board_get_source_coordinate(board_struct *board);
short int *coordinate_format(short int row_i, short int cell_i);

short int board_row(short int *coor);
short int board_cell(short int *coor);

#endif
