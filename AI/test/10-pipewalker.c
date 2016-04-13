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
		printf("\n");
			
		for(cell_i = 0; cell_i < board->size; cell_i++){
			ASSERT(board_get_tile(board, row_i, cell_i) != NULL);
			
			memset(str, 0, 64);
			tile_to_string(str, board_get_tile(board, row_i, cell_i));
			
			printf("\t[%d][%d] -> %s\n", row_i, cell_i, str);
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
	short int row_i;
	short int cell_i;
	
	short int source_row;
	short int source_cell;
	for(row_i = 0; row_i < board->size; row_i++){
		for(cell_i = 0; cell_i < board->size; cell_i++){
			if(board_get_tile(board, row_i, cell_i)->type == TILE_SOURCE){
				source_row = row_i;
				source_cell = cell_i;
				break;
				break;
			}
		}
	}
	
		
}

void TEST_ROUTE(){
	route_add("ping", method_ping);
	
	TEST_CALL("test read json", testReadJSON);
	TEST_CALL("test create board", testBoardCreate);
	TEST_CALL("test parse", testBoardParse);
	TEST_CALL("test board\'s data test", testBoardData);
	TEST_CALL("test reconnect", testBoardReconnect);
	
	//TEST_CALL("invalid json", testInvalidJSON);
	
	TEST_CALL("test destroy board", testBoardDestroy);
}
