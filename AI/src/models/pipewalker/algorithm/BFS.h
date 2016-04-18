#ifndef _BFS_H
#define _BFS_H

#include "../../../../lib/cJSON/cJSON.h"

#include "../tile.h"
#include "../board.h"


void AI_BFS_run(board_struct *testBoard, void (*each_move_callback)(board_struct *), int);
void AI_BFS(board_struct *testBoard, void (*each_move_callback)(board_struct *));


#endif
