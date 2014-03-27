/*
 *  Player.cpp
 *  Konane
 *
 *  Created by ablumer on 12/26/13.
 *  Copyright 2013. All rights reserved.
 *
 */
 
#include <iostream>
using namespace std;
#include <cstdlib>

#include "Player.h"
#include "Board.h"

Player::Player()
{
}

/* Initialize an instance of Player:
 * myDepth is the maximum search depth for this player
 * myType is 'h' for human, 'r' for random, and 'm' for minimax
 * myPiece is 'x' or 'o' for the player's game piece
 * The last statement initializes the (very poor) random number generator
 */
Player::Player( char ptype, char piece, unsigned searchDepth )
{
	myType = ptype;
	myPiece = piece;
	myDepth = searchDepth;
	srand( time(0) );
}

/* An interactive version of checkLegalMove for a human player
 * Prints error messages rather than just returning "false"
 */
bool Player::checkLegalMove( Board b, Move m, char p) {
	int direction;
	int row, col;
	char opponent;
	Position pos;
	int r1, c1, r2, c2;
	
	if (b.getPiece( m.from ) != p) {
		cout << "You can only move your own pieces" << endl;
		return false;
	}
	if (b.getPiece( m.to ) != ' ') {
		cout << "Move must end at empty space" << endl;
		return false;
	}
	r1 = m.from.row; c1 = m.from.col; r2 = m.to.row; c2 = m.to.col;
	if ( ( ((r2-r1)%2) == 1 ) || ( ((c2-c1)%2) == 1 ) ) {
		cout << "Cannot move an odd number of squares" << endl;
		return false;
	}
	if (p == 'x') { opponent = 'o'; }
	else { opponent = 'x'; };
	if (r1 == r2) {
		if (c2 > c1) { direction = 1; }
		else { direction = -1; }
		col = c1;
		while (col != c2) {
			pos.row = r1;
			pos.col = col+direction;
			if (b.getPiece( pos ) != opponent) {
				cout << "Jumps must be over opponent pieces" << endl;
				return false;
			}
			col += (2*direction);
			pos.col = col;
			if (b.getPiece( pos ) != ' ') {
				cout << "Jumps must land on empty spaces" << endl;
				return false;
			}			
		}
	}
	else if (c1 == c2) {
		if (r2 > r1) { direction = 1; }
		else { direction = -1; }
		row = r1;
		while (row != r2) {
			pos.row = row+direction;
			pos.col = c1;
			if (b.getPiece( pos ) != opponent) {
				cout << "Jumps must be over opponent pieces" << endl;
				return false;
			}
			row += (2*direction);
			pos.row = row;
			if (b.getPiece( pos ) != ' ') {
				cout << "Jumps must land on empty spaces" << endl;
				return false;
			}			
		}
	}
	else {
		cout << "Cannot move on diagonal" << endl;
		return false;
	}
	return true;
}

/* A non-interactive version of checkLegalMove for a computer player
 * Does less error-checking, since it's assumed that a computer
 * player will only try to move its own pieces, will always end
 * on an empty square, and won't attempt to move on a diagonal
 */
bool Player::checkLegalMoveC( Board b, Move m, char p) {
	int direction;
	int row, col;
	char opponent;
	Position pos;
	int r1, c1, r2, c2;
	
	r1 = m.from.row; c1 = m.from.col; r2 = m.to.row; c2 = m.to.col;	
	if ( ( ((r2-r1)%2) == 1 ) || ( ((c2-c1)%2) == 1 ) ) {
		return false;
	}
	if (p == 'x') { opponent = 'o'; }
	else { opponent = 'x'; };
	if (r1 == r2) {
		if (c2 > c1) { direction = 1; }
		else { direction = -1; }
		col = c1;
		while (col != c2) {
			pos.row = r1;
			pos.col = col+direction;
			if (b.getPiece( pos ) != opponent) {
				return false;
			}
			col += (2*direction);
			pos.col = col;
			if (b.getPiece( pos ) != ' ') {
				return false;
			}			
		}
	}
	else if (c1 == c2) {
		if (r2 > r1) { direction = 1; }
		else { direction = -1; }
		row = r1;
		while (row != r2) {
			pos.row = row+direction;
			pos.col = c1;
			if (b.getPiece( pos ) != opponent) {
				return false;
			}
			row += (2*direction);
			pos.row = row;
			if (b.getPiece( pos ) != ' ') {
				return false;
			}			
		}
	}
	else {
		return false;
	}
	return true;
}

/* Find all legal moves for this player and store them in the
 * "legalMoves" array (vector).
 */
void Player::findMoves( Board theBoard, char piece ) {
	Position p1, p2;
	Move m;
	
	legalMoves.clear();
	for (int r1=1; r1<=theBoard.getRows(); r1++) {
		for (int c1=1; c1<=theBoard.getCols(); c1++) {
			p1.row = r1;
			p1.col = c1;
			if (piece == theBoard.getPiece( p1 )) {
				for (int r2=1; r2<=theBoard.getRows(); r2++) {
					p2.row = r2;
					p2.col = c1;
					if (theBoard.getPiece( p2 ) == ' ') {
						m.from = p1;
						m.to = p2;
						if (checkLegalMoveC( theBoard, m, piece)) {
							legalMoves.push_back( m );
						}
					}
				}
				for (int c2=1; c2<=theBoard.getCols(); c2++) {
					p2.row = r1;
					p2.col = c2;
					if (theBoard.getPiece( p2 ) == ' ') {
						m.from = p1;
						m.to = p2;
						if (checkLegalMoveC( theBoard, m, piece)) {
							legalMoves.push_back( m );
						}
					}
				}
			}
		}
	}
}

/* MinValue and MaxValue implement a minimax player with maximum
 * search depth specified by "maxDepth".  The heuristic for evaluating
 * board positions is the number of legal moves (numMoves) minus the
 * number of legal moves for the opponent (numOppMoves).
 */
int Player::MinValue( unsigned depth, unsigned maxDepth, Board b, Move m ) {
	Board newBoard = b;
	int width = newBoard.getRows();
	int length = newBoard.getCols();
	int value = (width * length * width * length);  // upper bound on number of legal moves
	char opponent;
	
	newBoard.makeMove( m );
	findMoves( newBoard, myPiece );
	int numMoves = legalMoves.size();
	legalMoves.clear();
	if (myPiece == 'x') { opponent = 'o'; }
	else { opponent = 'x'; };
	findMoves( newBoard, opponent );
	int numOppMoves = legalMoves.size();
	if (numOppMoves == 0) { return value; };  // game over, so return upper bound
	if (depth == maxDepth) {
		legalMoves.clear();
		return ( numMoves - numOppMoves );  // maxDepth, so return heuristic
	};
	if (numOppMoves == 1) {  // opponent has no choice
		legalMoves.clear();
		return MaxValue( depth+1, maxDepth, newBoard, legalMoves[0] );
	};	
	for (int i=0; i<numOppMoves; i++) {  // minimize the possibilities for Max
		int tempValue = MaxValue( depth+1, maxDepth, newBoard, legalMoves[i] );
		if (tempValue < value) { value = tempValue; };
	}
	legalMoves.clear();
	return value;
}

int Player::MaxValue( unsigned depth, unsigned maxDepth, Board b, Move m ) {
	Board newBoard = b;
	int width = newBoard.getRows();
	int length = newBoard.getCols();
	int value = -(width * length * width * length);  // negative of upper bound on number of legal moves
	char opponent;
	
	newBoard.makeMove( m );
	if (myPiece == 'x') { opponent = 'o'; }
	else { opponent = 'x'; };
	findMoves( newBoard, opponent );
	int numOppMoves = legalMoves.size();
	legalMoves.clear();
	findMoves( newBoard, myPiece );
	int numMoves = legalMoves.size();
	if (numMoves == 0) { return value; };  // game over, so return lower bound
	if (depth == maxDepth) {
		legalMoves.clear();
		return ( numMoves - numOppMoves );  // maxDepth, so return heuristic
	};
	if (numMoves == 1) {  // opponent has no choice
		legalMoves.clear();
		return MinValue( depth+1, maxDepth, newBoard, legalMoves[0] );
	};	
	for (int i=0; i<numMoves; i++) {  // maximize the possibilities for Min
		int tempValue = MinValue( depth+1, maxDepth, newBoard, legalMoves[i] );
		if (tempValue > value) { value = tempValue; };
	}
	legalMoves.clear();
	return value;
}

/* Make an initial move for the first player by removing a piece from a full board.
 * The piece removed can be either a corner piece or a center piece.
 */
void Player::firstMove( Board &theBoard, bool shouldPrintMove )
{
	int width = theBoard.getRows();
	int length = theBoard.getCols();
	Position firstMove;
	bool oddwidth = ( (width%2) == 1 );
	bool oddlength = ( (length%2) == 1 );
	Position p;
	char corner1, corner2, corner3;
	char reply;
	
	p.row = 1; p.col = 1;
	corner1 = theBoard.getPiece( p );
	p.row = width; p.col = 1;
	corner2 = theBoard.getPiece( p );
	p.row = 1; p.col = length;
	corner3 = theBoard.getPiece( p );	
	if ((myType == 'r') || (myType == 'm') || (myType == 'a')) {
		if (oddwidth && oddlength && (corner1 != myPiece) ) {
			if (length == 1) {
				firstMove.row = 2;
				firstMove.col = 1;
			}
			else {
				firstMove.row = 1;
				firstMove.col = 2;
			}
			theBoard.setFirstMove( firstMove );
		}
		else if (corner1 == myPiece) {
			firstMove.row = 1;
			firstMove.col = 1;
			theBoard.setFirstMove( firstMove );
		}
		else if (corner2 == myPiece) {
			firstMove.row = width;
			firstMove.col = 1;
			theBoard.setFirstMove( firstMove );
		}
		else if (corner3 == myPiece) {
			firstMove.row = 1;
			firstMove.col = length;
			theBoard.setFirstMove( firstMove );
		}
	}
	else if (myType == 'h') {
		do {
			cout << "First move at corner (c) or middle (m)? ";
			cin >> reply;
			if (reply == 'c') {
				if (oddwidth && oddlength && (corner1 != myPiece) ) {
					cout << "Sorry, no corner pieces are " << myPiece << endl;
				} else if (corner1 == myPiece) {
					firstMove.row = 1;
					firstMove.col = 1;
					theBoard.setFirstMove( firstMove );
				} else if (corner2 == myPiece) {
					firstMove.row = width;
					firstMove.col = 1;
					theBoard.setFirstMove( firstMove );
				} else if (corner3 == myPiece) {
					firstMove.row = 1;
					firstMove.col = length;
					theBoard.setFirstMove( firstMove );
				}
			}
			else if (reply == 'm') {
				firstMove.row = (1+width)/2;
				firstMove.col = (1+length)/2;
				if (theBoard.getPiece( firstMove ) != myPiece) {
					if (width == 1) {
						firstMove.col += 1;
					}
					else {
						firstMove.row += 1;
					}
				}
				theBoard.setFirstMove( firstMove );
			}
			else { cout << "Please enter c or m" << endl; }
		} while ( (reply != 'c') && (reply != 'm') );
	}
	if (shouldPrintMove) {
		cout << "(" << firstMove.row-1 << ", " << firstMove.col-1 << ")";
	}
}

/* Make an initial move for the second player by removing a piece adjacent
 * to the piece removed by the first player.
 */
bool Player::secondMove( Board &theBoard, bool shouldPrintMove ) {
	int width = theBoard.getRows();
	int length = theBoard.getCols();
	Position firstMove;
	Position secondMove;
	bool oddwidth = ( (width%2) == 1 );
	bool oddlength = ( (length%2) == 1 );
	char corner;
	int row, col;
	Position p;
	Position center;
	int rowdiff, coldiff;
	char piece;
	bool illegalMove;

	theBoard.getFirstMove( firstMove );
	// cout << firstMove.row-1 << " " << firstMove.col-1;
	if ((myType == 'r') || (myType == 'm') || (myType == 'a')) {
		if (oddwidth && oddlength) {
			center.row = (1+width)/2;
			center.col = (1+length)/2;
			rowdiff = center.row - firstMove.row;
			coldiff = center.col - firstMove.col;
			if ( (center.row == firstMove.row) && (center.col == firstMove.col) ) {
				secondMove = center;
				if (width == 1) {
					secondMove.col += 1;
				}
				else {
					secondMove.row += 1;
				}
			}
			else if ( (coldiff == 0) && ((rowdiff == 1) || (rowdiff == -1)) ) {
				secondMove = center;
			}
			else if ( (rowdiff == 0) && ((coldiff == 1) || (coldiff == -1)) ) {
				secondMove = center;
			}
			else {
				secondMove = firstMove;
				p.row = 1; p.col = 1;
				corner = theBoard.getPiece( p );
				if (corner == myPiece) {
					if (firstMove.row == 2) {
						secondMove.row = 1;
					}
					else if (firstMove.row == (width-1)) {
						secondMove.row = width;
					}
					else if (firstMove.col == 2) {
						secondMove.col = 1;
					}
					else if (firstMove.col == (length-1)) {
						secondMove.col = length;
					}
				}
				else {
					if (firstMove.row == 1) {
						if ((width == 1) && (firstMove.col == 1)) {
							secondMove.col = 2;
						}
						else if ((width == 1) && (firstMove.col == length)) {
							secondMove.col = length-1;
						}
						else {
							secondMove.row = 2;
						}
					}
					else if (firstMove.row == width) {
						if (firstMove.col == 1) {
							secondMove.col = 2;
						}
						else {
							secondMove.col = firstMove.col-1;
						}
					}
				}
			}
		}
		else {
			if (firstMove.row == 1) {
				secondMove.row = 2;
				secondMove.col = firstMove.col;
			}
			else if (firstMove.row == width) {
				secondMove.row = firstMove.row-1;
				secondMove.col = firstMove.col;
			}
			else if ( ((width%2) == 1) ) {
				if ( ((length%2) == 1) ) {
					secondMove.row = firstMove.row-1;
					secondMove.col = firstMove.col;
				}
				else {
					secondMove.row = firstMove.row;
					secondMove.col = (1+length)/2;
					if (secondMove.col == firstMove.col) {
						secondMove.col += 1;
					}
				}
			}
			else {
				secondMove.row = (1+width)/2;
				if (secondMove.row == firstMove.row) {
					secondMove.row += 1;
				}
				secondMove.col = firstMove.col;
			}
		}
		theBoard.setSecondMove( secondMove );
	}
	else if (myType == 'h') {
		do {
			illegalMove = false;
			cout << "Enter row and column for second move: ";
			cin >> row >> col;
			if ( (row < 1) || (col < 1) || (row > width) || (col > length) ) {
				cout << "row must be between 1 and " << width <<
					" and col must be between 1 and " << length << endl;
				illegalMove = true;
			}
			else {
				p.row = row; p.col = col;
				piece = theBoard.getPiece( p );
				if (piece != myPiece) {
					cout << "You must pick one of your pieces" << endl;
					illegalMove = true;
				}
				else if (row == firstMove.row) {
					if ( (col != (firstMove.col - 1)) && (col != (firstMove.col + 1)) ) {
						cout << "Your move must be adjacent to the first move" << endl;
						illegalMove = true;
					}
					else if (theBoard.isCenter(firstMove) && !theBoard.isCenter(p)) {
						cout << "You must pick a center piece" << endl;
						illegalMove = true;
					}
					else if (theBoard.isNextToCorner(firstMove) && !theBoard.isCorner(p)) {
						cout << "You must pick a corner piece" << endl;
						illegalMove = true;
					}
				}
				else if (col == firstMove.col) {
					if ( (row != (firstMove.row - 1)) && (row != (firstMove.row + 1)) ) {
						cout << "Your move must be adjacent to the first move" << endl;
						illegalMove = true;
					}
					else if (theBoard.isCenter(firstMove) && !theBoard.isCenter(p)) {
						cout << "You must pick a center piece" << endl;
						illegalMove = true;
					}
					else if (theBoard.isNextToCorner(firstMove) && !theBoard.isCorner(p)) {
						cout << "You must pick a corner piece" << endl;
						illegalMove = true;
					}
				}
				else {
					cout << "Your move must be adjacent to the first move" << endl;
					illegalMove = true;
				}
			}
		} while (illegalMove);
		secondMove.row = row;
		secondMove.col = col;
		theBoard.setSecondMove( secondMove );
	}
	if (shouldPrintMove) {
		cout << "(" << secondMove.row-1 << ", " << secondMove.col-1 << ")";
	}
	return false;
}

/* Make a move for either player.
 * If the player type is 'r' (random), a random legal move is selected.
 * If the player type is 'm' (minimax), a minimax optimal move is selected.
 * If the player type is 'a' (alpha-beta), an alpha-beta optimal move is selected.
 * If the player type is 'h', a move is requested from the human and
 *    checked for legality.
 * The return value is "true" if there are no legal moves.
 */
bool Player::nextMove( Board &theBoard, char mode, bool shouldPrintMove ) {
	bool gameOver;
	bool illegalMove;
	int numMoves;
	int width = theBoard.getRows();
	int length = theBoard.getCols();
	int row1, col1, row2, col2;
	char piece;
	Move bestMove;
	std::vector< Move > Actions;
	int bestValue;
	
	gameOver = false;
	if (myType == 'r') {
		findMoves( theBoard, myPiece );
		numMoves = legalMoves.size();
		if (mode == 'v') { cout << numMoves << " legal moves for " << myPiece << endl; };
		if (numMoves == 0) { return true; };
		int i = rand()%numMoves;
		bestMove = legalMoves[i];
		theBoard.makeMove( legalMoves[i] );
		legalMoves.clear();
	}
	else if (myType == 'm') {
		findMoves( theBoard, myPiece );
		numMoves = legalMoves.size();
		if (mode == 'v') { cout << numMoves << " legal moves for " << myPiece << endl; };
		if (numMoves == 0) { return true; };
		if (numMoves == 1) {
			bestMove = legalMoves[0];
		}
		else {
			bestValue = -(width * length * width * length + 1);
			Actions = legalMoves;
			for (int i=0; i<numMoves; i++) {
				int newValue = MinValue( 0, myDepth, theBoard, Actions[i] );
				if ( newValue > bestValue) {
					bestValue = newValue;
					bestMove = Actions[i];
				}
			}
		}
		theBoard.makeMove( bestMove );
		legalMoves.clear();
	}
	else if (myType == 'a') {
		findMoves( theBoard, myPiece );
		numMoves = legalMoves.size();
		if (mode == 'v') { cout << numMoves << " legal moves for " << myPiece << endl; };
		if (numMoves == 0) { return true; };
		if (numMoves == 1) {
			bestMove = legalMoves[0];
		}
		else {
			int alpha = -(width * length * width * length + 1);
			int beta = (width * length * width * length + 1);
			// You need to call alpha-beta here
		}
		theBoard.makeMove( bestMove );
		legalMoves.clear();
	}
	else if (myType == 'h') {
		do {
			illegalMove = false;
			cout << "Enter move for " << myPiece << " (r1, c1, r2, c2): ";
			cin >> row1 >> col1 >> row2 >> col2;
			if (cin.eof()) { return true; };
			if ( (row1 < 1) || (col1 < 1) || (row1 > width) || (col1 > length) ) {
				cout << "r1 must be between 1 and " << width <<
					" and c1 must be between 1 and " << length << endl;
				illegalMove = true;
			}
			else if ( (row2 < 1) || (col2 < 1) || (row2 > width) || (col2 > length) ) {
				cout << "r2 must be between 1 and " << width <<
					" and c2 must be between 1 and " << length << endl;
				illegalMove = true;
			}
			else {
				bestMove.from.row = row1;
				bestMove.from.col = col1;
				piece = theBoard.getPiece( bestMove.from );
				if (piece != myPiece) {
					cout << "You must pick one of your pieces" << endl;
					illegalMove = true;
				}
				else {
					bestMove.to.row = row2;
					bestMove.to.col = col2;
					if (!checkLegalMove( theBoard, bestMove, myPiece)) {
						illegalMove = true;
					}
					else {
						theBoard.makeMove( bestMove );
					}
				}
			}
		} while (illegalMove);
	}
	if (shouldPrintMove) {
		cout << "((" << bestMove.from.row-1 << ", " << bestMove.from.col-1
         << "), (" << bestMove.to.row-1 << ", " << bestMove.to.col-1 << "))";
	}
	return gameOver;
}
