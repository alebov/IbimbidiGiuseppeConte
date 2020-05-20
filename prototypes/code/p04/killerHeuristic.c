#pragma once

#include "killerHeuristic.h"
#include "client.h"
#include "stdio.h"
KillerMoves table [MAX_DEPTH][2];


void printKiller(){
    int i;
    int j;
    printf("-------WHITE------\n");
    for(i=0;i<MAX_DEPTH;i++){
        if(table[i][0].dim){
               printf("%d | ",i);
                for(j=0; j<table[i][WHITE_TURN].dim;j++){
                    printf(" %d ",j);
                    printMove(table[i][WHITE_TURN].moves[j]);
                    printf(" | ");
                }
                printf(" \n");
        }   
    }
    printf("-------BLACK------\n");
    for(i=0;i<MAX_DEPTH;i++){
        if(table[i][0].dim){
            printf("%d | ",i);
            for(j=0; j<table[i][BLACK_TURN].dim;j++){
                printf(" %d ",j);
                printMove(table[i][BLACK_TURN].moves[j]);
                printf(" | ");
            }
            printf(" \n");
        }
    }
}

//inizialize the table
void initKillerMoves(){
    int i,j;
    for(i=0;i<MAX_DEPTH;i++)
        for(j=0;j<2;j++){
            table[i][j].index=0;
            table[i][j].dim=0;
        }
}


//add a killer move to the table
void addKillerMove(Move move, int depth, int colour){
    KillerMoves* killers= &table[depth][colour];
    killers->moves[killers->index]=move;
    killers->index++;
    if(killers->index==WIDTH)
        killers->index=0;
    if(killers->dim!=WIDTH)
        killers->dim++;
}

//a killer move is calculated in a previus board and it can be invalid in the current board
//this function return true if the move is valid in the board
int isKillerMoveValid(Move move, BoardState board){
    int target_column=TARGET_COLUMN(move);
    int start_column=START_COLUMN(move);
    int start_row= START_ROW(move);
    int target_row=TARGET_ROW(move);
    //column movement
    if(target_column==start_column){
        if(start_row>target_row)
            for(start_row--;start_row>=target_row;start_row--)
                if(GET_CELL_STATE(board.Board,start_row,start_column) != EMPTY)
                    return 0;
        if(start_row<target_row)
            for(start_row++;start_row<=target_row;start_row++)
                if(GET_CELL_STATE(board.Board,start_row,start_column) != EMPTY)
                    return 0;   
    }else{
    //row movement
        if(start_column>target_column)
            for(start_column--;start_column>=target_column;start_column--)
                if(GET_CELL_STATE(board.Board,start_row,start_column) != EMPTY)
                    return 0;
        if(start_column<target_column)
            for(start_column++;start_column<=target_column;start_column++)
                if(GET_CELL_STATE(board.Board,start_row,start_column) != EMPTY)
                    return 0; 
    }
    return 1;
}  

Move* getKillerMoves(int depth, int colour ,int* dim){
    *dim=table[depth][colour].dim;
    return table[depth][colour].moves;
}

int isKiller(int depth, Move move, int colour) {
    int i=0;
    for(i;i<table[depth][colour].dim;i++){
        if(table[depth][colour].moves[i]==move)
            return 1;
    }
    return 0;
}