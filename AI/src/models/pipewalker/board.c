#include <malloc.h>
#include <string.h>

#include "tile.h"
#include "board.h"
#include "../../../lib/safeTo/safeTo.h"


board_struct *board_create(int size){
	board_struct *board;
	tile_struct **row;
	int row_i, cell_i;
	
	board = (void *) malloc(sizeof(board_struct));
	board->size = size;
	board->data = (tile_struct ***) calloc(size, sizeof(tile_struct **));
	
	for(row_i = 0; row_i < board->size; row_i++){
		row = (tile_struct **) malloc(size * sizeof(tile_struct *));
		for(cell_i = 0; cell_i < board->size; cell_i++){
			row[cell_i] = (tile_struct *) malloc(sizeof(tile_struct));
			
			row[cell_i]->type = TILE_BLANK;
			row[cell_i]->active = TILE_ACTIVE;
			row[cell_i]->direction = TILE_LEFT;
		}
		
		board->data[row_i] = row;
	}
	
	board->result = 0;
	board->clientConnected = 0;
	board->pipeConnected = 0;
	board->isAllClientOK = 0;
	
	return board;
}

void board_destroy(board_struct *board){
	int i = 0;
	while(board->data && i < board->size){
		free(*board->data);
		board->data++;
		i++;
	}
	free(board);
}

tile_struct *board_get_tile(board_struct *board, short int row, short int cell){
	if(row < board->size && cell < board->size){
		return board->data[row][cell];
	}
	
	return NULL;
}

tile_struct *board_get_up_tile(board_struct *board, short int row, short int cell){
	tile_struct *currentCell = board_get_tile(board, row, cell);
	if(currentCell != NULL){
		return board_get_tile(
			board, 
			(row - 1) == -1 ? (board->size - 1) : (row - 1),
			cell
		);
	}
	
	return NULL;
}

tile_struct *board_get_left_tile(board_struct *board, short int row, short int cell){
	tile_struct *currentCell = board_get_tile(board, row, cell);
	if(currentCell != NULL){
		return board_get_tile(
			board, 
			row,
			(cell - 1) == -1 ? (board->size - 1) : (cell - 1)
		);
	}
	
	return NULL;
}

tile_struct *board_get_down_tile(board_struct *board, short int row, short int cell){
	tile_struct *currentCell = board_get_tile(board, row, cell);
	if(currentCell != NULL){
		return board_get_tile(
			board, 
			(row + 1) == board->size ? 0 : (row + 1),
			cell
		);
	}
	
	return NULL;
}

tile_struct *board_get_right_tile(board_struct *board, short int row, short int cell){
	tile_struct *currentCell = board_get_tile(board, row, cell);
	if(currentCell != NULL){
		return board_get_tile(
			board, 
			row,
			(cell + 1) == board->size ? 0 : (cell + 1)
		);
	}
	
	return NULL;
}

void board_parseJSON(board_struct *board, cJSON *JSON){
	short int row_i = 0;
	short int cell_i = 0;
	
	cJSON *row_tmp;
	cJSON *cell_tmp;
	
	row_tmp = JSON->child;
	while (row_tmp){
		cell_i = 0;
		cell_tmp = row_tmp->child;
		while (cell_tmp){
			
			if(cell_tmp->type == cJSON_Object){
				// type
				board->data[row_i][cell_i]->type = (
					(strcmp(cJSON_GetObjectItem(cell_tmp, "type")->valuestring, "client") == 0 ? TILE_CLIENT :
					(strcmp(cJSON_GetObjectItem(cell_tmp, "type")->valuestring, "source") == 0 ? TILE_SOURCE : 
					(strcmp(cJSON_GetObjectItem(cell_tmp, "type")->valuestring, "pipe-1") == 0 ? TILE_PIPE_1 : 
					(strcmp(cJSON_GetObjectItem(cell_tmp, "type")->valuestring, "pipe-2") == 0 ? TILE_PIPE_2 : 
					(strcmp(cJSON_GetObjectItem(cell_tmp, "type")->valuestring, "pipe-3") == 0 ? TILE_PIPE_3 : TILE_BLANK
					)))))
				);
				
				// active
				board->data[row_i][cell_i]->active = cJSON_GetObjectItem(cell_tmp, "active")->valueint == 1 ? TILE_ACTIVE : TILE_INACTIVE;
				
				// direction
				if(strcmp(cJSON_GetObjectItem(cell_tmp, "type")->valuestring, "blank") == 0){
					board->data[row_i][cell_i]->direction = TILE_UP;
				}else{
					board->data[row_i][cell_i]->direction = (
						(strcmp(cJSON_GetObjectItem(cell_tmp, "direction")->valuestring, "up") == 0 ? TILE_UP :
						(strcmp(cJSON_GetObjectItem(cell_tmp, "direction")->valuestring, "left") == 0 ? TILE_LEFT : 
						(strcmp(cJSON_GetObjectItem(cell_tmp, "direction")->valuestring, "down") == 0 ? TILE_DOWN : 
						(strcmp(cJSON_GetObjectItem(cell_tmp, "direction")->valuestring, "right") == 0 ? TILE_RIGHT : TILE_UP
						))))
					);
				}
			}
			
		cell_i++;
		cell_tmp = cell_tmp->next; }
	
	row_i++;
	row_tmp = row_tmp->next; }
}


void board_evaluator(board_struct *board){
	short int row_i;
	short int cell_i;
	tile_struct *tmp;
	
	board->clientConnected = 0;
	board->pipeConnected = 0;
	board->result = 0;
	board->isAllClientOK = 1;
	
	for(row_i = 0; row_i < board->size; row_i++){
		for(cell_i = 0; cell_i < board->size; cell_i++){
			tmp = board_get_tile(board, row_i, cell_i);
			
			// check client connected
			if((tmp->type == TILE_CLIENT) && (tmp->active == TILE_ACTIVE)) board->clientConnected++;
			
			// check pipe connected
			if((tmp->type == TILE_PIPE_1) && (tmp->active == TILE_ACTIVE)) board->pipeConnected++;
			if((tmp->type == TILE_PIPE_2) && (tmp->active == TILE_ACTIVE)) board->pipeConnected++;
			if((tmp->type == TILE_PIPE_3) && (tmp->active == TILE_ACTIVE)) board->pipeConnected++;
			
			// check isAllClientOK
			if((tmp->type == TILE_CLIENT) && (tmp->active == TILE_INACTIVE)) board->isAllClientOK = 0;
			
		}
	}
	
	// make result
	board->result = (board->clientConnected * 3) + (board->pipeConnected * 1);
}
