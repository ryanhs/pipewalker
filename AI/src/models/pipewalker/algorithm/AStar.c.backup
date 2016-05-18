#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "../../../helpers/map.h"
#include "../tile.h"
#include "../board.h"
#include "AStar.h"
#include "../../../../lib/safeTo/safeTo.h"



void AI_AStar(board_struct *testBoard, void (*each_move_callback)(board_struct *)){
	AI_AStar_run(testBoard, each_move_callback, 5000);
}

void AI_AStar_run(board_struct *testBoard, void (*each_move_callback)(board_struct *), int maxIteration){
	board_struct *currentBoard = board_clone(testBoard);
								 board_reconnectAll(currentBoard);
								 board_evaluator(currentBoard);
	if(currentBoard->isAllClientOK == 1) return;
	
	char *currentJSONString = board_JSONString(currentBoard);
	cJSON *currentJSON;
	int currentResult;
	
	map_item *closedSet = NULL;
	map_item *openSet = map_item_add(NULL, currentJSONString);
	
	map_item *cameFrom = NULL;
	
	map_item *gScore = 	map_item_add(NULL, currentJSONString);
						gScore->valueInt1 = 0;
	
	map_item *fScore =	map_item_add(NULL, currentJSONString);
						fScore->valueInt1 = abs(currentBoard->result - 1000); // use -1000 to replicate heuristic_cost_estimate()
	safeFree(currentJSONString); currentJSONString = NULL;
	board_destroy(currentBoard);
	
	map_item *tmpSet;
	map_item *tmpSetBest;
	board_struct *tmpBoard;
	char *tmpJSONString;
	int tentative_gScore;
	int i = 0;
	int allGood;
	while(1){
		i++;
		if(openSet == NULL) break;
		if(i > maxIteration) break;
		
		// get lowest openSet
		currentResult = 0;
		tmpSet = openSet;
		map_reset(&tmpSet);
		while(tmpSet != NULL){
			if(tmpSet->valueInt1 >= currentResult){
				currentResult = tmpSet->valueInt1;
				tmpSetBest = tmpSet;
			}
		tmpSet = tmpSet->next; }
		// make the lowest parsed
		currentJSON = cJSON_Parse(tmpSetBest->key);
		currentBoard =  board_clone(testBoard);
						board_parseJSON(currentBoard, currentJSON);
						board_reconnectAll(currentBoard);
						board_evaluator(currentBoard);
		currentJSONString = board_JSONString(currentBoard);
		
		// show process
		each_move_callback(currentBoard);
		if(testBoard) board_destroy(testBoard);
		testBoard = board_clone(currentBoard);
					//~ board_parseJSON(testBoard, currentJSON);
					//~ board_reconnectAll(testBoard);
					//~ board_evaluator(testBoard);
		
		// if finish reconstruct path !!!!!
		if(currentBoard->isAllClientOK == 1){			
			cJSON_Delete(currentJSON);
			board_destroy(currentBoard);
			safeFree(currentJSONString);
			break;
		} // recon
		
		// openSet splice(0, 1)
		if(map_size(openSet) > 1){ map_item_remove(openSet, currentJSONString); }
		else map_item_delete(openSet); openSet = NULL;
		
		// closedSet push
		if(closedSet == NULL) closedSet = map_item_add(NULL, currentJSONString);
		else closedSet = map_item_add(closedSet, currentJSONString);
		
		
		// process neighbors
		int row_i;
		int cell_i;
		for(row_i = 0; row_i < currentBoard->size; row_i++){
			for(cell_i = 0; cell_i < currentBoard->size; cell_i++){
				if(board_get_tile(currentBoard, row_i, cell_i)->type != TILE_BLANK){
					
					// TILE_UP
					tmpBoard = board_clone(currentBoard);
							   board_get_tile(tmpBoard, row_i, cell_i)->direction = TILE_UP;
							   board_reconnectAll(tmpBoard);
							   board_evaluator(tmpBoard);
					tmpJSONString = board_JSONString(tmpBoard);
					if(!map_item_exists(closedSet, tmpJSONString)){
						tentative_gScore = map_item_get(gScore, currentJSONString)->valueInt1 + 1;
						allGood = 1;
						if(openSet == NULL){
							openSet = map_item_add(NULL, tmpJSONString);
							openSet->valueInt1 = tmpBoard->result;
						}else{
							if(!map_item_exists(openSet, tmpJSONString)){
								tmpSet = map_item_add(openSet, tmpJSONString);
								tmpSet->valueInt1 = tmpBoard->result;
							}else if(tentative_gScore >= map_item_get(gScore, tmpJSONString)->valueInt1){
								allGood = 0; // this is not better path
							}
						}
						if(allGood == 1){
							// cameFrom push
							if(cameFrom == NULL) cameFrom = map_item_add(NULL, tmpJSONString);
							else cameFrom = map_item_add(cameFrom, tmpJSONString);
							cameFrom->valueInt1 = tmpBoard->result;
							// gScore push
							tmpSet = map_item_add(gScore, tmpJSONString);
							tmpSet->valueInt1 = tentative_gScore;
							// fScore push
							tmpSet = map_item_add(fScore, tmpJSONString);
							tmpSet->valueInt1 = tentative_gScore + abs(tmpBoard->result - 1000);
						}
					}
					safeFree(tmpJSONString);
					board_destroy(tmpBoard);
					
					// ----------------------------------------------------------------------------------
					// TILE_RIGHT
					tmpBoard = board_clone(currentBoard);
							   board_get_tile(tmpBoard, row_i, cell_i)->direction = TILE_RIGHT;
							   board_reconnectAll(tmpBoard);
							   board_evaluator(tmpBoard);
					tmpJSONString = board_JSONString(tmpBoard);
					if(!map_item_exists(closedSet, tmpJSONString)){
						tentative_gScore = map_item_get(gScore, currentJSONString)->valueInt1 + 1;
						allGood = 1;
						if(openSet == NULL){
							openSet = map_item_add(NULL, tmpJSONString);
							openSet->valueInt1 = tmpBoard->result;
						}else{
							if(!map_item_exists(openSet, tmpJSONString)){
								tmpSet = map_item_add(openSet, tmpJSONString);
								tmpSet->valueInt1 = tmpBoard->result;
							}else if(tentative_gScore >= map_item_get(gScore, tmpJSONString)->valueInt1){
								allGood = 0; // this is not better path
							}
						}
						if(allGood == 1){
							// cameFrom push
							if(cameFrom == NULL) cameFrom = map_item_add(NULL, tmpJSONString);
							else cameFrom = map_item_add(cameFrom, tmpJSONString);
							cameFrom->valueInt1 = tmpBoard->result;
							// gScore push
							tmpSet = map_item_add(gScore, tmpJSONString);
							tmpSet->valueInt1 = tentative_gScore;
							// fScore push
							tmpSet = map_item_add(fScore, tmpJSONString);
							tmpSet->valueInt1 = tentative_gScore + abs(tmpBoard->result - 1000);
						}
					}
					safeFree(tmpJSONString);
					board_destroy(tmpBoard);
					
					// ----------------------------------------------------------------------------------
					// TILE_DOWN
					tmpBoard = board_clone(currentBoard);
							   board_get_tile(tmpBoard, row_i, cell_i)->direction = TILE_DOWN;
							   board_reconnectAll(tmpBoard);
							   board_evaluator(tmpBoard);
					tmpJSONString = board_JSONString(tmpBoard);
					if(!map_item_exists(closedSet, tmpJSONString)){
						tentative_gScore = map_item_get(gScore, currentJSONString)->valueInt1 + 1;
						allGood = 1;
						if(openSet == NULL){
							openSet = map_item_add(NULL, tmpJSONString);
							openSet->valueInt1 = tmpBoard->result;
						}else{
							if(!map_item_exists(openSet, tmpJSONString)){
								tmpSet = map_item_add(openSet, tmpJSONString);
								tmpSet->valueInt1 = tmpBoard->result;
							}else if(tentative_gScore >= map_item_get(gScore, tmpJSONString)->valueInt1){
								allGood = 0; // this is not better path
							}
						}
						if(allGood == 1){
							// cameFrom push
							if(cameFrom == NULL) cameFrom = map_item_add(NULL, tmpJSONString);
							else cameFrom = map_item_add(cameFrom, tmpJSONString);
							cameFrom->valueInt1 = tmpBoard->result;
							// gScore push
							tmpSet = map_item_add(gScore, tmpJSONString);
							tmpSet->valueInt1 = tentative_gScore;
							// fScore push
							tmpSet = map_item_add(fScore, tmpJSONString);
							tmpSet->valueInt1 = tentative_gScore + abs(tmpBoard->result - 1000);
						}
					}
					safeFree(tmpJSONString);
					board_destroy(tmpBoard);
					
					// ----------------------------------------------------------------------------------
					// TILE_LEFT
					tmpBoard = board_clone(currentBoard);
							   board_get_tile(tmpBoard, row_i, cell_i)->direction = TILE_LEFT;
							   board_reconnectAll(tmpBoard);
							   board_evaluator(tmpBoard);
					tmpJSONString = board_JSONString(tmpBoard);
					if(!map_item_exists(closedSet, tmpJSONString)){
						tentative_gScore = map_item_get(gScore, currentJSONString)->valueInt1 + 1;
						allGood = 1;
						if(openSet == NULL){
							openSet = map_item_add(NULL, tmpJSONString);
							openSet->valueInt1 = tmpBoard->result;
						}else{
							if(!map_item_exists(openSet, tmpJSONString)){
								tmpSet = map_item_add(openSet, tmpJSONString);
								tmpSet->valueInt1 = tmpBoard->result;
							}else if(tentative_gScore >= map_item_get(gScore, tmpJSONString)->valueInt1){
								allGood = 0; // this is not better path
							}
						}
						if(allGood == 1){
							// cameFrom push
							if(cameFrom == NULL) cameFrom = map_item_add(NULL, tmpJSONString);
							else cameFrom = map_item_add(cameFrom, tmpJSONString);
							cameFrom->valueInt1 = tmpBoard->result;
							// gScore push
							tmpSet = map_item_add(gScore, tmpJSONString);
							tmpSet->valueInt1 = tentative_gScore;
							// fScore push
							tmpSet = map_item_add(fScore, tmpJSONString);
							tmpSet->valueInt1 = tentative_gScore + abs(tmpBoard->result - 1000);
						}
					}
					safeFree(tmpJSONString);
					board_destroy(tmpBoard);
					
				}
			}
		}
		
		
		// clean up
		cJSON_Delete(currentJSON);
		board_destroy(currentBoard);
		safeFree(currentJSONString);
	}
	
	if(closedSet != NULL) 	map_item_delete(closedSet);
	if(openSet != NULL) 	map_item_delete(openSet);
	if(gScore != NULL) 		map_item_delete(gScore);
	if(fScore != NULL) 		map_item_delete(fScore);
	if(cameFrom != NULL) 	map_item_delete(cameFrom);
}
