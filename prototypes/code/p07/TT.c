#pragma once
#include "TT.h"
#include "main.h"

Entry whiteTT[TTDIM];
Entry blackTT[TTDIM];


void initTT(){
    int i;
    for(i=0;i<TTDIM;i++)
      {
        whiteTT[i].type=EMTPY_ENTRY;
        whiteTT[i].type=EMTPY_ENTRY;
      }


}

//to add a colmpetly evaluate node NB: you must not call it if the node is a cut-node
void addWhite(Hash hash, int value, int depth, Move bestMove,Type type){
  //printf("added white entry\n");
  int h = hash % TTDIM;
  if(whiteTT[h].type==EMTPY_ENTRY || whiteTT[h].depth<=depth){//always+depth
    whiteTT[h].hash = hash;
    whiteTT[h].value = value;
    whiteTT[h].depth = depth;
    whiteTT[h].bestMove = bestMove;
    whiteTT[h].type=type;
  }
}

//to know if the board is already been evaluate whne min or max start
Entry findWhiteEntry(Hash hash){
  int h = hash % TTDIM;
  Entry result = whiteTT[h];
  if((result.hash ) == hash){  // If the entry is present
    //printf("finded white entry\n");
    return result;
  } 
  else{
    //printf("find white entry miss\n");
    result.type = EMTPY_ENTRY; // This means that the entry has not been found
    return result;
  }
}


Entry findWhite(BoardState board){
    Hash hash=stringHash(board);
    BoardState temp;
    int h = hash % TTDIM;
    Entry result = whiteTT[h];
    //1-nessuna simmetria
    if(result.hash == hash)
      return result;
    //2-simmetria orrizzontale
    CreateSymmetricalHorizontal(&board,&temp);
    hash=stringHash(temp);
    h = hash % TTDIM;
    result = whiteTT[h];
    if(result.hash == hash){
      result.bestMove=createSymmetricalHorizontalMove(result.bestMove);
      return result;
    }
    //3-simmetria verticale
    CreateSymmetricalVertical(&board,&temp);
    hash=stringHash(temp);
    h = hash % TTDIM;
    result = whiteTT[h];
    if(result.hash == hash){
      result.bestMove=createSymmetricalVerticalMove(result.bestMove);
      return result;
    }
    //4-diagonale up to down
    CreateSymmetricalUpToDown(&board,&temp);
    hash=stringHash(temp);
    h = hash % TTDIM;
    result = whiteTT[h];
    if(result.hash == hash){
      result.bestMove=createSymmetricalUpToDownMove(result.bestMove);
      return result;
    }
    //5-diagonale down to up
    CreateSymmetricalDownToUp(&board,&temp);
    hash=stringHash(temp);
    h = hash % TTDIM;
    result = whiteTT[h];
    if(result.hash == hash){
      result.bestMove=createSymmetricalDownToUpMove(result.bestMove);
      return result;
    }
    //6-90 ddegrees rotation
    Create90degrees(&board,&temp);
    hash=stringHash(temp);
    h = hash % TTDIM;
    result = whiteTT[h];
    if(result.hash == hash){
      result.bestMove=create90degreesMove(create90degreesMove(create90degreesMove(result.bestMove))); //270 rotation
      return result;
    }
    //7-180 degrees rotation
    Create90degrees(&temp,&board);  //new value of board is rotate by 180 
    hash=stringHash(board);
    h = hash % TTDIM;
    result = whiteTT[h];
    if(result.hash == hash){
      result.bestMove=create90degreesMove(create90degreesMove(result.bestMove)); //180 rotation
      return result;
    }
    //8-270 degrees rotation
    Create90degrees(&board,&temp);  //new value of temp is rotate by 270
    hash=stringHash(temp);
    h = hash % TTDIM;
    result = whiteTT[h];
    if(result.hash == hash){
      result.bestMove=create90degreesMove(result.bestMove); //90 rotation
      return result;
    }
    result.type=EMTPY_ENTRY;
    return result;
}

void addBlack(Hash hash, int value, int depth, Move bestMove,Type type){
  //printf("added black entry\n");
  int h = hash % TTDIM;
  if(blackTT[h].type==EMTPY_ENTRY || blackTT[h].depth<depth){ //always replace +depth
    blackTT[h].hash = hash;
    blackTT[h].value = value;
    blackTT[h].depth = depth;
    blackTT[h].bestMove = bestMove;
    blackTT[h].type=type;
    }
}


// Chech whether the found entry is valid or not
Bool isValidEntry(Entry entry){
  if(entry.type == EMTPY_ENTRY)
    return false;
  else
    return true;
}

Entry findBlackEntry(Hash hash){
  int h = hash % TTDIM;
  Entry result = blackTT[h];
  if(result.hash  == hash){ // If the entry is present
    //printf("finded black entry\n");
    return result;
  }
  else{
    //printf("find black entry miss\n");
    result.type = EMTPY_ENTRY; // This means that the entry has not been found
    return result;
  }
}

Entry findBlack(BoardState board){
    Hash hash=stringHash(board);
    BoardState temp;
    int h = hash % TTDIM;
    Entry result = blackTT[h];
    //1-nessuna simmetria
    if(result.hash == hash)
      return result;
    //2-simmetria orrizzontale
    CreateSymmetricalHorizontal(&board,&temp);
    hash=stringHash(temp);
    h = hash % TTDIM;
    result = blackTT[h];
    if(result.hash == hash){
      result.bestMove=createSymmetricalHorizontalMove(result.bestMove);
      return result;
    }
    //3-simmetria verticale
    CreateSymmetricalVertical(&board,&temp);
    hash=stringHash(temp);
    h = hash % TTDIM;
    result = blackTT[h];
    if(result.hash == hash){
      result.bestMove=createSymmetricalVerticalMove(result.bestMove);
      return result;
    }
    //4-diagonale up to down
    CreateSymmetricalUpToDown(&board,&temp);
    hash=stringHash(temp);
    h = hash % TTDIM;
    result = blackTT[h];
    if(result.hash == hash){
      result.bestMove=createSymmetricalUpToDownMove(result.bestMove);
      return result;
    }
    //5-diagonale down to up
    CreateSymmetricalDownToUp(&board,&temp);
    hash=stringHash(temp);
    h = hash % TTDIM;
    result = blackTT[h];
    if(result.hash == hash){
      result.bestMove=createSymmetricalDownToUpMove(result.bestMove);
      return result;
    }
    //6-90 ddegrees rotation
    Create90degrees(&board,&temp);
    hash=stringHash(temp);
    h = hash % TTDIM;
    result = blackTT[h];
    if(result.hash == hash){
      result.bestMove=create90degreesMove(create90degreesMove(create90degreesMove(result.bestMove))); //270 rotation
      return result;
    }
    //7-180 degrees rotation
    Create90degrees(&temp,&board);  //new value of board is rotate by 180 
    hash=stringHash(board);
    h = hash % TTDIM;
    result = blackTT[h];
    if(result.hash == hash){
      result.bestMove=create90degreesMove(create90degreesMove(result.bestMove)); //180 rotation
      return result;
    }
    //8-270 degrees rotation
    Create90degrees(&board,&temp);  //new value of temp is rotate by 270
    hash=stringHash(temp);
    h = hash % TTDIM;
    result = blackTT[h];
    if(result.hash == hash){
      result.bestMove=create90degreesMove(result.bestMove); //90 rotation
      return result;
    }
    result.type=EMTPY_ENTRY;
    return result;
}