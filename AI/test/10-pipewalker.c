#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "../lib/unitTesting/lib/unitTesting.h"
#include "../src/models/config.h"
#include "../src/helpers/handler.h"
#include "../src/models/route.h"

#include "../src/models/pipewalker/board.h"
#include "../src/models/pipewalker/tile.h"

cJSON *boardJSON;
board_struct *board;

int method_ping(cJSON *params, cJSON **result, char *error_msg){
	*result = cJSON_CreateString("ok");
	return 0;
}

void testReadJSON(){
	FILE *f;
	long len;
	char *text;
	
	f=fopen("test/10-pipewalker.3x3.json","rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	text=(char*)malloc(len+1);fread(text,1,len,f);fclose(f);
	
	boardJSON=cJSON_Parse(text);
	if (!boardJSON) {
		ASSERT(0);
	}else{
		ASSERT(1);
	}
	free(text);
}

void testBoardCreate(){
	board = board_create(3);
}

void testBoardDestroy(){
	board_destroy(board);
}

void testBoardData(){
	short int row_i;
	short int cell_i;
	
	char str[64];
	
	for(row_i = 0; row_i < board->size; row_i++){
		//~ printf("\n");
			
		for(cell_i = 0; cell_i < board->size; cell_i++){
			ASSERT(board_get_tile(board, row_i, cell_i) != NULL);
			
			memset(str, 0, 64);
			tile_to_string(str, board_get_tile(board, row_i, cell_i));
			
			//~ printf("\t[%d][%d] -> %s\n", row_i, cell_i, str);
		}
	}
}

void testBoardParse(){
	board_parseJSON(board, boardJSON);
	
	short int row_i;
	short int cell_i;
	
	for(row_i = 0; row_i < board->size; row_i++){
		for(cell_i = 0; cell_i < board->size; cell_i++){
			ASSERT(board_get_tile(board, row_i, cell_i) != NULL);
			
			if(board_get_tile(board, row_i, cell_i)->type != TILE_BLANK)
				ASSERT(1);
		}
	}
}

void testBoardReconnect(){
	FILE *f;
	long len;
	char *text;
	f=fopen("test/10-pipewalker.4x4.json","rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	text=(char*)malloc(len+1);fread(text,1,len,f);fclose(f);
	cJSON *json=cJSON_Parse(text);
	
	
	board_struct *testBoard = board_create(4);
	board_parseJSON(testBoard, json);
	board_reconnectAll(testBoard);
	
	ASSERT(board_get_tile(testBoard, 0, 0)->active ==  TILE_INACTIVE);
	ASSERT(board_get_tile(testBoard, 0, 1)->active ==  TILE_INACTIVE);
	ASSERT(board_get_tile(testBoard, 0, 2)->active ==  TILE_ACTIVE);
	ASSERT(board_get_tile(testBoard, 0, 3)->active ==  TILE_INACTIVE);
	
	ASSERT(board_get_tile(testBoard, 1, 0)->active ==  TILE_INACTIVE);
	ASSERT(board_get_tile(testBoard, 1, 1)->active ==  TILE_INACTIVE);
	ASSERT(board_get_tile(testBoard, 1, 2)->active ==  TILE_ACTIVE);
	ASSERT(board_get_tile(testBoard, 1, 3)->active ==  TILE_INACTIVE);
	
	ASSERT(board_get_tile(testBoard, 2, 0)->active ==  TILE_INACTIVE);
	ASSERT(board_get_tile(testBoard, 2, 1)->active ==  TILE_ACTIVE);
	ASSERT(board_get_tile(testBoard, 2, 2)->active ==  TILE_ACTIVE);
	ASSERT(board_get_tile(testBoard, 2, 3)->active ==  TILE_ACTIVE);
	
	ASSERT(board_get_tile(testBoard, 3, 0)->active ==  TILE_INACTIVE);
	ASSERT(board_get_tile(testBoard, 3, 1)->active ==  TILE_ACTIVE);
	ASSERT(board_get_tile(testBoard, 3, 2)->active ==  TILE_ACTIVE);
	ASSERT(board_get_tile(testBoard, 3, 3)->active ==  TILE_ACTIVE);
	
	
	//~ cJSON_Delete(json);
	//~ json = board_JSON(testBoard);
	//~ char *out=cJSON_Print(json);
			  //~ printf("%s\n",out);
			  //~ free(out);
	
	cJSON_Delete(json);
	board_destroy(testBoard);
	free(text);
}

void testBoardEvaluate(){
	board_evaluator(board);
	ASSERT(board->result > 0);
	ASSERT(board->pipeConnected > 0);
	ASSERT(board->clientConnected == 0);
	
}

void testBoardClone(){
	board_struct *newBoard = board_clone(board);
	
	ASSERT(board->size == newBoard->size);
	
	short int row_i;
	short int cell_i;
	for(row_i = 0; row_i < board->size; row_i++){
		for(cell_i = 0; cell_i < board->size; cell_i++){
			ASSERT(board_get_tile(board, row_i, cell_i)->type == board_get_tile(newBoard, row_i, cell_i)->type);
			ASSERT(board_get_tile(board, row_i, cell_i)->active == board_get_tile(newBoard, row_i, cell_i)->active);
			ASSERT(board_get_tile(board, row_i, cell_i)->direction == board_get_tile(newBoard, row_i, cell_i)->direction);
		}
	}
	
	
	board_destroy(newBoard);
}

void testBoardJSON(){
	cJSON *json = board_JSON(board);
	
	//~ char *out = cJSON_PrintUnformatted(json);
	//~ printf("%s\n",out);
	//~ free(out);
	
	board_struct *newBoard = board_create(board->size);
	board_parseJSON(newBoard, json);
	
	short int row_i;
	short int cell_i;
	for(row_i = 0; row_i < board->size; row_i++){
		for(cell_i = 0; cell_i < board->size; cell_i++){
			ASSERT(board_get_tile(board, row_i, cell_i)->type == board_get_tile(newBoard, row_i, cell_i)->type);
			ASSERT(board_get_tile(board, row_i, cell_i)->active == board_get_tile(newBoard, row_i, cell_i)->active);
			ASSERT(board_get_tile(board, row_i, cell_i)->direction == board_get_tile(newBoard, row_i, cell_i)->direction);
		}
	}
	
	board_destroy(newBoard);
	cJSON_Delete(json);
}

void testBoardSource(){
	short int *coor = board_get_source_coordinate(board);
	ASSERT(0 == *coor);
	ASSERT(2 == *(coor + 1));
	
	free(coor);
}

void testTileBranches(){
	ASSERT(tile_has_up(		TILE_SOURCE, TILE_UP));
	ASSERT(tile_has_right(	TILE_SOURCE, TILE_RIGHT));
	ASSERT(tile_has_down(	TILE_SOURCE, TILE_DOWN));
	ASSERT(tile_has_left(	TILE_SOURCE, TILE_LEFT));
	
	ASSERT(tile_has_up(		TILE_CLIENT, TILE_UP));
	ASSERT(tile_has_right(	TILE_CLIENT, TILE_RIGHT));
	ASSERT(tile_has_down(	TILE_CLIENT, TILE_DOWN));
	ASSERT(tile_has_left(	TILE_CLIENT, TILE_LEFT));
	
	ASSERT(tile_has_up(		TILE_PIPE_1, TILE_UP));
	ASSERT(tile_has_down(	TILE_PIPE_1, TILE_UP));
	ASSERT(tile_has_right(	TILE_PIPE_1, TILE_RIGHT));
	ASSERT(tile_has_left(	TILE_PIPE_1, TILE_RIGHT));
	ASSERT(tile_has_up(		TILE_PIPE_1, TILE_DOWN));
	ASSERT(tile_has_down(	TILE_PIPE_1, TILE_DOWN));
	ASSERT(tile_has_right(	TILE_PIPE_1, TILE_LEFT));
	ASSERT(tile_has_left(	TILE_PIPE_1, TILE_LEFT));
	
	ASSERT(tile_has_up(		TILE_PIPE_2, TILE_UP));
	ASSERT(tile_has_right(	TILE_PIPE_2, TILE_UP));
	ASSERT(tile_has_right(	TILE_PIPE_2, TILE_RIGHT));
	ASSERT(tile_has_down(	TILE_PIPE_2, TILE_RIGHT));
	ASSERT(tile_has_down(	TILE_PIPE_2, TILE_DOWN));
	ASSERT(tile_has_left(	TILE_PIPE_2, TILE_DOWN));
	ASSERT(tile_has_left(	TILE_PIPE_2, TILE_LEFT));
	ASSERT(tile_has_up(		TILE_PIPE_2, TILE_LEFT));
	
	ASSERT(tile_has_up(		TILE_PIPE_3, TILE_UP));
	ASSERT(tile_has_right(	TILE_PIPE_3, TILE_UP));
	ASSERT(tile_has_down(	TILE_PIPE_3, TILE_UP));
	ASSERT(tile_has_right(	TILE_PIPE_3, TILE_RIGHT));
	ASSERT(tile_has_down(	TILE_PIPE_3, TILE_RIGHT));
	ASSERT(tile_has_left(	TILE_PIPE_3, TILE_RIGHT));
	ASSERT(tile_has_down(	TILE_PIPE_3, TILE_DOWN));
	ASSERT(tile_has_left(	TILE_PIPE_3, TILE_DOWN));
	ASSERT(tile_has_up(		TILE_PIPE_3, TILE_DOWN));
	ASSERT(tile_has_left(	TILE_PIPE_3, TILE_LEFT));
	ASSERT(tile_has_up(		TILE_PIPE_3, TILE_LEFT));
	ASSERT(tile_has_right(	TILE_PIPE_3, TILE_LEFT));
}

void TEST_ROUTE(){
	route_add("ping", method_ping);
	
	TEST_CALL("test read json", testReadJSON);
	TEST_CALL("test create board", testBoardCreate);
	TEST_CALL("test parse", testBoardParse);
		//TEST_CALL("test board\'s data test", testBoardData);
	
	TEST_CALL("test source coordinate", testBoardSource);
	TEST_CALL("test tile branches", testTileBranches);
	TEST_CALL("test reconnect", testBoardReconnect);
		TEST_CALL("test board\'s data test", testBoardData);
	TEST_CALL("test evaluator", testBoardEvaluate);
	
	TEST_CALL("test JSON of board\'s data", testBoardJSON);
	TEST_CALL("test clone board", testBoardClone);
	TEST_CALL("test destroy board", testBoardDestroy);
}
