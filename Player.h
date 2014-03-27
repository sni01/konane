/*
 *  Player.h
 *  Konane
 *
 *  Created by ablumer on 12/26/13.
 *  Copyright 2013. All rights reserved.
 *
 * A Konane player class
 * Current types (in myType) are 'h' for human and 'c' for computer.
 * myPiece is 'x' or 'o' for the player's game piece
 * myOrder identifies whether this player made the first or second move
 * legalMoves is an array of currently available moves
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "Board.h"

/* Public member functions:
 *
 * firstMove - removes a corner or center piece for the first move
 * secondMove - removes a piece adjacent to the first piece removed
 * nextMove - makes a legal move after the first two
 *
 * Private member functions:
 *
 * checkLegalMove - checks move m for piece p on board b for legality
 *    gives error messages and returns "false" if illegal
 * checkLegalMoveC - same as above without error messages
 * findMoves - find all legal moves and store them in legalMoves
 * 
 */
class Player
{
public:

Player();

Player( char ptype, char piece, unsigned searchDepth );

void firstMove( Board &, bool shouldPrintMove=false);
bool secondMove( Board &, bool shouldPrintMove=false);
bool nextMove( Board &, char mode, bool shouldPrintMove=false);

private:

bool checkLegalMove( Board b, Move m, char p);
bool checkLegalMoveC( Board b, Move m, char p);
void findMoves( Board b, char p );
int MinValue( unsigned depth, unsigned maxDepth, Board b, Move m );
int MaxValue( unsigned depth, unsigned maxDepth, Board b, Move m );

char myType;
char myPiece;
unsigned myDepth;
std::vector< Move > legalMoves;
};

#endif
