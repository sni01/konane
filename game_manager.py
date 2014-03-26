#!/usr/bin/env python

from copy import deepcopy
import game_rules

# Game state constants
AWAITING_INITIAL_X = -1
AWAITING_INITIAL_O = 0
X_TURN = 1
O_TURN = 2
X_VICTORY = 3
O_VICTORY = 4

class GameManager:
  def __init__(self, rows, cols, player1, player2, verbose=False):
    self.rows = rows
    self.cols = cols
    self.p1 = player1
    self.p2 = player2
    self.reset()
    self.verbose = verbose

  def reset(self):
    self.state = AWAITING_INITIAL_X
    self.board = game_rules.makeBoard(self.rows, self.cols)

  def play(self):
    while self.state is not X_VICTORY and self.state is not O_VICTORY:
      self._takeTurn()
      if self.verbose:
        game_rules.printBoard(self.board)

  def _takeTurn(self):
    playerBoard = deepcopy(self.board)
    if self.state == AWAITING_INITIAL_X:
      self._handleInitialX(playerBoard, self.board)
    elif self.state == AWAITING_INITIAL_O:
      self._handleInitialO(playerBoard, self.board)
    elif self.state == X_TURN:
      self._handleTurnX(playerBoard, self.board)
    elif self.state == O_TURN:
      self._handleTurnO(playerBoard, self.board)

  def _handleInitialX(self, playerBoard, board):
    pt = self.p1.selectInitialX(playerBoard)
    if pt in game_rules.getFirstMovesForX(board):
      self.board[pt[0]][pt[1]] = " "
      self.state = AWAITING_INITIAL_O
    else:
      self.state = O_VICTORY

  def _handleInitialO(self, playerBoard, board):
    pt = self.p2.selectInitialO(playerBoard)
    if pt in game_rules.getFirstMovesForO(board):
      self.board[pt[0]][pt[1]] = " "
      self.state = X_TURN
    else:
      self.state = X_VICTORY

  def _handleTurnX(self, playerBoard, board):
    self.state = O_TURN
    (self.board, legal) = game_rules.makePlayerMove(board, 'x', self.p1.getMove(playerBoard))
    if not legal:
      self.state = O_VICTORY

  def _handleTurnO(self, playerBoard, board):
    self.state = X_TURN
    (self.board, legal) = game_rules.makePlayerMove(board, 'o', self.p2.getMove(playerBoard))
    if not legal:
      self.state = X_VICTORY
