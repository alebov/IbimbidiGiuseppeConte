#pragma once
#include "board.h"

typedef uint64_t Hash; 

Hash hash(BoardState board);
Hash zobrist(BoardState* board);