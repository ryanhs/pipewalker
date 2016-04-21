#ifndef _METHOD_AI_H
#define _METHOD_AI_H

#include "../../lib/cJSON/cJSON.h"

int method_ai(cJSON *params, cJSON **result, char *error_msg);
int method_AStar(cJSON *params, cJSON **result, char *error_msg);


#endif
