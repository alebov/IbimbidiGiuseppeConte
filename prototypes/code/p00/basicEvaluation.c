#pragma once
#include "board.h"
#include "basicEvaluation.h"

int blackGoalTest(BoardState board){
    int i,j;
    for(i=0;i<9;i++)
        for(j=0;j<9;j++)
            if(GET_CELL_STATE(board.Board,i,j)==WHITE_KING)
                return 0;
    return 1;
}


int whiteGoalTest(BoardState board){
    return
            ((board.Board[1] & 0b110000000000000000U) == 0b110000000000000000U || ((board.Board[1] & 0b000000000000000011U) == 0b000000000000000011U)) ||
            ((board.Board[2] & 0b110000000000000000U) == 0b110000000000000000U || ((board.Board[2] & 0b000000000000000011U) == 0b000000000000000011U)) ||
            ((board.Board[6] & 0b110000000000000000U) == 0b110000000000000000U || ((board.Board[6] & 0b000000000000000011U) == 0b000000000000000011U)) ||
            ((board.Board[7] & 0b110000000000000000U) == 0b110000000000000000U || ((board.Board[7] & 0b000000000000000011U) == 0b000000000000000011U)) ||
            ((board.Board[0] & 0b001100000000000000U) == 0b001100000000000000U || ((board.Board[0] & 0b000011000000000000U) == 0b000011000000000000U)) ||
            ((board.Board[0] & 0b000000000000110000U) == 0b000000000000110000U || ((board.Board[0] & 0b000000000000001100U) == 0b000000000000001100U)) ||
            ((board.Board[8] & 0b001100000000000000U) == 0b001100000000000000U || ((board.Board[8] & 0b000011000000000000U) == 0b000011000000000000U)) ||
            ((board.Board[8] & 0b000000000000110000U) == 0b000000000000110000U || ((board.Board[8] & 0b000000000000001100U) == 0b000000000000001100U));
}

int evaluation(BoardState board){
    int i,j;
    int cell;
    int value=0;
     for(i=0;i<9;i++)
        for(j=0;j<9;j++){
            cell=GET_CELL_STATE(board.Board,i,j);
            if(cell==WHITE_KING || cell==WHITE)
                value++;
            if(cell==BLACK)
                value--;
        }
    return value;
}
