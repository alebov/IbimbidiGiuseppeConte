#include "newEvaluation.h"
#include "moves.h"

#include <string.h>



//chiamata da max, ha appena mosso il nero 
Score newEvaluateForBlack(BoardState board) {

Score result = START_VALUE_FOR_BLACK;
Coordinate kingCoordinate;
//controllo se il re è stato catturato -> black win
if(isKingCaptured(board))
	return BLACK_WIN;

kingCoordinate = findKing(board);
result -= kingInDanger(board,kingCoordinate);

//Bonus bianco per ogni pedone ancora in vita
result += pawnAlive(board,WHITE)*500;

return result;



}

//chiamata da min. ha appena mosso il bianco 
Score newEvaluateForWhite(BoardState board) {

	Score result = START_VALUE_FOR_WHITE;
	Coordinate kingCoordinate;


	if(throneState(board) == EMPTY ){
		kingCoordinate = findKing(board);
		if(isWhiteWin(kingCoordinate))
			return WHITE_WIN;
		if(isCheck(board,kingCoordinate)<4)
			return ALMOST_WHITE_WIN;
		//Bonus prossimità corner
		result+=(abs(kingCoordinate.row-4)+abs(kingCoordinate.column-4))*500;

	}

	//Bonus per ogni pedone nero in meno
	result += (16-pawnAlive(board,BLACK))*500;


	return result;

}



int pawnAlive(BoardState board,int color){
	int pawn,row,column;
	pawn = 0;
	for (row = 0; row < 9; row++)
    	for (column = 0; column < 9; column++)
            if(GET_CELL_STATE(board.Board, row, column)==color)
            	pawn++;

    return pawn;
}


Coordinate findKing(BoardState board){
	bool found;
	int rowKing, columnKing;
	Coordinate coordinate;
	found=false;

	for (rowKing = 0; rowKing < 9 && !found; rowKing++)
    	for (columnKing = 0; columnKing < 9 && !found; columnKing++)
            if(GET_CELL_STATE(board.Board, rowKing, columnKing)==WHITE_KING){
            	found = true;
            	coordinate.row=rowKing;
            	coordinate.column=columnKing;
            }
    return coordinate;
}



Score kingInDanger(BoardState board,Coordinate kingCoordinate){
	Score result = 0;


	if(GET_CELL_STATE(board.Board, kingCoordinate.row+1, kingCoordinate.column)==BLACK)
		if(isThroneNear(kingCoordinate) || throneState(board)==WHITE_KING)
			result+=5000;
		else 
			result+=10000;

	if(GET_CELL_STATE(board.Board, kingCoordinate.row-1, kingCoordinate.column)==BLACK)
		if(isThroneNear(kingCoordinate) || throneState(board)==WHITE_KING)
			result+=5000;
		else 
			result+=10000;

	if(GET_CELL_STATE(board.Board, kingCoordinate.row, kingCoordinate.column+1)==BLACK)
		if(isThroneNear(kingCoordinate) || throneState(board)==WHITE_KING)
			result+=5000;
		else 
			result+=10000;

	if(GET_CELL_STATE(board.Board, kingCoordinate.row, kingCoordinate.column-1)==BLACK)
		if(isThroneNear(kingCoordinate) || throneState(board)==WHITE_KING)
			result+=5000;
		else 
			result+=10000;

}


bool isThroneNear(Coordinate kingCoordinate){
	if((kingCoordinate.row == 4 && kingCoordinate.column==5) || (kingCoordinate.row == 5 && kingCoordinate.column==4)|| (kingCoordinate.row == 4 && kingCoordinate.column==3)||(kingCoordinate.row == 3 && kingCoordinate.column==4) )
		return true;
}


bool isKingCaptured(BoardState board){
	bool kingDeath = true;
	int i,j;
	
    for (i = 0; i < 9 && kingDeath; i++)
        for (j = 0; j < 9 && kingDeath; j++)
            if(GET_CELL_STATE(board.Board, i, j)==WHITE_KING)
            	kingDeath = false;

    return kingDeath;
}

int throneState(BoardState board){
	return GET_CELL_STATE(board.Board,4,4);
}

bool isWhiteWin(Coordinate kingCoordinate){
	if (kingCoordinate.row == 0 || kingCoordinate.row == 8 || kingCoordinate.column == 0 || kingCoordinate.column == 8) //board limits
       return true;
   else
   	return false;
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


int isCheck(BoardState board, Coordinate kingCoordinate){
	int i,j;
	int pawn = 0;

	for (i = 0; i < kingCoordinate.row; i++)
		 if(GET_CELL_STATE(board.Board, i, kingCoordinate.column)!=EMPTY){
		 	pawn++;
		 	break;
		 }

	for (i = kingCoordinate.row+1; i < 9; i++)
		 if(GET_CELL_STATE(board.Board, i, kingCoordinate.column)!=EMPTY){
		 	pawn++;
		 	break;
		 }

    for (j = 0; j < kingCoordinate.column; j++)
		 if(GET_CELL_STATE(board.Board, kingCoordinate.row, j)!=EMPTY){
		 	pawn++;
		 	break;
		 }

	for (j = kingCoordinate.column+1; j < 9; j++)
		 if(GET_CELL_STATE(board.Board, kingCoordinate.row, j)!=EMPTY){
		 	pawn++;
		 	break;
		 }



	return pawn;
}


 int abs(int i)
 {
     if (i < 0)
         return -i;
     else
         return i;
 }

