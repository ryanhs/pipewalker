#include <malloc.h>
#include <string.h>

#include "tile.h"
#include "board.h"
#include "../../../lib/safeTo/safeTo.h"
#include "../../helpers/map.h"


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
				board->data[row_i][cell_i]->active = cJSON_GetObjectItem(cell_tmp, "active")->type == cJSON_True ? TILE_ACTIVE : TILE_INACTIVE;
				
				// direction
				board->data[row_i][cell_i]->direction = (
					(strcmp(cJSON_GetObjectItem(cell_tmp, "direction")->valuestring, "up") == 0 ? TILE_UP :
					(strcmp(cJSON_GetObjectItem(cell_tmp, "direction")->valuestring, "left") == 0 ? TILE_LEFT : 
					(strcmp(cJSON_GetObjectItem(cell_tmp, "direction")->valuestring, "down") == 0 ? TILE_DOWN : 
					(strcmp(cJSON_GetObjectItem(cell_tmp, "direction")->valuestring, "right") == 0 ? TILE_RIGHT : TILE_UP
					))))
				);
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

void board_reconnectAll(board_struct *board){
	
	
	
}

board_struct *board_clone(board_struct *board){
	board_struct *newBoard = board_create(board->size);
	
	short int row_i;
	short int cell_i;
	for(row_i = 0; row_i < board->size; row_i++){
		for(cell_i = 0; cell_i < board->size; cell_i++){
			newBoard->data[row_i][cell_i]->type = 		board->data[row_i][cell_i]->type;
			newBoard->data[row_i][cell_i]->active = 	board->data[row_i][cell_i]->active;
			newBoard->data[row_i][cell_i]->direction =	board->data[row_i][cell_i]->direction;
		}
	}
	
	return newBoard;
}

cJSON *board_JSON(board_struct *board){
	cJSON *root;
	cJSON *row_tmp;
	cJSON *cell_tmp;
	char tmpStr[64];
	
	root = cJSON_CreateArray();
	
	short int row_i;
	short int cell_i;
	for(row_i = 0; row_i < board->size; row_i++){
		row_tmp = cJSON_CreateArray();
		
		for(cell_i = 0; cell_i < board->size; cell_i++){
			cell_tmp = cJSON_CreateObject();
			
			
			cJSON_AddStringToObject(cell_tmp, "type", tile_type_string2JSON(tmpStr, board_get_tile(board, row_i, cell_i)));
			if(board_get_tile(board, row_i, cell_i)->active == TILE_ACTIVE)
				cJSON_AddTrueToObject(cell_tmp, "active");
			else
				cJSON_AddFalseToObject(cell_tmp, "active");
			cJSON_AddStringToObject(cell_tmp, "direction", tile_direction_string2JSON(tmpStr, board_get_tile(board, row_i, cell_i)));
			
			cJSON_AddItemToArray(row_tmp, cell_tmp);
		}
		
		cJSON_AddItemToArray(root, row_tmp);
	}
	
	return root;	
}

short int *board_get_source_coordinate(board_struct *board){
	short int row_i;
	short int cell_i;
	
	short int *result = calloc(2, sizeof(short int));	
						 result[0] = -1;
						 result[1] = -1;
	
	for(row_i = 0; row_i < board->size; row_i++){
		for(cell_i = 0; cell_i < board->size; cell_i++){
			if(board_get_tile(board, row_i, cell_i)->type == TILE_SOURCE){				
				result[0] = row_i;
				result[1] = cell_i;
				break;
				break;
			}
		}
	}
	
	
	if(result[0] == -1 && result[1] == -1) return NULL; // there is no source!!!
	
	return result;
}
