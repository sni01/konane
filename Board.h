/*
 *  Board.h
 *  Konane
 *
 *  Created by ablumer on 12/20/13.
 *  Copyright 2013. All rights reserved.
 *
 *  A Konane board as a two-dimensional array of char (boardArray)
 *  The Board class also contains the positions of the pieces removed
 *  on the first and second moves, and a history of moves up to the
 *  current board state.
 *  The arrays boardCorners, boardAdjCorners, boardCenters are
 *  lists of positions that are corners, adjacent to corners, and
 *  centers of the board.  They are computed by the constructor and
 *  should not need to be modified.
 */
 
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

/* Constants giving the maximum dimensions of the Konane
 * board.  The actual array is larger by 1 in each dimension
 * to allow 1-based indexing
 */

const int MAX_ROWS = 30;
const int MAX_COLS = 60;

/* Data structures:
 * Position gives the row (1..boardRows) and
 *    column (1..boardCols) of a square on the board
 * Move gives the start and end position of a move 
 */
struct Position {
	unsigned row;
	unsigned col;
};

struct Move {
	Position from;
	Position to;
};

/* Public member functions:
 *
 * printBoard - prints the board to output stream "out"
 * setRows and setCols - set the actual size of the board
 * setPiece - change the value of a square on the board
 * setFirstMove - remember which piece was removed by the first player
 * setSecondMove - remember which piece was removed by the second player
 * makeMove - modify the board for a legal move
 *    Note: no error checking - move must be legal
 * getRows and getCols - return the actual dimensions of the board
 * getPiece - return the contents of the square at board position p
 * getFirstMove - find the position of the first move
 *    ( value returned through parameter )
 * isCorner checks whether a position is a corner square of the board
 * isNextToCorner checks whether a position is a corner square of the board
 * isCorner checks whether a position is adjacent to a corner square of the board
 * isCenter checks whether a position is a center square of the board
 */
class Board
{
public:

Board();
Board( unsigned rows, unsigned cols );
Board(unsigned rows, unsigned cols, char* const raw);

void printBoard( ostream & out ) const;
void setRows( int rows );
void setCols( int cols );
void setPiece( int r, int c, char piece );
void setFirstMove( Position p );
void setSecondMove( Position p );
void makeMove( Move m );
int getRows() const;
int getCols() const;
char getPiece( Position p ) const;
void getFirstMove( Position & ) const;
bool isCorner( Position p );
bool isNextToCorner( Position p );
bool isCenter( Position p );
unsigned getBlanks() const;

private:
void populateVectors();
char boardArray[MAX_ROWS+1][MAX_COLS+1];
unsigned blanks;
unsigned boardRows, boardCols;
Position firstMove;
Position secondMove;
std::vector< Move > moveHistory;
std::vector< Position > boardCorners;
std::vector< Position > boardAdjCorners;
std::vector< Position > boardCenters;

};

#endif
