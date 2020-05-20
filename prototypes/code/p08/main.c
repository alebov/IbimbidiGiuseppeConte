#pragma once
#include "board.h"
#include "moves.h"
#include "client.h"
#include "minmax.h"
#include "stats.h"
#include "TTbase.h"
#include "lazySmp.h"
#include "main.h"
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

/* ARGUMENTS */
// argv[1] : colore giocatore
// argv[2] : valore timeout (a questo verranno sottratti 5 secondi di margine)
// argv[3] : numero di thread

int main(int argc, char* argv[]) {
    int playerColor, maxTime, numThreads;
    int i;

    // Too few arguments
    if (argc < 2){
        printf("Errore nel numero di argomenti\n");
        exit(1);
    }

    // Elaborates arguments
    for(i = 0; i < strlen(argv[1]); i++) {
        argv[1][i] = tolower(argv[1][i]);
    }

    // Reads player color
    if(strcmp(argv[1], "white") == 0) playerColor = WHITE;
    else if(strcmp(argv[1], "black") == 0) playerColor = BLACK;
    else playerColor = WHITE;

    //Reads additionals arguments
	if(argc == 2){
	    maxTime = TIME;
        numThreads = THREAD;
    }else{
        if (argc == 3){
            maxTime = atoi(argv[2]) - 5;
            numThreads = THREAD;
		}else{
            if (argc == 4){
                maxTime = atoi(argv[2]) - 5;
                numThreads = atoi(argv[3]);
            }
        }
    }

    // Prints arguments
    printf("Colore giocatore: %s\n", argv[1]);
	printf("Max time: %d\n", maxTime);
    printf("Num threads: %d\n\n", numThreads);

    // Starts to play
    setTime(maxTime);
    gameLoop(playerColor, maxTime, numThreads);

    // Prints stats
    printGameStats(numThreads);
    return 0;
}

