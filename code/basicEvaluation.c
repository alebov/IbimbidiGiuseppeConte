#pragma once
#include "board.h"
#include "basicEvaluation.h"

#define ABS(x) ((x)<0 ? -(x) : (x))


Coordinate findKing(BoardState board){
	boolean found;
	int rowKing, columnKing;
	Coordinate coordinate;
	found=0;

	for (rowKing = 0; rowKing < 9 && !found; rowKing++)
    	for (columnKing = 0; columnKing < 9 && !found; columnKing++)
            if(GET_CELL_STATE(board.Board, rowKing, columnKing)==WHITE_KING){
            	found = 1;
            	coordinate.row=rowKing;
            	coordinate.column=columnKing;
            }
    return coordinate;
}


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


/*

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
    value+=2*ABS(5-iKing);
    value+=2*ABS(5-jKing);
    return value;
}

*/



//chiamata da max, ha appena mosso il nero 
ScoreType evaluateForBlack(BoardState board) {
	ScoreType result = START_VALUE_FOR_BLACK;
	Coordinate kingCoordinate;
	kingCoordinate.column=0;
	kingCoordinate.row=0;
	kingCoordinate = findKing(board);
	//controllo se il re è stato catturato -> black win
	if(isKingCaptured(kingCoordinate))
		return BLACK_WIN;
	//lo vorrei mettere ma mi dava segmentation fault ogni tanto e quando l'ho cavata non me l'ha mai data. Credo sia colpa di questa funzione ma non capisco perchè
	/*if(throneState(board) == EMPTY )
		if(isCheck(board,kingCoordinate))
			return ALMOST_WHITE_WIN;*/
	result+=(abs(kingCoordinate.row-4)+abs(kingCoordinate.column-4));
	result += pawnAlive(board,kingCoordinate);
	return result;
}

//chiamata da min. ha appena mosso il bianco 
ScoreType evaluateForWhite(BoardState board) {
	ScoreType result = START_VALUE_FOR_WHITE;
	Coordinate kingCoordinate;
	kingCoordinate = findKing(board);
	if(throneState(board) == EMPTY ){
		if(isWhiteWin(kingCoordinate))
			return WHITE_WIN;
	}
	result+=(abs(kingCoordinate.row-4)+abs(kingCoordinate.column-4));
	result += pawnAlive(board,kingCoordinate);
	return result;

}


//al momento non usata che ho messo una roba simile in pawn alive
ScoreType checkDiagonal(BoardState board) {
	ScoreType result = 0;
	int i;

	if(GET_CELL_STATE(board.Board, 2, 1)==BLACK )
		result += 500;
	if(GET_CELL_STATE(board.Board, 1, 6)==BLACK )
		result += 500;
	if(GET_CELL_STATE(board.Board, 6, 1)==BLACK )
		result += 500;
	if(GET_CELL_STATE(board.Board, 6, 7)==BLACK )
		result += 500;
	if(GET_CELL_STATE(board.Board, 1, 2)==BLACK )
		result += 500;
	if(GET_CELL_STATE(board.Board, 2, 7)==BLACK )
		result += 500;
	if(GET_CELL_STATE(board.Board, 7, 2)==BLACK)
		result += 500;
	if(GET_CELL_STATE(board.Board, 7, 6)==BLACK )
		result += 500;


	return result;

}



int pawnAlive(BoardState board, Coordinate kingCoordinate){
	int i,j;
	int cell;
	int value=0;
	int lastCellInColumn[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int lastCell=0;
	for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            cell=GET_CELL_STATE(board.Board,i,j);
			//BONUS NERO	pedina + vicinanza al re
			if(cell==BLACK){
				value-=20;
				if((ABS(i-kingCoordinate.row) <= 3 && ABS(j-kingCoordinate.column) <= 3))
                	value-= 2*(8-(ABS(i-kingCoordinate.row) + ABS(j-kingCoordinate.column)));
			}
            //BONUS BIANCO pedina + pedine non adiacenti
			if(cell==WHITE){
				value+=30;
				//bonus per pedine del bianco non adiacenti, non sono sicuro sia utile farlo anche per il re
                /*
				if(lastCell!=WHITE || lastCell!=WHITE_KING)
					value+=5;
				if(lastCellInColumn[j]!=WHITE || lastCellInColumn[j] !=WHITE_KING)
					value+=5;
				*/
			}
            /*
			lastCellInColumn[j] = cell;
            lastCell = cell;
            */
			//questa parte non so quanto sia utile
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
    }
    

    return value;
}




//al momento non usata perchè do già il bonus per la vicinanza
ScoreType kingInDanger(BoardState board,Coordinate kingCoordinate){
	ScoreType result = 0;


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


boolean isThroneNear(Coordinate kingCoordinate){
	if((kingCoordinate.row == 4 && kingCoordinate.column==5) || (kingCoordinate.row == 5 && kingCoordinate.column==4)|| (kingCoordinate.row == 4 && kingCoordinate.column==3)||(kingCoordinate.row == 3 && kingCoordinate.column==4) )
		return 1;
}


boolean isKingCaptured(Coordinate coordinate){
    return coordinate.row==0 && coordinate.column==0 ? 1 : 0;
}

int throneState(BoardState board){
	return GET_CELL_STATE(board.Board,4,4);
}

boolean isWhiteWin(Coordinate kingCoordinate){
	if (kingCoordinate.row == 0 || kingCoordinate.row == 8 || kingCoordinate.column == 0 || kingCoordinate.column == 8) //board limits
       return 1;
   else
   	return 0;
}



//modifica: se controllo solo che sia minore di quattro non basta fare return appena trova una riga/colonna libera
//modifica: lo metterei in evaluate for black, se ha appena mosso il nero(tocca al bianco) e c'è una riga/colonna libera allora ha quasi vinto
boolean isCheck(BoardState board, Coordinate kingCoordinate){
	int i,j;

	for (i = 0; i < kingCoordinate.row; i++)
		 if(GET_CELL_STATE(board.Board, i, kingCoordinate.column)!=EMPTY){
		 	return 0;
		 }
	
	for (i = kingCoordinate.row+1; i < 9; i++)
		 if(GET_CELL_STATE(board.Board, i, kingCoordinate.column)!=EMPTY){
		 	return 0;
		 }

    for (j = 0; j < kingCoordinate.column; j++)
		 if(GET_CELL_STATE(board.Board, kingCoordinate.row, j)!=EMPTY){
		 	return 0;
		 }

	for (j = kingCoordinate.column+1; j < 9; j++)
		 if(GET_CELL_STATE(board.Board, kingCoordinate.row, j)!=EMPTY){
		 	return 0;
		 }
	return 1;
}


 int abs(int i)
 {
     if (i < 0)
         return -i;
     else
         return i;
 }

