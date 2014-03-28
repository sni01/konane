#!/usr/bin/env python
"""Tufts Comp 131 Stand Alone Konane Player

Running this file directly computes a single turn for the specified player.

Usage:
  ./player.py (-p <player>) [-t <type>] (-r <rows>) (-c <cols>) <board>

Options:
  -p <player>     Which player (x or o) the player is playing for?
  -t <type>       What player type for this player?  [default: A]
  -r <rows>       Sets the number of rows of the board.
  -c <cols>       Sets the number of columns of the board.
"""

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
  def __init__(self, symbol):
    super(MinimaxPlayer, self).__init__(symbol)
    self.depthLimit = 2  # TODO: fix me

  def selectInitialX(self, board):
    return (0, 0)

  def selectInitialO(self, board):
    validMoves = game_rules.getFirstMovesForO(board)
    return random.choice(list(validMoves))

  def getMove(self, board):
    #  This is how I would do the assignment, however Anselm asked me to
    #  make it look like the pseudocode in the book...
    return self._negamax(board, self.depthLimit, self.symbol)[1]
    # ... so if you care about it looking like the book, comment out the
    # above line and uncomment the next one.
    # return self._minimaxDecision(board, self.symbol)

  def _negamax(self, board, depth, symbol):
    legalMoves = game_rules.getLegalMoves(board, symbol)
    if depth == 0 or len(legalMoves) == 0:
      return self._assessBoard(board, symbol)
    best = (-1000000000, None)
    for move in legalMoves:
      child = game_rules.makeMove(board, move)
      (score, _) = self._negamax(child, depth-1, 'o' if symbol == 'x' else 'x')
      best = max(best, (-score, move))
    return best
    
  def _assessBoard(self, board, symbol):
    #return (1 if self.symbol == symbol else -1, board)  # TODO: write a better heuristic
    heuristic = self.Heuristic(board)
    return (heuristic, board)

  def _minimaxDecision(self, board, symbol):
    return self._maxValue(board, self.depthLimit, symbol)[1]

  def _maxValue(self, board, depth, symbol):
    legalMoves = game_rules.getLegalMoves(board, symbol)
    if depth == 0 or len(legalMoves) == 0:
      return (self._assessBoard(board, symbol), None)
    best = (-1000000000, None)
    for move in legalMoves:
      child = game_rules.makeMove(board, move)
      (score, _) = self._minValue(child, depth-1, 'o' if symbol == 'x' else 'x')
      best = max(best, (score, move))
    return best

  def _minValue(self, board, depth, symbol):
    legalMoves = game_rules.getLegalMoves(board, symbol)
    if depth == 0 or len(legalMoves) == 0:
      return (self._assessBoard(board, symbol), None)
    best = (1000000000, None)
    for move in legalMoves:
      child = game_rules.makeMove(board, move)
      (score, _) = self._minValue(child, depth-1, 'o' if symbol == 'x' else 'x')
      best = min(best, (score, move))
    return best

  def Heuristic(self, board):
    countCoins = [(r, c) for r in range(len(board)) for c in range(len(board[0])) if game_rules.pieceAt(board, (r, c)) == self.symbol]
    return len(countCoins)



class AlphaBetaPlayer(Player):
  def __init__(self, symbol):
    super(AlphaBetaPlayer, self).__init__(symbol)
    self.depthLimit = 2
  # initial methods for x player the same as ramdom x player initial method
  def selectInitialX(self, board):
    validMoves = game_rules.getFirstMovesForX(board)
    return random.choice(list(validMoves))

  # initial methods for o player the same as ramdom o player initial method
  def selectInitialO(self, board):
    validMoves = game_rules.getFirstMovesForO(board)
    return random.choice(list(validMoves))

  def Max_Value(self, board, a, b, depthLimit, symbol):
    #set initial max value to minus infinity
    val = -100000
    #get legalmoves
    legalMoves = self.getLegalMove(board)
    #return utility when no more legalmoves
    values = []
    if(len(legalMoves) == 0):
      val = 100000
      return (val,((-1, -1), (-2, -2)))
    for i in range(len(legalMoves)):
      nextBoard = deepcopy(board)
      nextBoard = game_rules.makeMove(nextBoard, legalMoves[i])

      #when meet depth limit, then use heuristic function to replace the val Min_Value
      Min = 100000
      if(depthLimit == 0):
        Min = self.Heuristic(nextBoard)
      else:
        Min = self.Min_Value(nextBoard,a,b,depthLimit-1,'o' if symbol == 'x' else 'x')[0]
      val = max(val, Min)
      values.append(val)
      if(val >= b):
        return (val,((-1, -1), (-2, -2)))
      else:
        a = max(a, val)
    for i in range(len(values)):
      if (values[i] == val):
        pos = i
    return (val,legalMoves[i])

  def Min_Value(self, board, a, b, depthLimit, symbol):
    #set initial min value to infinity
    val = 100000
    #get legalmoves
    legalMoves = self.getLegalMove(board)
    #return utility when no more legalmoves
    values = []
    if(len(legalMoves) == 0):
      val = -100000
      return (val,((-1, -1), (-2, -2)))
    for i in range(len(legalMoves)):
      nextBoard = deepcopy(board)
      nextBoard = game_rules.makeMove(nextBoard, legalMoves[i])

      #when meet depth limit, then use heuristic function to replace the val Max_Value
      Max = -100000
      if(depthLimit == 0):
        Max = self.Heuristic(nextBoard)
      else:
        Max = self.Max_Value(nextBoard,a,b,depthLimit-1,'o' if symbol == 'x' else 'x')[0]
      val = min(val, Max)
      values.append(Max)
      if(val <= a):
        return (val,((-1, -1), (-2, -2)))
      else:
        b = min(b, val)
    for i in range(len(values)):
      if (values[i] == val):
        pos = i
    return (val,legalMoves[i])

  def Heuristic(self, board):
    countCoins = [(r, c) for r in range(len(board)) for c in range(len(board[0])) if game_rules.pieceAt(board, (r, c)) == self.symbol]
    return len(countCoins)

  def Heuristic_2(self, board):
    legalmoves = self.getLegalMove(board)
    return len(legalmoves)

  def getLegalMove(self, board):
    mine = [(r, c) for r in range(len(board)) for c in range(len(board[0])) if game_rules.pieceAt(board, (r, c)) == self.symbol]
    allMoves = [(o, d) for o in mine for d in game_rules.getEmptySquares(board)]
    legalMoves = list(filter(lambda move: game_rules.isLegalMove(board, self.symbol, move, False), allMoves))
    return legalMoves

  def AlphaBetaSearch(self, board):
    # TODO: write me
    a = -100000
    b = 100000
    return self.Max_Value(board, a, b, self.depthLimit,self.symbol)

  def getMove(self, board):
    choice = self.AlphaBetaSearch(board)
    return choice[1]


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
    legalMoves = game_rules.getLegalMoves(board, self.symbol)
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


def makePlayer(playerType, symbol, timeout=0):
  import os
  from subprocess_player import ExternalPlayer
  if playerType[0] == 'H' or playerType[0] == 'h':
    return HumanPlayer(symbol)
  elif playerType[0] == 'R' or playerType[0] == 'r':
    return RandomPlayer(symbol)
  elif playerType[0] == 'M' or playerType[0] == 'm':
    return MinimaxPlayer(symbol)
  elif playerType[0] == 'A' or playerType[0] == 'a':
    return AlphaBetaPlayer(symbol)
  elif os.path.exists(playerType[:-1]):
    return ExternalPlayer(playerType, symbol, timeout)
  else:
    print("Unrecognized playerType %s for player %s" % (playerType, symbol))

def callMoveFunction(player, board):
  if game_rules.isInitialMove(board):
    if player.symbol == 'x':
      return player.selectInitialX(board)
    else:
      return player.selectInitialO(board)
  else:
    return player.getMove(board)

if __name__ == "__main__":
  # parse the arguments
  from docopt import docopt
  args = docopt(__doc__, version="Konane Player v1.0")
  # create a Player
  from konane import makePlayer
  player = makePlayer(args["-t"], args["-p"])
  # create a board
  rows = int(args["-r"])
  cols = int(args["-c"])
  board = game_rules.delinearizeBoard(args["<board>"], rows, cols)
  # call the appropriate select move function
  print(callMoveFunction(player, board))
