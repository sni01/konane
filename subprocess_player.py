#!/usr/bin/env python
"""
"""

from game_rules import linearizeBoard, getLegalMoves
from player import Player
from subprocess import Popen, PIPE
from tempfile import TemporaryFile
from threading import Timer

import re

class ExternalPlayer(Player):
  def __init__(self, executable, symbol, timeout):
    super(ExternalPlayer, self).__init__(symbol)
    self.executable = executable[:-1]
    self.playerType = executable[-1]
    self.process = None
    self.timeout = timeout
    self.timer = None

  def interrupt(self):
    if self.process:
      print("Got SIGINT, killing child (process).")
      try:
        self.process.kill()
      except:
        pass
    if self.timer:
      self.timer.cancel()

  def selectInitialX(self, board):
    return self._sanitizedMove(board)

  def selectInitialO(self, board):
    return self._sanitizedMove(board)

  def getMove(self, board):
    return self._sanitizedMove(board)

  def _sanitizedMove(self, board):
    legalMoves = getLegalMoves(board, self.symbol)
    resultMove = self._parseMove(self._run(board))
    if resultMove in legalMoves:
      return resultMove
    else:
      return ((1, 2), (3, 4))

  def _run(self, board):
    self.process = Popen([self.executable, "-p"+ self.symbol, "-t"+ self.playerType, "-r"+ str(len(board)), "-c"+ str(len(board[0])), linearizeBoard(board)]
                         , stdout=PIPE)
    def killProcess():
      if self.process.poll() == None:
        try:
          self.process.kill()
          print('Error: player %s taking too long to complete--terminating' % self.symbol)
        except:
          pass  # if the self.process exits between the poll and the kill, that's fine, do nothing
    self.timer = Timer(self.timeout, killProcess)
    self.timer.start()
    self.process.wait()
    self.timer.cancel()
    out = self.process.stdout.read()
    # print(out)
    return out

  def _parseMove(self, move):
    return self._parseInitialMove(move) or self._parseSubsequentMove(move)

  def _parseInitialMove(self, move):
    match = re.match(r"^\((\d+), (\d+)\)$", move)
    return (int(match.group(1)), int(match.group(2))) if match else None

  def _parseSubsequentMove(self, move):
    match = re.match(r"^\(\((\d+), (\d+)\), \((\d+), (\d+)\)\)$", move)
    return ((int(match.group(1)), int(match.group(2)))
            , (int(match.group(3)), int(match.group(4)))) if match else None
