#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "../lib/unitTesting/lib/unitTesting.h"
#include "../src/models/config.h"
#include "../src/helpers/handler.h"
#include "../src/models/route.h"

#include "../src/helpers/map.h"

void testAdd(){
	map_item *start = map_item_add(NULL, "1");
					  start->valueInt1 = 1;
					  start->valueInt2 = 2;
					  start->valueInt3 = 3;
	
	ASSERT(start->valueInt1 == 1);
	ASSERT(start->valueInt2 == 2);
	ASSERT(start->valueInt3 == 3);
	map_item_delete(start);
}

void testGet(){
	map_item *start;
	map_item *tmp;
	
	start = map_item_add(NULL, "1");
			start->valueInt1 = 1;
			start->valueInt2 = 2;
			start->valueInt3 = 3;
	
	tmp =	map_item_add(start, "2");
			tmp->valueInt1 = 4;
			tmp->valueInt2 = 5;
			tmp->valueInt3 = 6;
	
	tmp =	map_item_add(start, "3");
			tmp->valueInt1 = 7;
			tmp->valueInt2 = 8;
			tmp->valueInt3 = 9;
	
	tmp = 	map_item_get(start, "1");
			ASSERT(tmp != NULL);
			ASSERT(tmp->valueInt1 == 1);
			ASSERT(tmp->valueInt2 == 2);
			ASSERT(tmp->valueInt3 == 3);
	
	tmp = 	map_item_get(start, "2");
			ASSERT(tmp != NULL);
			ASSERT(tmp->valueInt1 == 4);
			ASSERT(tmp->valueInt2 == 5);
			ASSERT(tmp->valueInt3 == 6);
	
	tmp = 	map_item_get(start, "3");
			ASSERT(tmp != NULL);
			ASSERT(tmp->valueInt1 == 7);
			ASSERT(tmp->valueInt2 == 8);
			ASSERT(tmp->valueInt3 == 9);
	
	// test backward
	tmp = 	map_item_get(tmp, "2");
			ASSERT(tmp != NULL);
			ASSERT(tmp->valueInt1 == 4);
			ASSERT(tmp->valueInt2 == 5);
			ASSERT(tmp->valueInt3 == 6);
	
	map_item_delete(start);
}

void testNotFound(){
	map_item *start;
	map_item *tmp;
	
	start = map_item_add(NULL, "1");
			start->valueInt1 = 1;
			start->valueInt2 = 2;
			start->valueInt3 = 3;
	
	tmp =	map_item_add(start, "2");
			tmp->valueInt1 = 4;
			tmp->valueInt2 = 5;
			tmp->valueInt3 = 6;
	
	// use get
	tmp = 	map_item_get(start, "3");
			ASSERT(tmp == NULL);
	
	// use exists
	ASSERT(map_item_exists(start, "1") == 1);
	ASSERT(map_item_exists(start, "2") == 1);
	ASSERT(map_item_exists(start, "3") == 0); // not found
	
	map_item_delete(start);
}

void testRemove(){
	map_item *start;
	map_item *tmp;
	short int remove;
	
	start = map_item_add(NULL, "1");
			start->valueInt1 = 1;
			start->valueInt2 = 2;
			start->valueInt3 = 3;
	
	tmp =	map_item_add(start, "2");
			tmp->valueInt1 = 4;
			tmp->valueInt2 = 5;
			tmp->valueInt3 = 6;
	
	// use get
	tmp = 	map_item_get(start, "3");
			ASSERT(tmp == NULL);
	
	//remove
	remove = map_item_remove(start, "2");
	ASSERT(remove == 1);
	remove = map_item_remove(start, "2");
	ASSERT(remove == 0);
	
	// use exists
	ASSERT(map_item_exists(start, "1") == 1);
	ASSERT(map_item_exists(start, "2") == 0);
	ASSERT(map_item_exists(start, "3") == 0); // not found
	
	map_item_delete(start);
}

void TEST_ROUTE(){
	TEST_CALL("test add", testAdd);
	TEST_CALL("test get", testGet);
	TEST_CALL("test not found", testNotFound);
	TEST_CALL("test remove", testRemove);
}
