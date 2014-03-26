#!/usr/bin/env python

import game_rules
import random
from copy import deepcopy
###########################################################################
# Explanation of the types:
# The board is represented by a row-major 2D list of characters, 0 indexed
# A point is a tuple of (int, int) representing (row, column)
# A move is a tuple of (point, point) representing (origin, destination)
# A jump is a move of length 2
###########################################################################

class Player(object):
  """ This is the player interface that is consumed by the GameManager.
  """
  def __init__(self, symbol):
    self.symbol = symbol  # 'x' or 'o'

  def selectInitialX(self, board):
    return (0, 0)

  def selectInitialO(self, board):
    pass

  def getMove(self, board):
    pass


class MinimaxPlayer(Player):
  def __init__(self, symbol, depth):
    super(MinimaxPlayer, self).__init__(symbol)
    self.depthLimit = depth

  # initial methods for x player the same as ramdom x player initial method
  def selectInitialX(self, board):
    validMoves = game_rules.getFirstMovesForX(board)
    return random.choice(list(validMoves))

  # initial methods for o player the same as ramdom o player initial method
  def selectInitialO(self, board):
    validMoves = game_rules.getFirstMovesForO(board)
    return random.choice(list(validMoves))

  # getMove_Recursive function implements the function of minimax algorithm with heuristic cutoff = depth
  def getMove_Recursive(self, board):

    mine = [(r, c) for r in range(len(board)) for c in range(len(board[0])) if game_rules.pieceAt(board, (r, c)) == self.symbol]
    allMoves = [(o, d) for o in mine for d in game_rules.getEmptySquares(board)]
    legalMoves = list(filter(lambda move: game_rules.isLegalMove(board, self.symbol, move, False), allMoves))

    choice = ()
    if(len(legalMoves) == 0):
      return (((0, 1), (2, 3)),0)


    max = 0
    min = 100000
    values = []
    for i in range(len(legalMoves)):
      nextBoard = deepcopy(board)
      nextBoard = game_rules.makeMove(nextBoard, legalMoves[i])
      nextBoard = nextBoard[0]

      heuristic = -1
      if(self.depthLimit == 0):
        countCoins = [(r, c) for r in range(len(nextBoard)) for c in range(len(nextBoard[0])) if game_rules.pieceAt(nextBoard, (r, c)) == self.symbol]
        heuristic = len(countCoins)
      else:
        if(self.symbol == 'x'):
          MP = MinimaxPlayer('o', self.depthLimit - 1)
          heuristic = MP.getMove_Recursive(nextBoard)[1]
        else:
          MP = MinimaxPlayer('x', self.depthLimit - 1)
          heuristic = MP.getMove_Recursive(nextBoard)[1]
      values.append(heuristic)
    if(self.symbol == 'x'):
      for i in range(len(values)):
        if(values[i] >= max):
          max = values[i]
          choice = legalMoves[i]
    else:
      for i in range(len(values)):
        if(values[i] <= min):
          min = values[i]
          choice = legalMoves[i]
    choice = (choice, i)
    return choice
  
  # getMove function return the movement this player choose
  def getMove(self, board):
    return self.getMove_Recursive(board)[0]

class RandomPlayer(Player):
  def __init__(self, symbol):
    super(RandomPlayer, self).__init__(symbol)

  def selectInitialX(self, board):
    validMoves = game_rules.getFirstMovesForX(board)
    return random.choice(list(validMoves))

  def selectInitialO(self, board):
    validMoves = game_rules.getFirstMovesForO(board)
    return random.choice(list(validMoves))

  def getMove(self, board):
    mine = [(r, c) for r in range(len(board)) for c in range(len(board[0])) if game_rules.pieceAt(board, (r, c)) == self.symbol]
    allMoves = [(o, d) for o in mine for d in game_rules.getEmptySquares(board)]
    legalMoves = list(filter(lambda move: game_rules.isLegalMove(board, self.symbol, move, False), allMoves))
    if len(legalMoves) > 0:
      return random.choice(legalMoves)
    else:
      return ((0, 1), (2, 3))


class HumanPlayer(Player):
  def __init__(self, symbol):
    super(HumanPlayer, self).__init__(symbol)

  def _promptForPoint(self, prompt):
    raw = raw_input(prompt)
    (r, c) = raw.split()
    return (int(r), int(c))

  def selectInitialX(self, board):
    game_rules.printBoard(board)
    pt = (0, 0, 0)  # obviously not a valid location on a 2-D board
    validMoves = game_rules.getFirstMovesForX(board)
    while pt not in validMoves:
      pt = self._promptForPoint("Enter a valid starting location for player X (in the format 'row column'): ")
    return pt

  def selectInitialO(self, board):
    game_rules.printBoard(board)
    pt = (0, 0, 0)  # obviously not a valid location on a 2-D board
    validMoves = game_rules.getFirstMovesForO(board)
    while pt not in validMoves:
      pt = self._promptForPoint("Enter a valid starting location for player O (in the format 'row column'): ")
    return pt

  def getMove(self, board):
    game_rules.printBoard(board)
    origin = self._promptForPoint("Choose a piece to move for %s (in the format 'row column'): " % self.symbol.capitalize())
    destination = self._promptForPoint("Choose a destination for %s (%s, %s) -> " % (self.symbol.capitalize(), origin[0], origin[1]))
    return (origin, destination)
