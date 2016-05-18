#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <resolv.h>	
#include <signal.h>	

#include "models/config.h"
#include "models/route.h"

#include "helpers/signal.h"
#include "helpers/handler.h"
#include "helpers/wrapper.h"

#include "methods/util.h"
#include "methods/AI.h"

#include "childs/http.h"
#include "childs/dataRefresher.h"
#include "../lib/safeTo/safeTo.h"

int test();


int main(){
	handle_signal();
	
	config_init("AI/config.json");
	
	/*
	// example
	category_init();
	post_init();
	*/
	
	// start refresher
	// refresher_init();
	// printf("refresher init...\n");
	
	// route method add
	route_add("ping", 					method_ping);
	route_add("ai_test", 				method_ai_test);
	route_add("ai_newBoard", 			method_ai_newBoard);
	route_add("ai_closeBoard", 			method_ai_closeBoard);
	route_add("ai_getBoard", 			method_ai_getBoard);
	route_add("ai_A*", 					method_ai_AStar);
	route_add("ai_BFS", 					method_ai_BFS);
	
	printf("route added...\n");
	
	
	
	
	// http api server
	int tmp_param;
	printf("start service...\n");
	safeFree(http_multi_thread(&tmp_param));
	
	//raise(SIGINT);
	
	return 0;
}
