#pragma once
#include "board.h"
#include "moves.h"
#include "client.h"
#include "minmax.h"
#include "basicEvaluation.h"
#include "stats.h"
#include "TT.h"
#include "lazySmp.h"
#include "main.h"
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>


/* ARGUMENTS */
// argv[1] : player color
// argv[2] : timeout [seconds]
// argv[3] : server ip


int main(int argc, char* argv[]) {
    int playerColor, maxTime, numThreads;
    int i;
    char * server_ip;

    // Arguments number errors
    if (argc < 2){
        printf("Too few arguments\n"
        "./IbimbidiGiuseppeConte [colorplayer]\n"
        "./IbimbidiGiuseppeConte [colorplayer] [timeout]\n"
        "./IbimbidiGiuseppeConte [colorplayer] [timeout] [ip_server]\n");
        exit(1);
    }
    if (argc > 4){
        printf("Too many arguments\n"
        "./IbimbidiGiuseppeConte [colorplayer]\n"
        "./IbimbidiGiuseppeConte [colorplayer] [timeout]\n"
        "./IbimbidiGiuseppeConte [colorplayer] [timeout] [ip_server]\n");
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
    numThreads = THREAD;

    //Reads additionals arguments
	if(argc == 2){
	    maxTime = TIME;
        server_ip= SERVER_IP;
    }else{
        if (argc == 3){
            maxTime = atoi(argv[2]) - SAFETY_TIME;
            server_ip= SERVER_IP;
		}else{
            if (argc == 4){
                maxTime = atoi(argv[2]) - SAFETY_TIME;    
                server_ip= argv[3];
            }
        }
    }

    // Prints arguments
    printf("Color player: %s\n", argv[1]);
	printf("Timeout: %d\n", maxTime);
    printf("Safety time: %d\n", SAFETY_TIME);
    printf("Server IP: %s\n", server_ip);
    printf("Threads: %d\n\n", numThreads);
    
    // Starts to play
    setTime(maxTime);
    gameLoop(playerColor, maxTime, numThreads, server_ip);

    // Prints stats
    printGameStats(numThreads);
    return 0;
}