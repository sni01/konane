/*
 *  Board.cpp
 *  Konane
 *
 *  Created by ablumer on 12/20/13.
 *  Copyright 2013. All rights reserved.
 *
 */

#include <iostream>
using namespace std;

#include "Board.h"

Board::Board()
{
}

Board::Board(unsigned rows, unsigned cols, char* const raw) {
	boardRows = rows;
	boardCols = cols;
	blanks = 0;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			boardArray[r+1][c+1] = raw[r * cols + c];
			if (boardArray[r+1][c+1] == ' ') {
				blanks++;
			}
		}
	}
	// HACK: I shouldn't have to do this
	if (blanks == 1) {
		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < cols; c++) {
				if (boardArray[r+1][c+1] == ' ') {
					firstMove.row = r+1;
					firstMove.col = r+1;
				}
			}
		}		
	}
	populateVectors();
}

/* Set the actual size of the board and fill it with
 * alternating x and o chars
 */
Board::Board( unsigned rows, unsigned cols )
{
	boardRows = rows;
	boardCols = cols;
	for (int r=1; r<=boardRows; r++) {
		for (int c=1; c<=boardCols; c++) {
			if ( (r+c) % 2 )
				boardArray[r][c] = 'x';
			else boardArray[r][c] = 'o';
		}
	}
	populateVectors();
}

void Board::populateVectors() {
	Position p;
	if (boardRows == 1) {
		p.row = 1; p.col = 1;
		boardCorners.push_back( p );
		p.row = 1; p.col = boardCols;
		boardCorners.push_back( p );
		p.row = 1; p.col = 2;
		boardAdjCorners.push_back( p );
		p.row = 1; p.col = boardCols - 1;
		boardAdjCorners.push_back( p );
		if ( (boardCols%2) == 1 ) {
			p.row = 1; p.col = (1+boardCols)/2;
			boardCenters.push_back( p );
		}
		else {
			p.row = 1; p.col = (1+boardCols)/2;
			boardCenters.push_back( p );
			p.col += 1;
			boardCenters.push_back( p );
		}
	}
	else if (boardCols == 1) {
		p.row = 1; p.col = 1;
		boardCorners.push_back( p );
		p.row = boardRows; p.col = 1;
		boardCorners.push_back( p );
		p.row = 2; p.col = 1;
		boardAdjCorners.push_back( p );
		p.row = boardRows - 1; p.col = 1;
		boardAdjCorners.push_back( p );
		if ( (boardRows%2) == 1 ) {
			p.row = (1+boardRows)/2; p.col = 1;
			boardCenters.push_back( p );
		}
		else {
			p.row = (1+boardRows)/2; p.col = 1;
			boardCenters.push_back( p );
			p.row += 1;
			boardCenters.push_back( p );
		}
	}
	else {
		p.row = 1; p.col = 1;
		boardCorners.push_back( p );
		p.row = 1; p.col = boardCols;
		boardCorners.push_back( p );
		p.row = boardRows; p.col = 1;
		boardCorners.push_back( p );
		p.row = boardRows; p.col = boardCols;
		boardCorners.push_back( p );
		
		p.row = 1; p.col = 2;
		boardAdjCorners.push_back( p );
		p.row = 1; p.col = boardCols - 1;
		boardAdjCorners.push_back( p );
		p.row = boardRows - 1; p.col = 1;
		boardAdjCorners.push_back( p );
		p.row = boardRows - 1; p.col = boardCols;
		boardAdjCorners.push_back( p );
		p.row = 2; p.col = 1;
		boardAdjCorners.push_back( p );
		p.row = 2; p.col = boardCols;
		boardAdjCorners.push_back( p );
		p.row = boardRows; p.col = 2;
		boardAdjCorners.push_back( p );
		p.row = boardRows; p.col = boardCols - 1;
		boardAdjCorners.push_back( p );
		
		if ( ( (boardRows%2) == 1 ) && ( (boardCols%2) == 1 )) {
			p.row = (1+boardRows)/2; p.col = (1+boardCols)/2;
			boardCenters.push_back( p );
		}
		else if ( (boardRows%2) == 1 ) {
			p.row = (1+boardRows)/2; p.col = (1+boardCols)/2;
			boardCenters.push_back( p );
			p.col += 1;
			boardCenters.push_back( p );
		}
		else if ( (boardCols%2) == 1 ) {
			p.row = (1+boardRows)/2; p.col = (1+boardCols)/2;
			boardCenters.push_back( p );
			p.row += 1;
			boardCenters.push_back( p );
		}
		else {
			p.row = (1+boardRows)/2; p.col = (1+boardCols)/2;
			boardCenters.push_back( p );
			p.row += 1;
			boardCenters.push_back( p );
			p.row = (1+boardRows)/2; p.col += 1;
			boardCenters.push_back( p );
			p.row += 1;
			boardCenters.push_back( p );
		}
	}
}

unsigned Board::getBlanks() const {
	return blanks;
}


/* Print the current board to an output stream
 * Mark the most recent move by an underscore for
 * the start position and capitalize the end position
 */
void Board::printBoard( ostream & out ) const
{
	Move lastMove;
	Position lastFrom, lastTo;
	char lastPiece;
	
	if (moveHistory.size() > 0) {
		lastMove = moveHistory[moveHistory.size()-1];
		lastFrom = lastMove.from;
		lastTo = lastMove.to;
		lastPiece = boardArray[lastTo.row][lastTo.col];
	}
	else {
		lastFrom.row = 0; lastFrom.col = 0; lastTo.row = 0; lastTo.col = 0;
	}
	out << "     ";
	for (int c=1; c<=boardCols; c++) {
		if (c<10) { out << " "; }
		else { out << ( c/10 ); }
	}
	out << endl;
	out << "     ";
	for (int c=1; c<=boardCols; c++) {
		out << ( c % 10 );
	}
	out << endl;
	out << endl;
	for (int r=boardRows; r>=1; r--) {
		out.width(2);
		out << r;
		out << "   ";
		for (int c=1; c<=boardCols; c++) {
			if ( (r == lastFrom.row) && (c == lastFrom.col) ) {
				out << "_";
			}
			else if ( (r == lastTo.row) && (c == lastTo.col) ) {
				if (lastPiece == 'x') {
					out << 'X';
				}
				else {
					out << 'O';
				}
			}
			else {
				out << boardArray[r][c];
			}
		}
		out << endl;
	}
}

void Board::setRows( int rows )
{
	boardRows = rows;
}

void Board::setCols( int cols )
{
	boardCols = cols;
}

void Board::setPiece( int r, int c, char piece )
{
	boardArray[r][c] = piece;
}

void Board::setFirstMove( Position p )
{
	firstMove.row = p.row;
	firstMove.col = p.col;
	boardArray[p.row][p.col] = ' ';
}

void Board::setSecondMove( Position p )
{
	secondMove.row = p.row;
	secondMove.col = p.col;
	boardArray[p.row][p.col] = ' ';
}

/* Make a move.  The move must have been previously
 * checked for legality.  Moves can be horizontal
 * or vertical, in a positive or negative direction,
 * but not on a diagonal.
 */
void Board::makeMove( Move m )
{
	int r, c, direction;
	char piece;
	Position p1, p2;
	
	p1 = m.from;
	p2 = m.to;
	moveHistory.push_back( m );
	piece = boardArray[p1.row][p1.col];
	boardArray[p1.row][p1.col] = ' ';
	boardArray[p2.row][p2.col] = piece;
	if (p1.row == p2.row) {
		if (p2.col > p1.col) { direction = 1; }
		else { direction = -1; }
		c = p1.col;
		while (c != p2.col) {
			boardArray[p1.row][c+direction] = ' ';
			c += (2*direction);
		}
	}
	else {
		if (p2.row > p1.row) { direction = 1; }
		else { direction = -1; }
		r = p1.row;
		while (r != p2.row) {
			boardArray[r+direction][p1.col] = ' ';
			r += (2*direction);
		}
	}
}

int Board::getRows() const
{
	return boardRows;
}

int Board::getCols() const
{
	return boardCols;
}

char Board::getPiece( Position p ) const
{
	return boardArray[p.row][p.col];
}

void Board::getFirstMove( Position &p ) const
{
	p.row = firstMove.row;
	p.col = firstMove.col;
}

bool Board::isCorner( Position p )
{
	vector< Position >::iterator location;

	for (location = boardCorners.begin(); location != boardCorners.end(); ++location ) {
		if ((location->row == p.row) && (location->col == p.col)) { return true; };
	}
	return false;
}

bool Board::isNextToCorner( Position p )
{
	vector< Position >::iterator location;

	for (location = boardAdjCorners.begin(); location != boardAdjCorners.end(); ++location ) {
		if ((location->row == p.row) && (location->col == p.col)) { return true; };
	}
	return false;
}

bool Board::isCenter( Position p )
{
	vector< Position >::iterator location;

	for (location = boardCenters.begin(); location != boardCenters.end(); ++location ) {
		if ((location->row == p.row) && (location->col == p.col)) { return true; };
	}
	return false;
}
