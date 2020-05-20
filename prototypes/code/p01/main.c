#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "search.h"
#include "moves.h"
#include "client.h"
#include "minmax.h"
#include "stats.h"
#define TIME 55
#define THREADS 2


// argv[1] : colore giocatore
// argv[2] : valore timeout (a questo verranno sottratti 5 secondi di margine)
// argv[3] : numero di thread
int main(int argc, char* argv[]) {
    int playerColor, maxTime, numThreads;
    int i;

	  if (argc < 2){
		    printf("Errore nel numero di argomenti\n");
		    exit(1);
	  }

    for(i = 0; i < strlen(argv[1]); i++) {
        argv[1][i] = tolower(argv[1][i]);
    }
    printf("%s\n", argv[1]);

    if(strcmp(argv[1], "white") == 0)
        playerColor = WHITE;
    else if(strcmp(argv[1], "black") == 0)
        playerColor = BLACK;
    else
        playerColor = WHITE;

	  if(argc == 2){
		    maxTime = TIME;
        numThreads = THREADS;
    } else {
        if (argc == 3){
            maxTime = atoi(argv[2]) - 5;   // 5 secondi di margine rispetto al timeout
            numThreads = THREADS;
		    } else {
            if (argc == 4){
                maxTime = atoi(argv[2]) - 5;   // 5 secondi di margine rispetto al timeout
                numThreads = atoi(argv[3]);
            }
        }
    }

	printf("Max time: %d\n", maxTime);
    printf("Num threads: %d\n\n", numThreads);
    setTime(maxTime);
    gameLoop(playerColor, maxTime, numThreads);
    printGameStats();
    return 0;
}


/*
int main(int argc, char** argv){
	BoardState board;
	int i=0;
	int dim;
	initBoard(&board);
	printBoard(board);
    printf("%d",evaluation(board));
	return 0;

}

*/