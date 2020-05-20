#include "client.h"
#include "search.h"
#include "moves.h"
#include "movesStack.h"
#include "sort.h"
#include "hash.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

char columnToServerFormat(uint32_t column) {
	return 'A' + column;
}

uint32_t  rowToServerFormat(uint32_t row) {
	return row + 1U;
}

int jsonToState(char* stateString, BoardState* state) {
	int row = 0;
	int column = 0;
	int count = 0;
	const char delimiter[2] = "\"";
	char* token;

	initEmptyBoard(state);
	token = strtok(stateString, delimiter);

	while(token != NULL && count < 81) {
		token = strtok(NULL, delimiter);

		if(strcmp(token, "EMPTY") == 0 || strcmp(token, "THRONE") == 0) {
			state->Board[row] = state->Board[row] << 2U;
			column++;
			count++;
		}
		else if(strcmp(token, "WHITE") == 0) {
			state->Board[row] = state->Board[row] << 2U;
			state->Board[row] |= WHITE;
			column++;
			count++;
		}
		else if(strcmp(token, "BLACK") == 0) {
			state->Board[row] = state->Board[row] << 2U;
			state->Board[row] |= BLACK;
			column++;
			count++;
		}
		else if(strcmp(token, "KING") == 0) {
			state->Board[row] = state->Board[row] << 2U;
			state->Board[row] |= WHITE_KING;
			column++;
			count++;
		}

		if(column == 9) {
			column = 0;
			row++;
		}
	}

    strtok(NULL, delimiter);
    strtok(NULL, delimiter);
    strtok(NULL, delimiter);
    token = strtok(NULL, delimiter);
    printf("TURNO: -%s-\n", token);

    if(strcmp(token, "WHITE") == 0)
        return WHITE_TURN;
    else if(strcmp(token, "BLACK") == 0)
        return BLACK_TURN;
    else if(strcmp(token, "WHITEWIN") == 0)
        return WHITE_WIN_TURN;
    else if(strcmp(token,"BLACKWIN") == 0)
        return BLACK_WIN_TURN;
    else if(strcmp(token, "DRAW") == 0)
        return DRAW_TURN;

    return -1;
}

void sendMove(int sd, Move move, int playerColor) {
	int startRow, targetRow, stringLength;
	char startColumn, targetColumn;
	char moveString[MOVE_BUFFER_SIZE];

	startRow = rowToServerFormat(START_ROW(move));
	startColumn = columnToServerFormat(START_COLUMN(move));
	targetRow = rowToServerFormat(TARGET_ROW(move));
	targetColumn = columnToServerFormat(TARGET_COLUMN(move));

	if(playerColor == WHITE)
		sprintf(moveString, "{\"from\":\"%c%d\",\"to\":\"%c%d\",\"turn\":\"WHITE\"}\n", startColumn, startRow, targetColumn, targetRow);
	else
		sprintf(moveString, "{\"from\":\"%c%d\",\"to\":\"%c%d\",\"turn\":\"BLACK\"}\n", startColumn, startRow, targetColumn, targetRow);

	printf("\nInvio mossa: %s\n", moveString);

	stringLength = htonl(strlen(moveString));
	write(sd, &stringLength, sizeof(stringLength));
	write(sd, moveString, sizeof(char) * strlen(moveString));
}


void gameLoop(int playerColor, int maxTime, int numThreads, char* server_ip) {
	int sd, port;
	struct hostent *host;
	struct sockaddr_in servaddr;
	char stateString[STATE_BUFFER_SIZE];
	char name[] = PLAYER_NAME;
	BoardState state;
	Move move;

	// Initializes Trasposition table
	initTT();
	initStack(numThreads);

	// Sets sort tables
	setHistoryValueTable();
	setRelativeHistoryValueTable();

	// Initializes IP and port server
	port = (playerColor == WHITE) ? WHITE_SERVER_PORT : BLACK_SERVER_PORT;
	memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	host = gethostbyname(server_ip);
	servaddr.sin_addr.s_addr=((struct in_addr *)(host->h_addr))->s_addr;
	servaddr.sin_port = htons(port);

	// Socket creation
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0) {
	   perror("Socket error.\n");
	   exit(1);
	}

	// Connect
	if(connect(sd,(struct sockaddr *) &servaddr, sizeof(struct sockaddr)) < 0) {
	   perror("Connect error.\n");
	   exit(1);
	}

	// Player name send
	int length = htonl(strlen(name));
	write(sd, &length, sizeof(int));
	write(sd, name, sizeof(char) * strlen(name));

	// Game loop
	while(read(sd, stateString, 1) > 0) {
		read(sd, stateString, 1);
		read(sd, stateString, 1);
		read(sd, stateString, 1);

		ssize_t bytesReceived = read(sd, stateString, STATE_BUFFER_SIZE);
        stateString[bytesReceived] = '\0';
		int turn = jsonToState(stateString, &state);
		printBoard(state);

		// Black palyer turn
		if(turn == BLACK_TURN && playerColor == BLACK) {
			// Move calculate
			move=lazySmpBlack(state, maxTime, numThreads);
			printf("\nBest move: ");
			printMove(move);
			printf("\n");
			resetStack(numThreads);
			pushPlayed(state, numThreads);
			pushPlayed(moveBlack(state,move),numThreads);

			// Move send
			sendMove(sd, move, playerColor);
		}

		// White palyer turn
		else if(turn == WHITE_TURN && playerColor == WHITE) {
			// Move calculate
			move=lazySmpWhite(state, maxTime, numThreads);
			resetStack(numThreads);
			pushPlayed(state, numThreads);
			printf("Best move: ");
			printMove(move);
			printf("\n");
			pushPlayed(moveWhite(state,move),numThreads);

			// Move send
			sendMove(sd, move, playerColor);
		}

		// White wins, end match
		else if(turn == WHITE_WIN_TURN) {
		    printf("\n\nEND MATCH: WHITE WINS\n\n");
		    continue;
		}

		// Black wins, end match
		else if(turn == BLACK_WIN_TURN) {
		    printf("\n\nEND MATCH: BLACK WINS\n\n");
		    continue;
		}

		// Draw, end match
		else if(turn == DRAW_TURN) {
		    printf("\n\nEND MATCH: DRAW\n\n");
		    continue;
		}
	}
	close(sd);
}
