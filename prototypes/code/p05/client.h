#pragma once

#include "board.h"

//---------CONFIGURATION------------------
#define PLAYER_NAME "Tablut"
#define MAX_DEPTH 20
#define SERVER_IP "localhost"
//----------------------------------------
#define WHITE_TURN 0
#define BLACK_TURN 1
#define WHITE_WIN_TURN 2
#define BLACK_WIN_TURN 3
#define DRAW_TURN 4


char columnToServerFormat(uint32_t column);
uint32_t rowToServerFormat(uint32_t row);
void gameLoop(int playerColor, int maxTime, int numThreads);
