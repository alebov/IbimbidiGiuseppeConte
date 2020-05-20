#pragma once
#include "board.h"

typedef uint64_t Hash; 

Hash stringHash(BoardState* board);
Hash zobrist(BoardState* board);
void initZobrist();