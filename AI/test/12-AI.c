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

void TEST_ROUTE(){
	TEST_CALL("test BFS", testBoardBFS);
	TEST_CALL("test A*", testBoardAStar);
}
