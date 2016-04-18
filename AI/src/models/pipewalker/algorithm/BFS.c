#include <malloc.h>
#include <string.h>

#include "../../../helpers/map.h"
#include "../tile.h"
#include "../board.h"
#include "BFS.h"
#include "../../../../lib/safeTo/safeTo.h"



void AI_BFS(board_struct *testBoard, void (*each_move_callback)(board_struct *)){
	AI_BFS_run(testBoard, each_move_callback, 5000);
}

void AI_BFS_run(board_struct *testBoard, void (*each_move_callback)(board_struct *), int maxIteration){
	board_struct *currentBoard = board_clone(testBoard);
								 board_reconnectAll(currentBoard);
								 board_evaluator(currentBoard);
	if(currentBoard->isAllClientOK == 1) return;
	
	char *currentJSONString = board_JSONString(currentBoard);
	map_item *log = map_item_add(NULL, currentJSONString);
	map_item *stack = map_item_add(NULL, currentJSONString);

	cJSON *tmpJSON;
	map_item *tmpItem;
	char *tmpJSONString;
	board_struct *tmpBoard;
	board_struct *tmpBestBoard;
	int tmpBestResult;
	short int row_i;
	short int cell_i;
	short int i = 0;
	while(1){
		i++;
		if(currentBoard->isAllClientOK == 1) return;
		if(i > maxIteration) return;
		
		tmpBestResult = 0;
		tmpBestBoard = NULL;
		
		for(row_i = 0; row_i < currentBoard->size; row_i++){
			for(cell_i = 0; cell_i < currentBoard->size; cell_i++){
				if(board_get_tile(currentBoard, row_i, cell_i)->type != TILE_BLANK){
					
					// TILE_UP
					tmpBoard =  board_clone(currentBoard);
					board_get_tile(tmpBoard, row_i, cell_i)->direction = TILE_UP;
					board_reconnectAll(tmpBoard);
					board_evaluator(tmpBoard);
					tmpJSONString = board_JSONString(tmpBoard);
					if(!map_item_exists(log, tmpJSONString) && tmpBoard->result > tmpBestResult && board_get_tile(tmpBoard, row_i, cell_i)->active == TILE_ACTIVE){
						tmpBestBoard = tmpBoard;
						tmpBestResult = tmpBoard->result;
					}else{
						board_destroy(tmpBoard);
					}
					free(tmpJSONString);
					
					// TILE_RIGHT
					tmpBoard =  board_clone(currentBoard);
					board_get_tile(tmpBoard, row_i, cell_i)->direction = TILE_RIGHT;
					board_reconnectAll(tmpBoard);
					board_evaluator(tmpBoard);
					tmpJSONString = board_JSONString(tmpBoard);
					if(!map_item_exists(log, tmpJSONString) && tmpBoard->result > tmpBestResult && board_get_tile(tmpBoard, row_i, cell_i)->active == TILE_ACTIVE){
						tmpBestBoard = tmpBoard;
						tmpBestResult = tmpBoard->result;
					}else{
						board_destroy(tmpBoard);
					}
					free(tmpJSONString);
					
					// TILE_DOWN
					tmpBoard =  board_clone(currentBoard);
					board_get_tile(tmpBoard, row_i, cell_i)->direction = TILE_DOWN;
					board_reconnectAll(tmpBoard);
					board_evaluator(tmpBoard);
					tmpJSONString = board_JSONString(tmpBoard);
					if(!map_item_exists(log, tmpJSONString) && tmpBoard->result > tmpBestResult && board_get_tile(tmpBoard, row_i, cell_i)->active == TILE_ACTIVE){
						tmpBestBoard = tmpBoard;
						tmpBestResult = tmpBoard->result;
					}else{
						board_destroy(tmpBoard);
					}
					free(tmpJSONString);
					
					// TILE_LEFT
					tmpBoard =  board_clone(currentBoard);
					board_get_tile(tmpBoard, row_i, cell_i)->direction = TILE_LEFT;
					board_reconnectAll(tmpBoard);
					board_evaluator(tmpBoard);
					tmpJSONString = board_JSONString(tmpBoard);
					if(!map_item_exists(log, tmpJSONString) && tmpBoard->result > tmpBestResult && board_get_tile(tmpBoard, row_i, cell_i)->active == TILE_ACTIVE){
						tmpBestBoard = tmpBoard;
						tmpBestResult = tmpBoard->result;
					}else{
						board_destroy(tmpBoard);
					}
					free(tmpJSONString);
				}
			}
		}
		
		if(tmpBestResult > 0 && tmpBestBoard != NULL){
			board_destroy(currentBoard);
			currentBoard =  board_clone(tmpBestBoard);
							board_reconnectAll(currentBoard);
							board_evaluator(currentBoard);
			tmpJSONString = board_JSONString(currentBoard);
			
			map_item_add(log, tmpJSONString);
			map_item_add(stack, tmpJSONString);
			free(tmpJSONString);
			each_move_callback(currentBoard);
		}else{
			tmpItem = map_item_pop(stack);
			
			tmpJSON = cJSON_Parse(tmpItem->key);
			currentBoard =  board_clone(testBoard);
							board_parseJSON(currentBoard, tmpJSON);
							board_reconnectAll(currentBoard);
							board_evaluator(currentBoard);
			
			cJSON_Delete(tmpJSON);
			//~ if(tmpItem) free(tmpItem); // this need to be fixed, aargh!
			each_move_callback(currentBoard);
		}
	}
	
	free(currentJSONString);
	map_item_delete(log);
	map_item_delete(stack);
	board_destroy(currentBoard);
}
