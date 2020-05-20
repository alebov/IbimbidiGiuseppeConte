#pragma once

#include "board.h"

#define MAX_DEPTH 20
#define WHITE_TURN 0
#define BLACK_TURN 1
#define WHITE_WIN_TURN 2
#define BLACK_WIN_TURN 3
#define DRAW_TURN 4
#define STATE_BUFFER_SIZE 2000
#define MOVE_BUFFER_SIZE 100
#define WHITE_SERVER_PORT 5800
#define BLACK_SERVER_PORT 5801
#define h_addr h_addr_list[0]

#define PLAYER_NAME "IbimbidiGiuseppeConte"

char columnToServerFormat(uint32_t column);
uint32_t rowToServerFormat(uint32_t row);
void gameLoop(int playerColor, int maxTime, int numThreads, char* server_ip);
