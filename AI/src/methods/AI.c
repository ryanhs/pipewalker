#include <stdio.h>
#include <stdlib.h>
#include "../../lib/cJSON/cJSON.h"
#include "../helpers/handler.h"

#include "AI.h"

int method_ai(cJSON *params, cJSON **result, char *error_msg){
	*result = cJSON_CreateString("test AI debug");
	return 0;
}

int method_AStar(cJSON *params, cJSON **result, char *error_msg){
	*result = cJSON_CreateString("test AI debug");
	return 0;
}
