#pragma once
#include "board.h"
#include "basicEvaluation.h"

#define ABS(x) ((x)<0 ? -(x) : (x))

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
    int value=-72;
    int iKing=0,jKing=0;
     for(i=0;i<9;i++)
        for(j=0;j<9;j++){
            cell=GET_CELL_STATE(board.Board,i,j);
            if(cell==WHITE_KING){
                iKing=i;
                jKing=j;
            }
            if(cell==WHITE)
                value+=30;
            if(cell==BLACK)
                value-=20;
        }
    //bonus per vicinanza al re per il nero
    
    for(i=0;i<9;i++)
        for(j=0;j<9;j++){
            cell=GET_CELL_STATE(board.Board,i,j);
            if(cell==BLACK && (ABS(i-iKing) <= 3 && ABS(j-jKing) <= 3))
                value-= 2*(8-(ABS(i-iKing) + ABS(j-jKing)));
            if(cell==EMPTY){
                 if(i==1 || i==7)
                    value+=2;
                if(i==2 || i==6)
                    value+=4;
                if (j==1 ||  j==7)
                    value+=2;
                if(j==2 || j==6 )
                    value+=4;
            }
        }

    return value;
}
