#ifndef _AStar_H
#define _AStar_H

#include "../../../../lib/cJSON/cJSON.h"

#include "../tile.h"
#include "../board.h"


void AI_AStar_run(board_struct *testBoard, void (*each_move_callback)(board_struct *), int);
void AI_AStar(board_struct *testBoard, void (*each_move_callback)(board_struct *));


#endif
