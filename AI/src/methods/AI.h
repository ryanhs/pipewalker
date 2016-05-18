#ifndef _METHOD_AI_H
#define _METHOD_AI_H

#include "../../lib/cJSON/cJSON.h"

int method_ai_test(cJSON *params, cJSON **result, char *error_msg);
int method_ai_newBoard(cJSON *params, cJSON **result, char *error_msg);
int method_ai_closeBoard(cJSON *params, cJSON **result, char *error_msg);
int method_ai_getBoard(cJSON *params, cJSON **result, char *error_msg);
	
	
int method_ai_AStar(cJSON *params, cJSON **result, char *error_msg);
int method_ai_BFS(cJSON *params, cJSON **result, char *error_msg);


#endif
