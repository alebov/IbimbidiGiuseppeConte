#include "moves.h"
#include <stdlib.h>
#include <string.h>




BoardState moveWhite(BoardState board, Move move) {
	if (GETSTART(board.Board, move) == WHITE_KING) {
		APPLY_MOVE_KING(board.Board, move);
	}
	else {
		APPLY_MOVE_WHITE(board.Board, move);
	}

	checkBlackCaptured(&board, TARGET_ROW(move), TARGET_COLUMN(move));

	return board;
}




BoardState moveBlack(BoardState board, Move move) {
	APPLY_MOVE_BLACK(board.Board, move);

	checkWhiteCaptured(&board, TARGET_ROW(move), TARGET_COLUMN(move));

	return board;
}


void getMovesForWhitePiece(BoardState currentState, int row, int column, Move* moves, int* count) {
	int i;

	for (i = column - 1; i >= 0; i--) {
		if ((GET_CELL_STATE(currentState.Board, row, i)) == EMPTY && isMoveValid(row, column, row, i)) {
			Move move = CREATE_MOVE(row, column, row, i);
			moves[*count] = move;
			*count = *count + 1;
		}
		else
			break;
	}

	for (i = column + 1; i < 9; i++) {
		if ((GET_CELL_STATE(currentState.Board, row, i)) == EMPTY && isMoveValid(row, column, row, i)) {
			Move move = CREATE_MOVE(row, column, row, i);
            moves[*count] = move;
            *count = *count + 1;
		}
		else
			break;
	}

	for (i = row - 1; i >= 0; i--) {
		if ((GET_CELL_STATE(currentState.Board, i, column)) == EMPTY && isMoveValid(row, column, i, column)) {
			Move move = CREATE_MOVE(row, column, i, column);
            moves[*count] = move;
            *count = *count + 1;
		}
		else
			break;
	}

	for (i = row + 1; i < 9; i++) {
		if ((GET_CELL_STATE(currentState.Board, i, column)) == EMPTY && isMoveValid(row, column, i, column)) {
			Move move = CREATE_MOVE(row, column, i, column);
            moves[*count] = move;
            *count = *count + 1;
		}
		else
			break;
	}
}


void getMovesForBlackPiece(BoardState currentState, int row, int column, Move* moves, int* count) {
	int i;

	for (i = column - 1; i >= 0; i--) {
		if ((GET_CELL_STATE(currentState.Board, row, i)) == EMPTY && isMoveValid(row, column, row, i)) {
			Move move = CREATE_MOVE(row, column, row, i);
            moves[*count] = move;
            *count = *count + 1;
		}
		else
			break;
	}

	for (i = column + 1; i < 9; i++) {
		if ((GET_CELL_STATE(currentState.Board, row, i)) == EMPTY && isMoveValid(row, column, row, i)) {
			Move move = CREATE_MOVE(row, column, row, i);
            moves[*count] = move;
            *count = *count + 1;
		}
		else
			break;
	}

	for (i = row - 1; i >= 0; i--) {
		if ((GET_CELL_STATE(currentState.Board, i, column)) == EMPTY && isMoveValid(row, column, i, column)) {
			Move move = CREATE_MOVE(row, column, i, column);
            moves[*count] = move;
            *count = *count + 1;
		}
		else
			break;
	}

	for (i = row + 1; i < 9; i++) {
		if ((GET_CELL_STATE(currentState.Board, i, column)) == EMPTY && isMoveValid(row, column, i, column)) {
			Move move = CREATE_MOVE(row, column, i, column);
            moves[*count] = move;
            *count = *count + 1;
		}
		else
			break;
	}
}



int getPossibleMovesWhite(BoardState currentState, Move* moves) {
	uint32_t row, column;
	int count = 0;

	for (row = 0; row < 9; row++) {
		for (column = 0; column < 9; column++) {
			if (GET_CELL_STATE(currentState.Board, row, column) > 1) {
				getMovesForWhitePiece(currentState, row, column, moves, &count);
			}
		}
	}

	return count;
}


int getPossibleMovesBlack(BoardState currentState, Move* moves) {
	uint32_t row, column;
	int count = 0;

	for (row = 0; row < 9; row++) {
		for (column = 0; column < 9; column++) {
			if (GET_CELL_STATE(currentState.Board, row, column) == BLACK) {
				getMovesForBlackPiece(currentState, row, column, moves, &count);
			}
		}
	}

	return count;
}

int isMoveValid(int startRow, int startColumn, int targetRow, int targetColumn) {
	// NB righe e colonne sono numerate da 0 a 8

	/*
	Mossa "star fermi" non generabile dalle nostre condizioni
	// non posso restare fermo
	if (startRow == targetRow && startColumn == targetColumn)
		return 0;
	*/

	// non si può andare nel trono
	if (targetRow == 4 && targetColumn == 4)
		return 0;
	/*
		Riguardo agli attraversamenti del trono non posso sintetizzare le condizioni sul maggiore e minore come diverso
		(targetColumn != 4 nell'attraversamento per movimento riga o targetRow != 4 nell'attraversamento per movimento
		colonna) perché così facendo renderei illegali mosse sulla riga/colonna del trono che non attraversano il trono
	*/
	// non si può attraversare il trono tramite movimento riga
	if ((startRow == 4 && targetRow == 4) &&
		((startColumn < 4 && targetColumn  > 4) || (startColumn > 4 && targetColumn < 4)))
		return 0;

	// non si può attraversare il trono tramite movimento colonna
	if ((startColumn == 4 && targetColumn == 4) &&
		((startRow < 4 && targetRow > 4) || (startRow > 4 && targetRow < 4)))
		return 0;

	/*
		Casi riguardo l'ingresso negli accampamenti dicono in pratica che partendo da una casella diversa da una
		dell'accampamento ed arrivando ad una casella propria dell'accampamento ho effettuato una mossa illegale
	*/

	// accampamento sopra credo ottimo (solo seconda metà raccolta)
	if (((startRow != 0 && startColumn != 3) || (startRow != 0 && startColumn != 4) || (startRow != 0 && startColumn != 5) ||
		(startRow != 1 && startColumn != 4)) &&
		((targetRow == 0 && targetColumn >= 3 && targetColumn <= 5) || (targetRow == 1 && targetColumn == 4)))
		return 0;

	// accampamento sotto credo ottimo (solo seconda metà raccolta)
	if (((startRow != 8 && startColumn != 3) || (startRow != 8 && startColumn != 4) || (startRow != 8 && startColumn != 5) ||
		(startRow != 7 && startColumn != 4)) &&
		((targetRow == 8 && targetColumn >= 3 && targetColumn <= 5) || (targetRow == 7 && targetColumn == 4)))
		return 0;

	// accampamento sinistro credo ottimo (solo seconda metà raccolta)
	if (((startColumn != 0 && startRow != 3) || (startColumn != 0 && startRow != 4) || (startColumn != 0 && startRow != 5) ||
		(startColumn != 1 && startRow != 4)) &&
		((targetColumn == 0 && targetRow >= 3 && targetRow <= 5) || (targetColumn == 1 && targetRow == 4)))
		return 0;

	// accampamento destro credo ottimo (solo seconda metà raccolta)
	if (((startColumn != 8 && startRow != 3) || (startColumn != 8 && startRow != 4) || (startColumn != 8 && startRow != 5) ||
		(startColumn != 7 && startRow != 4)) &&
		((targetColumn == 8 && targetRow >= 3 && targetRow <= 5) || (targetColumn == 7 && targetRow == 4)))
		return 0;

	/*
		Non ha senso considerare le infrazioni di attraversamento accampamento tramite colonna per gli accampamenti sopra
		e sotto, dato che un tale attraversamento corrisponde per forza a fermarcisi, caso già contemplato qui sopra
		Non ha senso considerare le infrazioni di attraversamento accampamento tramite riga per gli accampamenti sinistro
		e destro, dato che un tale attraversamento corrisponde per forza a fermarcisi, caso già contemplato qui sopra
	*/
	// non si può attraversare l'accampamento sopra tramite movimento riga
	if ((startRow == 0 || startRow == 1) && (startColumn < 3 && startColumn > 5) &&
		(targetRow == 0 || targetRow == 1) && (targetColumn < 3 && targetColumn > 5))
		return 0;

	// non si può attraversare l'accampamento sotto tramite movimento riga
	if ((startRow == 8 || startRow == 7) && (startColumn < 3 && startColumn > 5) &&
		(targetRow == 8 || targetRow == 7) && (targetColumn < 3 && targetColumn > 5))
		return 0;

	// non si può attraversare l'accampamento sinistro tramite movimento colonna
	if ((startColumn == 0 || startColumn == 1) && (startRow < 3 && startRow > 5) &&
		(targetColumn == 0 || targetColumn == 1) && (targetRow < 3 && targetRow > 5))
		return 0;

	// non si può attraversare l'accampamento destro tramite movimento colonna
	if ((startColumn == 8 || startColumn == 7) && (startRow < 3 && startRow > 5) &&
		(targetColumn == 8 || targetColumn == 7) && (targetRow < 3 && targetRow > 5))
		return 0;

	// se nessuna mossa illegale trovata allora mossa valida
	return 1;
}

inline void checkWhiteCaptured(BoardState * state, int targetRow, int targetColumn) {
	uint32_t horizontalKingMask = 0b000000011101000000U; // nero re nero, già shiftata per il primo check
	uint32_t horizontalMask = 0b000000000000011001U; // nero bianco nero
	uint32_t verticalBlackMask = 0b010000000000000000U; // nero...
	uint32_t verticalWhiteMask = 0b100000000000000000U; // bianco...
	uint32_t verticalKingMask = 0b110000000000000000U; // re...

	// prima check sulla ipotetica cattura del re sul trono
	if (GET_CELL_STATE(state->Board, 4, 4) == WHITE_KING) {
		if ((state->Board[4] & horizontalKingMask) == horizontalKingMask && GET_CELL_STATE(state->Board, 3, 4) == BLACK && GET_CELL_STATE(state->Board, 5, 4) == BLACK) {
			DELETE_WHITE_KING(state->Board, 4, 4); // vittoria
		}
	}
	else {
		if (GET_CELL_STATE(state->Board, 3, 4) == WHITE_KING || GET_CELL_STATE(state->Board, 5, 4) == WHITE_KING ||
			GET_CELL_STATE(state->Board, 4, 3) == WHITE_KING || GET_CELL_STATE(state->Board, 4, 5) == WHITE_KING) { // re adiacente trono, ripeto il check per la cattura con tre neri più trono

			// mangia re sopra al trono
			if ((state->Board[3] & horizontalKingMask) == horizontalKingMask && GET_CELL_STATE(state->Board, 2, 4) == BLACK) {
				DELETE_WHITE_KING(state->Board, 3, 4); // vittoria
			}

			// mangia re sotto al trono
			if ((state->Board[5] & horizontalKingMask) == horizontalKingMask && GET_CELL_STATE(state->Board, 6, 4) == BLACK) {
				DELETE_WHITE_KING(state->Board, 5, 4); // vittoria
			}

			// mangia re a sinistra del trono
			if (GET_CELL_STATE(state->Board, 4, 3) == WHITE_KING &&
				GET_CELL_STATE(state->Board, 4, 2) == BLACK && GET_CELL_STATE(state->Board, 3, 3) == BLACK && GET_CELL_STATE(state->Board, 5, 3) == BLACK) {
				DELETE_WHITE_KING(state->Board, 4, 3); // vittoria
			}

			// mangia re a destra del trono
			if (GET_CELL_STATE(state->Board, 4, 5) == WHITE_KING &&
				GET_CELL_STATE(state->Board, 4, 6) == BLACK && GET_CELL_STATE(state->Board, 3, 5) == BLACK && GET_CELL_STATE(state->Board, 5, 5) == BLACK) {
				DELETE_WHITE_KING(state->Board, 4, 5); // vittoria
			}
		}
		else { // re non sul trono o ad esso adiacente, check generico
			BoardState tempKing;
			memcpy(&tempKing, state, 36);
			fakeBoardBlack(&tempKing);

			horizontalKingMask = horizontalKingMask >> 6; // sposto la mask del tutto a dx per check successivi (come nella check cattura del bianco generico)
			horizontalKingMask = horizontalKingMask << 2 * (8 - targetColumn);

			// mangia re a sinistra
			if ((targetColumn > 1) &&
				((tempKing.Board[targetRow] & horizontalKingMask) == horizontalKingMask) &&
				(GET_CELL_STATE(tempKing.Board, targetRow, (targetColumn - 2)) != WHITE_KING))
			{
				DELETE_WHITE_KING(state->Board, targetRow, (targetColumn - 1));
			}

			// mangia re a destra
			if ((targetColumn < 7) &&
				((tempKing.Board[targetRow] & (horizontalKingMask >> 4)) == (horizontalKingMask >> 4)) &&
				(GET_CELL_STATE(tempKing.Board, targetRow, (targetColumn + 2)) != WHITE_KING))
			{
				DELETE_WHITE_KING(state->Board, targetRow, (targetColumn + 1));
			}

			verticalBlackMask = verticalBlackMask >> (2 * targetColumn);
			verticalKingMask = verticalKingMask >> (2 * targetColumn);

			// mangia re sopra
			if ((targetRow > 1) &&
				((tempKing.Board[targetRow - 1] & verticalKingMask) == verticalKingMask) &&
				((tempKing.Board[targetRow - 2] & verticalBlackMask) == verticalBlackMask) &&
				(GET_CELL_STATE(tempKing.Board, (targetRow - 2), targetColumn) != WHITE_KING))
			{
				DELETE_WHITE_KING(state->Board, (targetRow - 1), targetColumn);
			}

			// mangia re sotto
			if ((targetRow < 7) &&
				((tempKing.Board[targetRow + 1] & verticalKingMask) == verticalKingMask) &&
				((tempKing.Board[targetRow + 2] & verticalBlackMask) == verticalBlackMask) &&
				(GET_CELL_STATE(tempKing.Board, targetRow + 2, targetColumn) != WHITE_KING))
			{
				DELETE_WHITE_KING(state->Board, (targetRow + 1), targetColumn);
			}
		}
	}

	BoardState temp;
	memcpy(&temp, state, 36);
	fakeBoardBlack(&temp);

	horizontalMask = horizontalMask << 2 * (8 - targetColumn);

	// mangia a sinistra
	if ((targetColumn > 1) &&
		((temp.Board[targetRow] & horizontalMask) == horizontalMask) &&
		(GET_CELL_STATE(temp.Board, targetRow, (targetColumn - 2)) != WHITE_KING) &&
		(GET_CELL_STATE(temp.Board, targetRow, (targetColumn - 1)) != WHITE_KING))
	{
		DELETE_WHITE(state->Board, targetRow, (targetColumn - 1));
	}

	// mangia a destra
	if ((targetColumn < 7) &&
		((temp.Board[targetRow] & (horizontalMask >> 4)) == (horizontalMask >> 4)) &&
		(GET_CELL_STATE(temp.Board, targetRow, (targetColumn + 2)) != WHITE_KING) &&
		(GET_CELL_STATE(temp.Board, targetRow, (targetColumn + 1)) != WHITE_KING))
	{
		DELETE_WHITE(state->Board, targetRow, (targetColumn + 1));
	}

	verticalBlackMask = verticalBlackMask >> (2 * targetColumn);
	verticalWhiteMask = verticalWhiteMask >> (2 * targetColumn);

	// mangia sopra
	if ((targetRow > 1) &&
		((temp.Board[targetRow - 1] & verticalWhiteMask) == verticalWhiteMask) &&
		((temp.Board[targetRow - 2] & verticalBlackMask) == verticalBlackMask) &&
		(GET_CELL_STATE(temp.Board, (targetRow - 2), targetColumn) != WHITE_KING) &&
		(GET_CELL_STATE(temp.Board, (targetRow - 1), targetColumn) != WHITE_KING))
	{
		DELETE_WHITE(state->Board, (targetRow - 1), targetColumn);
	}

	// mangia sotto
	if ((targetRow < 7) &&
		((temp.Board[targetRow + 1] & verticalWhiteMask) == verticalWhiteMask) &&
		((temp.Board[targetRow + 2] & verticalBlackMask) == verticalBlackMask) &&
		(GET_CELL_STATE(temp.Board, targetRow + 2, targetColumn) != WHITE_KING) &&
		(GET_CELL_STATE(temp.Board, (targetRow + 1), targetColumn) != WHITE_KING))
	{
		DELETE_WHITE(state->Board, (targetRow + 1), targetColumn);
	}
}

inline void checkBlackCaptured(BoardState * state, int targetRow, int targetColumn) {
	uint32_t horizontalMask = 0b000000000000100110U; // bianco nero bianco
	uint32_t verticalWhiteMask = 0b100000000000000000U; // bianco...
	uint32_t verticalBlackMask = 0b010000000000000000U; // nero...


	BoardState temp;
	memcpy(&temp, state, 36);
	fakeBoardWhite(&temp);

	horizontalMask = horizontalMask << 2 * (8 - targetColumn);


	// mangia a sinistra
	if ((targetColumn > 1) &&
		((temp.Board[targetRow] & horizontalMask) == horizontalMask) &&
		(GET_CELL_STATE(temp.Board, targetRow, (targetColumn - 1)) != WHITE_KING))
	{
		DELETE_BLACK(state->Board, targetRow, (targetColumn - 1));
	}

	// mangia a destra
	if ((targetColumn < 7) &&
		((temp.Board[targetRow] & (horizontalMask >> 4)) == (horizontalMask >> 4)) &&
		(GET_CELL_STATE(temp.Board, targetRow, (targetColumn + 1)) != WHITE_KING))
	{
		DELETE_BLACK(state->Board, targetRow, (targetColumn + 1));
	}

	verticalWhiteMask = verticalWhiteMask >> (2 * targetColumn);
	verticalBlackMask = verticalBlackMask >> (2 * targetColumn);

	// mangia sopra
	if ((targetRow > 1) &&
		((temp.Board[targetRow - 2] & verticalWhiteMask) == verticalWhiteMask) &&
		((temp.Board[targetRow - 1] & verticalBlackMask) == verticalBlackMask) &&
		(GET_CELL_STATE(temp.Board, (targetRow - 1), targetColumn) != WHITE_KING))
	{
		DELETE_BLACK(state->Board, (targetRow - 1), targetColumn);
	}

	// mangia sotto
	if ((targetRow < 7) &&
		((temp.Board[targetRow + 2] & verticalWhiteMask) == verticalWhiteMask) &&
		((temp.Board[targetRow + 1] & verticalBlackMask) == verticalBlackMask) &&
		(GET_CELL_STATE(temp.Board, (targetRow + 1), targetColumn) != WHITE_KING))
	{
		DELETE_BLACK(state->Board, (targetRow + 1), targetColumn);
	}


}

// setta gli accampamenti come occupati da neri (anche il trono perché altrimenti le mask non trovano le catture che lo coinvolgono)
void fakeBoardBlack(BoardState * state) {
	uint32_t fakeRow08 = 0b000000010001000000U; // non setto casella centrale accampamento tanto non servirà mai
	uint32_t fakeRow17 = 0b000000000100000000U;
	uint32_t fakeRow35 = 0b010000000000000001U;
	uint32_t fakeRow4 = 0b000100000100000100U; // non setto casella centrale accampamento tanto non servirà mai

	// tramite l'& elimino il re dala trono in caso ci sia
	state->Board[0] = state->Board[0] | fakeRow08;
	state->Board[1] = state->Board[1] | fakeRow17;
	state->Board[3] = state->Board[3] | fakeRow35;
	state->Board[4] = (state->Board[4] & 0b111111110011111111U) | fakeRow4;
	state->Board[5] = state->Board[5] | fakeRow35;
	state->Board[7] = state->Board[7] | fakeRow17;
	state->Board[8] = state->Board[8] | fakeRow08;
}

// setta gli accampamenti ed il trono come occupati da bianchi (anche il trono perché altrimenti le mask non trovano le catture che lo coinvolgono)
void fakeBoardWhite(BoardState * state) {
	uint32_t fakeRow08 = 0b000000100010000000U; // non setto casella centrale accampamento tanto non servirà mai
	uint32_t fakeRow17 = 0b000000001000000000U;
	uint32_t fakeRow35 = 0b100000000000000010U;
	uint32_t fakeRow4 = 0b001000001000001000U; // non setto casella centrale accampamento tanto non servirà mai

	// tramite gli & elimino i neri dagli accampamenti in caso ci siano
	state->Board[0] = (state->Board[0] & 0b111111000000111111U) | fakeRow08;
	state->Board[1] = (state->Board[1] & 0b111111110011111111U) | fakeRow17;
	state->Board[3] = (state->Board[3] & 0b001111111111111100U) | fakeRow35;
	state->Board[4] = (state->Board[4] & 0b000011111111110000U) | fakeRow4;
	state->Board[5] = (state->Board[5] & 0b001111111111111100U) | fakeRow35;
	state->Board[7] = (state->Board[7] & 0b111111110011111111U) | fakeRow17;
	state->Board[8] = (state->Board[8] & 0b111111000000111111U) | fakeRow08;
}

void printMove(Move move) {
    char startColumn = columnToServerFormat(START_COLUMN(move));
    char targetColumn = columnToServerFormat(TARGET_COLUMN(move));
    int startRow = rowToServerFormat(START_ROW(move));
    int targetRow = rowToServerFormat(TARGET_ROW(move));

    printf("[%c%d -> %c%d]", startColumn, startRow, targetColumn, targetRow);
}