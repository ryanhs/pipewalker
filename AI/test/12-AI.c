#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <sys/time.h>

#include "../src/childs/dataRefresher.h"

#include "../lib/unitTesting/lib/unitTesting.h"
#include "../src/models/config.h"
#include "../src/helpers/handler.h"
#include "../src/models/route.h"

#include "../src/methods/AI.h"

#include "../src/helpers/map.h"

#include "../src/models/pipewalker/tile.h"
#include "../src/models/pipewalker/board.h"
#include "../src/models/pipewalker/algorithm/BFS.h"
#include "../src/models/pipewalker/algorithm/AStar.h"

int callback_i = 0;
void each_move_callback(board_struct *currentBoard){
	callback_i++;
	if(currentBoard->clientConnected == 0) return;
	
	printf("============== #%d [%s]\n", callback_i, currentBoard->isAllClientOK == 1 ? "finish" : "running");
	printf("\tresult:\t\t %d\n", currentBoard->result);
	printf("\tpipeConnected:\t %d\n", currentBoard->pipeConnected);
	printf("\tclientConnected: %d\n\n", currentBoard->clientConnected);
		
	//~ pthread_mysleep(60 * 3);
	pthread_ms_sleep(1);
}

void each_move_callback_null(board_struct *currentBoard){
	callback_i++;
	if(currentBoard->clientConnected == 0) return;
	
	//~ pthread_mysleep(60 * 3);
	pthread_ms_sleep(1);
}

void testBoardBFS(){
	FILE *f;
	long len;
	char *text;
	f=fopen("test/12-pipewalker.7x7.json","rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	text=(char*)malloc(len+1);fread(text,1,len,f);fclose(f);
	cJSON *json=cJSON_Parse(text);
	
	
	board_struct *testBoard = board_create(7);
	board_parseJSON(testBoard, json);
	board_reconnectAll(testBoard);
	
	callback_i = 0;
	AI_BFS_run(testBoard, each_move_callback_null, 100);
	//~ AI_BFS_run(testBoard, each_move_callback, 5000);
	
	cJSON_Delete(json);
	board_destroy(testBoard);
	free(text);
}

void testBoardAStar(){
	FILE *f;
	long len;
	char *text;
	f=fopen("test/12-pipewalker.7x7.json","rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	text=(char*)malloc(len+1);fread(text,1,len,f);fclose(f);
	cJSON *json=cJSON_Parse(text);
	
	
	board_struct *testBoard = board_create(7);
	board_parseJSON(testBoard, json);
	board_reconnectAll(testBoard);
	
	callback_i = 0;
	AI_AStar_run(testBoard, each_move_callback_null, 100);
	//~ AI_AStar_run(testBoard, each_move_callback, 5000);
	
	cJSON_Delete(json);
	board_destroy(testBoard);
	free(text);
}

void testAIMethodTest(){
	char expectedResult[] = "test AI debug";
	
	cJSON *param = cJSON_CreateObject();
	cJSON *result;
	route_call("ai_test", param, &result, NULL);
	ASSERT_LIKE(result->valuestring, expectedResult);
	cJSON_Delete(param);
	cJSON_Delete(result);
}

void testAINewBoard(){
	FILE *f;
	long len;
	char *text;
	f=fopen("test/12-pipewalker.7x7.json","rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	text=(char*)malloc(len+1);fread(text,1,len,f);fclose(f);
	
	int boardSize = 7;
	int boardId = 0;
	cJSON *param;
	cJSON *result;
	
	param =  cJSON_CreateObject();
			 cJSON_AddNumberToObject(param, "size", boardSize);
			 cJSON_AddStringToObject(param, "json", text);
	
	route_call("ai_newBoard", param, &result, NULL);
	ASSERT(cJSON_GetObjectItem(result, "id")->valueint > 0);
	ASSERT(cJSON_GetObjectItem(result, "size")->valueint == boardSize);
	ASSERT(cJSON_GetObjectItem(result, "json_length")->valueint == strlen(text));
	
	boardId = cJSON_GetObjectItem(result, "id")->valueint;
	ASSERT(boardId > 0);
	
	cJSON_Delete(param);
	cJSON_Delete(result);
	free(text);
	
	// --------------------------------------------------------------------------
	
	param =  cJSON_CreateObject();
			 cJSON_AddNumberToObject(param, "id", boardId);
			 
	route_call("ai_closeBoard", param, &result, NULL);
	ASSERT(cJSON_GetObjectItem(result, "id")->valueint == boardId);
	ASSERT_LIKE(cJSON_GetObjectItem(result, "status")->valuestring, "deleted");
	
	cJSON_Delete(param);
	cJSON_Delete(result);
}

void testAIGetBoard(){
	FILE *f;
	long len;
	char *text;
	f=fopen("test/12-pipewalker.7x7.json","rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	text=(char*)malloc(len+1);fread(text,1,len,f);fclose(f);
	
	int boardSize = 7;
	int boardId = 0;
	cJSON *param;
	cJSON *result;
	
	param =  cJSON_CreateObject();
			 cJSON_AddNumberToObject(param, "size", boardSize);
			 cJSON_AddStringToObject(param, "json", text);
	
	route_call("ai_newBoard", param, &result, NULL);
	ASSERT(cJSON_GetObjectItem(result, "id")->valueint > 0);
	ASSERT(cJSON_GetObjectItem(result, "size")->valueint == boardSize);
	ASSERT(cJSON_GetObjectItem(result, "json_length")->valueint == strlen(text));
	
	boardId = cJSON_GetObjectItem(result, "id")->valueint;
	ASSERT(boardId > 0);
	
	cJSON_Delete(param);
	cJSON_Delete(result);
	free(text);
	
	// --------------------------------------------------------------------------
	
	param =  cJSON_CreateObject();
			 cJSON_AddNumberToObject(param, "id", boardId);
			 
	route_call("ai_getBoard", param, &result, NULL);
	ASSERT(cJSON_GetObjectItem(result, "id")->valueint == boardId);
	ASSERT(strlen(cJSON_GetObjectItem(result, "json")->valuestring) > 0);
	
	cJSON_Delete(param);
	cJSON_Delete(result);
	
	// --------------------------------------------------------------------------
	
	param =  cJSON_CreateObject();
			 cJSON_AddNumberToObject(param, "id", boardId);
			 
	route_call("ai_closeBoard", param, &result, NULL);
	ASSERT(cJSON_GetObjectItem(result, "id")->valueint == boardId);
	ASSERT_LIKE(cJSON_GetObjectItem(result, "status")->valuestring, "deleted");
	
	cJSON_Delete(param);
	cJSON_Delete(result);
}

void testAIAStar(){
	FILE *f;
	long len;
	char *text;
	f=fopen("test/12-pipewalker.7x7.json","rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	text=(char*)malloc(len+1);fread(text,1,len,f);fclose(f);
	
	int boardSize = 7;
	int boardId = 0;
	cJSON *param;
	cJSON *result;
	
	param =  cJSON_CreateObject();
			 cJSON_AddNumberToObject(param, "size", boardSize);
			 cJSON_AddStringToObject(param, "json", text);
	
	route_call("ai_newBoard", param, &result, NULL);
	ASSERT(cJSON_GetObjectItem(result, "id")->valueint > 0);
	ASSERT(cJSON_GetObjectItem(result, "size")->valueint == boardSize);
	ASSERT(cJSON_GetObjectItem(result, "json_length")->valueint == strlen(text));
	
	boardId = cJSON_GetObjectItem(result, "id")->valueint;
	ASSERT(boardId > 0);
	
	cJSON_Delete(param);
	cJSON_Delete(result);
	free(text);
	
	// --------------------------------------------------------------------------
	//~ 
	param =  cJSON_CreateObject();
			 cJSON_AddNumberToObject(param, "id", boardId);
			 cJSON_AddTrueToObject(param, "debug");
			 
	route_call("ai_A*", param, &result, NULL);
	ASSERT(cJSON_GetObjectItem(result, "id")->valueint == boardId);
	ASSERT_LIKE(cJSON_GetObjectItem(result, "status")->valuestring, "in progress");
	
	//~ char *out = cJSON_Print(result);
					//~ printf("%s\n",out);
					//~ free(out);
					
	cJSON_Delete(param);
	cJSON_Delete(result);
	
	// --------------------------------------------------------------------------
	
	param =  cJSON_CreateObject();
			 cJSON_AddNumberToObject(param, "id", boardId);
			 
	route_call("ai_closeBoard", param, &result, NULL);
	ASSERT(cJSON_GetObjectItem(result, "id")->valueint == boardId);
	ASSERT_LIKE(cJSON_GetObjectItem(result, "status")->valuestring, "deleted");
	
	cJSON_Delete(param);
	cJSON_Delete(result);
}

void testAIRequest(){
	FILE *f;
	long len;
	char *text;
	f=fopen("test/12-pipewalker.jsonraw.json","rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	text=(char*)malloc(len+1);fread(text,1,len,f);fclose(f);
	
	cJSON *result = handler_raw(text);
	if(result){
		//~ printf("%d\n", cJSON_GetObjectItem(result, "id")->valueint);
		//~ 
		//~ char *out = cJSON_Print(result);
					//~ printf("%s\n",out);
					//~ free(out);
	}else{
		printf("waw parsed error!\n");
	}
	
	cJSON_Delete(result);
	free(text);
}

void TEST_ROUTE(){
	route_add("ai_test", 		method_ai_test);
	route_add("ai_newBoard", 	method_ai_newBoard);
	route_add("ai_closeBoard", 	method_ai_closeBoard);
	route_add("ai_getBoard", 	method_ai_getBoard);
	route_add("ai_A*", 			method_ai_AStar);
	
	
	//~ TEST_CALL("test AI algo   - BFS",				testBoardBFS);
	//~ TEST_CALL("test AI algo   - A*",				testBoardAStar);
	//~ TEST_CALL("test AI method - test", 				testAIMethodTest);
	//~ TEST_CALL("test AI method - new board", 		testAINewBoard);
	//~ TEST_CALL("test AI method - get board", 		testAIGetBoard);
	TEST_CALL("test AI method - A*", 				testAIAStar);
	TEST_CALL("test AI method - Request JSON raw", 	testAIRequest);
}
