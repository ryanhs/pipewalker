#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include "../childs/dataRefresher.h"
#include "../../lib/cJSON/cJSON.h"
#include "../helpers/handler.h"
#include "../helpers/map.h"
#include "../models/pipewalker/board.h"
#include "../models/pipewalker/algorithm/AStar.h"

#include "AI.h"

map_item *boards;
int boards_idx = 0;

int method_ai_test(cJSON *params, cJSON **result, char *error_msg){
	*result = cJSON_CreateString("test AI debug");
	return 0;
}

int method_ai_newBoard(cJSON *params, cJSON **result, char *error_msg){
	if(!params) return RESPONSE_INVALID_PARAMS;
	if(params->type != cJSON_Object) return RESPONSE_INVALID_PARAMS;
	
	char jsonstring[5000];
	int size = 0;
	cJSON *tmp;
	
	tmp = cJSON_GetObjectItem(params, "size");
	if(tmp)
		if(tmp->type == cJSON_Number)
			size = tmp->valueint;
				
	tmp = cJSON_GetObjectItem(params, "json");
	if(tmp)
		if(tmp->type == cJSON_String)
			if(strlen(tmp->valuestring) < 5000)
				strcpy(jsonstring, tmp->valuestring);
	
	// filter vars
	if(size == 0) return RESPONSE_INVALID_PARAMS;
	if(strlen(jsonstring) == 0) return RESPONSE_INVALID_PARAMS;
	
	boards_idx++;
	char tmpId[5];
	sprintf(tmpId, "%d", boards_idx);
	if(boards == NULL) boards = map_item_add(NULL, tmpId);
	else boards = map_item_add(boards, tmpId);
	
	cJSON *json=cJSON_Parse(jsonstring);
	board_struct *testBoard = 	board_create(size);
								board_parseJSON(testBoard, json);
								board_reconnectAll(testBoard);
								board_evaluator(testBoard);
	cJSON_Delete(json);
	
	boards->valueInt1 = size;
	boards->valueVoid1 = (void *) testBoard;
	
	*result = 	cJSON_CreateObject();
				cJSON_AddNumberToObject(*result, "id", boards_idx);
				cJSON_AddNumberToObject(*result, "size", size);
				cJSON_AddNumberToObject(*result, "json_length", strlen(jsonstring));
	return 0;
}

int method_ai_closeBoard(cJSON *params, cJSON **result, char *error_msg){
	if(!params) return RESPONSE_INVALID_PARAMS;
	if(params->type != cJSON_Object) return RESPONSE_INVALID_PARAMS;
	
	int tmpId = 0;
	cJSON *tmp;
	
	tmp = cJSON_GetObjectItem(params, "id");
	if(tmp)
		if(tmp->type == cJSON_Number)
			tmpId = tmp->valueint;
			
	// filter vars
	if(tmpId == 0) return RESPONSE_INVALID_PARAMS;
	
	char tmpIdStr[5];
	sprintf(tmpIdStr, "%d", tmpId);
	
	map_item *tmpItem = map_item_get(boards, tmpIdStr);
						board_destroy((board_struct *) tmpItem->valueVoid1);
						if(map_size(boards) == 1){ map_item_delete(boards); boards = NULL; }
						else map_item_remove(boards, tmpItem->key);
								
	*result = 	cJSON_CreateObject();
				cJSON_AddNumberToObject(*result, "id", tmpId);
				cJSON_AddStringToObject(*result, "status", "deleted");
	return 0;
}

int method_ai_getBoard(cJSON *params, cJSON **result, char *error_msg){
	if(!params) return RESPONSE_INVALID_PARAMS;
	if(params->type != cJSON_Object) return RESPONSE_INVALID_PARAMS;
	
	int tmpId = 0;
	cJSON *tmp;
	
	tmp = cJSON_GetObjectItem(params, "id");
	if(tmp)
		if(tmp->type == cJSON_Number)
			tmpId = tmp->valueint;
			
	// filter vars
	if(tmpId == 0) return RESPONSE_INVALID_PARAMS;
	
	char tmpIdStr[5];
	sprintf(tmpIdStr, "%d", tmpId);
	
	map_item *tmpItem = map_item_get(boards, tmpIdStr);
	char *jsonstring = board_JSONString((board_struct *) tmpItem->valueVoid1);
								
	*result = 	cJSON_CreateObject();
				cJSON_AddNumberToObject(*result, "id", tmpId);
				cJSON_AddStringToObject(*result, "json", jsonstring);
				
	free(jsonstring);
	return 0;
}


// ------------------------------------------------------------------------------------------------------------

static void *AStar_ai_worker_start(void *args){
	int AStar_callback_i = 0;
	char tmpIdStr[5];
	sprintf(tmpIdStr, "%d",  *((int *) args));
	map_item *tmpItem = map_item_get(boards, tmpIdStr);
	
	void AStar_each_move_callback(board_struct *currentBoard){
		if(!currentBoard) return;
		if(currentBoard->clientConnected == 0) return;
		
		AStar_callback_i++;
		printf("A* #%d  result:%d   pipe:%d   client:%d\t%s\n\n", 	AStar_callback_i,
																currentBoard->result,
																currentBoard->pipeConnected,
																currentBoard->clientConnected,
																currentBoard->isAllClientOK ? "[finish]" : ""
															);
															
		if(AStar_callback_i > 50) pthread_ms_sleep(250);
		if(AStar_callback_i > 75) pthread_ms_sleep(150);
		if(AStar_callback_i > 100) pthread_ms_sleep(100);
		//~ pthread_ms_sleep(250);
		//~ pthread_mysleep(1);
		//~ usleep(500000000);
		
		board_struct *tmpBoard = (board_struct *) tmpItem->valueVoid1;
		board_struct *testBoard = 	board_clone(currentBoard);
									//~ board_reconnectAll(testBoard);
									//~ board_evaluator(testBoard);
									
		tmpItem->valueVoid1 = testBoard;
		if(AStar_callback_i > 1) board_destroy(tmpBoard);
	}
	
	
	AI_AStar_run((board_struct *) tmpItem->valueVoid1, AStar_each_move_callback, 100);
	free(args);
	return 0;
}

int method_ai_AStar(cJSON *params, cJSON **result, char *error_msg){
	if(!params) return RESPONSE_INVALID_PARAMS;
	if(params->type != cJSON_Object) return RESPONSE_INVALID_PARAMS;
	
	int tmpId = 0;
	cJSON *tmp;
	
	tmp = cJSON_GetObjectItem(params, "id");
	if(tmp)
		if(tmp->type == cJSON_Number)
			tmpId = tmp->valueint;
			
	// filter vars
	if(tmpId == 0) return RESPONSE_INVALID_PARAMS;

	int *boardIdPointer = malloc(sizeof(int));
		*boardIdPointer = tmpId;
    pthread_t thread;
	pthread_create(&thread, NULL, &AStar_ai_worker_start, boardIdPointer);
	
	tmp = cJSON_GetObjectItem(params, "debug");
	if(tmp)
		if(tmp->type == cJSON_True){
			printf("A*: Waiting for thread finish.. \n");
			pthread_join(thread, NULL);
		}
	
	*result = 	cJSON_CreateObject();
				cJSON_AddNumberToObject(*result, "id", tmpId);
				cJSON_AddStringToObject(*result, "status", "in progress");
	return 0;
}
