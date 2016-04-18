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

short int *board_get_up_coordinate(board_struct *board, short int row, short int cell){
	short int *result = calloc(2, sizeof(short int));
						 result[0] = (row - 1) == -1 ? (board->size - 1) : (row - 1);
						 result[1] = cell;
	return result;
}
short int *board_get_right_coordinate(board_struct *board, short int row, short int cell){
	short int *result = calloc(2, sizeof(short int));
						 result[0] = row;
						 result[1] = (cell + 1) == board->size ? 0 : (cell + 1);
	return result;
}
short int *board_get_down_coordinate(board_struct *board, short int row, short int cell){
	short int *result = calloc(2, sizeof(short int));
						 result[0] = (row + 1) == board->size ? 0 : (row + 1);
						 result[1] = cell;
	return result;
}
short int *board_get_left_coordinate(board_struct *board, short int row, short int cell){
	short int *result = calloc(2, sizeof(short int));
						 result[0] = row;
						 result[1] = (cell - 1) == -1 ? (board->size - 1) : (cell - 1);
	return result;
}

tile_struct *board_get_tile(board_struct *board, short int row, short int cell){
	if(row < board->size && cell < board->size){
		return board->data[row][cell];
	}

	return NULL;
}
tile_struct *board_get_up_tile(board_struct *board, short int row, short int cell){
	tile_struct *currentCell = board_get_tile(board, row, cell);
	tile_struct *resultCell;
	short int *coor;
	
	if(currentCell != NULL){
		coor = board_get_up_coordinate(board, row, cell);
		resultCell = board_get_tile(
			board,
			*(coor + 0),
			*(coor + 1)
		);
		free(coor);
		return resultCell;
	}

	return NULL;
}
tile_struct *board_get_right_tile(board_struct *board, short int row, short int cell){
	tile_struct *currentCell = board_get_tile(board, row, cell);
	tile_struct *resultCell;
	short int *coor;
	
	if(currentCell != NULL){
		coor = board_get_right_coordinate(board, row, cell);
		resultCell = board_get_tile(
			board,
			*(coor + 0),
			*(coor + 1)
		);
		free(coor);
		return resultCell;
	}

	return NULL;
}
tile_struct *board_get_down_tile(board_struct *board, short int row, short int cell){
	tile_struct *currentCell = board_get_tile(board, row, cell);
	tile_struct *resultCell;
	short int *coor;
	
	if(currentCell != NULL){
		coor = board_get_down_coordinate(board, row, cell);
		resultCell = board_get_tile(
			board,
			*(coor + 0),
			*(coor + 1)
		);
		free(coor);
		return resultCell;
	}

	return NULL;
}
tile_struct *board_get_left_tile(board_struct *board, short int row, short int cell){
	tile_struct *currentCell = board_get_tile(board, row, cell);
	tile_struct *resultCell;
	short int *coor;
	
	if(currentCell != NULL){
		coor = board_get_left_coordinate(board, row, cell);
		resultCell = board_get_tile(
			board,
			*(coor + 0),
			*(coor + 1)
		);
		free(coor);
		return resultCell;
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
	short int row_i;
	short int cell_i;
	
	// make all inactive
	for(row_i = 0; row_i < board->size; row_i++){
		for(cell_i = 0; cell_i < board->size; cell_i++){
			board->data[row_i][cell_i]->active = TILE_INACTIVE;
		}
	}
	
	
	// ------------------------------------------------------------------------------------
	
	
	short int *sourceCoordinate = board_get_source_coordinate(board);
	//printf("[%d,%d]\n", board_row(sourceCoordinate), board_cell(sourceCoordinate));
	
	map_item *openSet = map_item_add(NULL, "source");
						openSet->valueInt1 = board_row(sourceCoordinate);
						openSet->valueInt2 = board_cell(sourceCoordinate);
						free(sourceCoordinate);
	
	map_item *currentSet;
	tile_struct *currentTile;
	
	map_item *branchSet;
	short int *branchCoor;
	tile_struct *branchTile;
	char branchKey[64];
	
	while(openSet != NULL){
		currentSet = openSet;
		currentTile = board_get_tile(board, currentSet->valueInt1, currentSet->valueInt2);
		
		if(tile_has_up(currentTile->type, currentTile->direction)){
			branchCoor = board_get_up_coordinate(board, currentSet->valueInt1, currentSet->valueInt2);
			branchTile = board_get_up_tile(board, currentSet->valueInt1, currentSet->valueInt2);
			
			memset(branchKey, 0, 64);
			sprintf(branchKey, "[%d,%d]", board_row(branchCoor), board_cell(branchCoor));
			
			if(tile_has_down(branchTile->type, branchTile->direction)){
				if(branchTile->active == TILE_ACTIVE){
					currentTile->active = TILE_ACTIVE;
				}else{
					if(map_item_exists(openSet, branchKey) == 0){
						branchSet = map_item_add(openSet, branchKey);
									branchSet->valueInt1 = board_row(branchCoor);
									branchSet->valueInt2 = board_cell(branchCoor);
					}
				}
			}
		}
		
		if(tile_has_right(currentTile->type, currentTile->direction)){
			branchCoor = board_get_right_coordinate(board, currentSet->valueInt1, currentSet->valueInt2);
			branchTile = board_get_right_tile(board, currentSet->valueInt1, currentSet->valueInt2);
			
			memset(branchKey, 0, 64);
			sprintf(branchKey, "[%d,%d]", board_row(branchCoor), board_cell(branchCoor));
			
			if(tile_has_left(branchTile->type, branchTile->direction)){
				if(branchTile->active == TILE_ACTIVE){
					currentTile->active = TILE_ACTIVE;
				}else{
					if(map_item_exists(openSet, branchKey) == 0){
						branchSet = map_item_add(openSet, branchKey);
									branchSet->valueInt1 = board_row(branchCoor);
									branchSet->valueInt2 = board_cell(branchCoor);
					}
				}
			}
		}
		
		if(tile_has_down(currentTile->type, currentTile->direction)){
			branchCoor = board_get_down_coordinate(board, currentSet->valueInt1, currentSet->valueInt2);
			branchTile = board_get_down_tile(board, currentSet->valueInt1, currentSet->valueInt2);
			
			memset(branchKey, 0, 64);
			sprintf(branchKey, "[%d,%d]", board_row(branchCoor), board_cell(branchCoor));
			
			if(tile_has_up(branchTile->type, branchTile->direction)){
				if(branchTile->active == TILE_ACTIVE){
					currentTile->active = TILE_ACTIVE;
				}else{
					if(map_item_exists(openSet, branchKey) == 0){
						branchSet = map_item_add(openSet, branchKey);
									branchSet->valueInt1 = board_row(branchCoor);
									branchSet->valueInt2 = board_cell(branchCoor);
					}
				}
			}
		}
		
		if(tile_has_left(currentTile->type, currentTile->direction)){
			branchCoor = board_get_left_coordinate(board, currentSet->valueInt1, currentSet->valueInt2);
			branchTile = board_get_left_tile(board, currentSet->valueInt1, currentSet->valueInt2);
			
			memset(branchKey, 0, 64);
			sprintf(branchKey, "[%d,%d]", board_row(branchCoor), board_cell(branchCoor));
			
			if(tile_has_right(branchTile->type, branchTile->direction)){
				if(branchTile->active == TILE_ACTIVE){
					currentTile->active = TILE_ACTIVE;
				}else{
					if(map_item_exists(openSet, branchKey) == 0){
						branchSet = map_item_add(openSet, branchKey);
									branchSet->valueInt1 = board_row(branchCoor);
									branchSet->valueInt2 = board_cell(branchCoor);
					}
				}
			}
		}
		
		if(currentTile->type == TILE_SOURCE){
			currentTile->active = TILE_ACTIVE;
		}
		
		openSet = currentSet->next;
		map_item_remove(currentSet, currentSet->key);
	}
	
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

char *board_JSONString(board_struct *board){
	cJSON *json = board_JSON(board);
	char *out=cJSON_PrintUnformatted(json);
	
	cJSON_Delete(json);
	return out;
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

short int *coordinate_format(short int row_i, short int cell_i){
	short int *result = calloc(2, sizeof(short int));
						 result[0] = row_i;
						 result[1] = cell_i;
	return result;
}


short int board_row(short int *coor){ return *(coor + 0); }
short int board_cell(short int *coor){ return *(coor + 1); }
