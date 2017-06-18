from PyQt5.QtWidgets import QFrame, QWidget
from PyQt5.QtCore import QObject, pyqtSignal, pyqtSlot
from GameSettings import GameSettings, PlayerColor, PlayerType, Player
from PyQt5 import QtGui
import Messages
from Messages import Message, MessageType
import ui.GameFrame
import ui.resources
from GameBoard import GameBoard
from BoardField import BoardField
from ServerConnector import ServerConnector


class GameScreen(QFrame, ui.GameFrame.Ui_GameFrame):
    endGame = pyqtSignal()

    def __init__(self, parent=None):
        super(GameScreen, self).__init__(parent)
        self.board = GameBoard(self)
        self.setupUi(self, self.board)

        self.iconRed = QtGui.QIcon()
        self.iconRed.addPixmap(QtGui.QPixmap(":/fields/color_red.png"), QtGui.QIcon.Disabled, QtGui.QIcon.Off)
        self.iconYellow = QtGui.QIcon()
        self.iconYellow.addPixmap(QtGui.QPixmap(":/fields/color_yellow.png"), QtGui.QIcon.Disabled, QtGui.QIcon.Off)

        self.board.columnHover.connect(self.columnHovered)
        self.board.columnClicked.connect(self.columnSelected)

        self.currentState = None
        self.changeState(AwaitingGameConfirmation(self))
        self.gameSettings = None
        self.currentPlayer = 0

        self.messageInterpreters = {
            MessageType.NoMessage: Interpreter_NoMessage(self),
            MessageType.RespConfirm: Interpreter_RespConfirm(self),
            MessageType.RespGoodMove: Interpreter_RespGoodMove(self),
            MessageType.RespBadMove: Interpreter_RespBadMove(self),
            MessageType.RespNextPlayer_Human: Interpreter_RespNextPlayer_Human(self),
            MessageType.RespNextPlayer_Cpu: Interpreter_RespNextPlayer_Cpu(self),
            MessageType.RespWinMove: Interpreter_RespWinMove(self),
            MessageType.RespDraw: Interpreter_RespDraw(self),
            MessageType.RespField: Interpreter_RespField(self)
        }

        self.serverConnector = ServerConnector()
        self.serverConnector.messageReceived.connect(self.messageReceived)

    def startGame(self, gameSettings):
        self.gameSettings = gameSettings
        self.currentPlayer = 1
        self.serverConnector.createServer()
        self.sendMessage(Messages.createMessage(
            MessageType.ReqSettings,
            [gameSettings.player_1.difficulty,
             gameSettings.player_2.difficulty, 0]))
        return

    def endClicked(self):
        self.serverConnector.destroyServer()
        self.endGame.emit()
        return

    def setCurrentPlayer(self, playerNum):
        self.currentPlayer = playerNum
        self.setPlayerColor(self.gameSettings.getPlayer(playerNum).color)
        self.setPlayerType(self.gameSettings.getPlayer(playerNum).type)

    def setPlayerColor(self, playerColor):
        if playerColor == PlayerColor.Red:
            self.currentPlayerColor.setIcon(self.iconRed)
        else:
            self.currentPlayerColor.setIcon(self.iconYellow)

    def setPlayerType(self, playerType):
        if playerType == PlayerType.Human:
            self.currentPlayerType.setText("Human")
        else:
            self.currentPlayerType.setText("Cpu")

    def setStatus(self, statusText):
        self.statusText.setText(statusText)

    @pyqtSlot(int)
    def columnHovered(self, column):
        self.currentState.onColumnHovered(column)
        return

    @pyqtSlot(int)
    def columnSelected(self, column):
        self.currentState.onColumnSelected(column)
        return

    @pyqtSlot(Message)
    def messageReceived(self, message):
        self.messageInterpreters[message.msgType].interpretMessage(message)
        return

    @pyqtSlot(Message)
    def sendMessage(self, message):
        self.serverConnector.sendToServer(message)
        return

    def changeState(self, state):
        if self.currentState is not None:
            self.currentState.end()
        self.currentState = state
        self.currentState.begin()

    def makeMove(self, column, player):
        for field in reversed(self.board.columns[column].fields):
            if field.getColor() is PlayerColor.NoPlayer:
                field.setColor(self.gameSettings.getPlayer(player).color)
                break
        return

    def nextPlayer(self):
        player = 0
        if self.currentPlayer == 0:
            player = 1
        self.setCurrentPlayer(player)

    def acceptMove(self, column):
        self.currentState.acceptMove(column)
        return

    def rejectMove(self, column):
        self.currentState.rejectMove(column)
        return

class GameState:
    def __init__(self, parent):
        self.gameScreen = parent

    def begin(self):
        return

    def end(self):
        return

    def onColumnHovered(self, column):
        return

    def onColumnSelected(self, column):
        return

    def acceptMove(self, column):
        return

    def rejectMove(self, column):
        return


class AwaitingGameConfirmation(GameState):
    def __init__(self, parent):
        super(AwaitingGameConfirmation, self).__init__(parent)

    def begin(self):
        self.gameScreen.setStatus("Awaiting for game server connection")
        return

    def end(self):
        self.gameScreen.setStatus("Game started")
        return


class AwaitingHumanMoveState(GameState):
    def __init__(self, parent):
        super(AwaitingHumanMoveState, self).__init__(parent)

    def begin(self):
        self.gameScreen.setStatus("Awaiting human move")
        return

    def end(self):
        return

    def onColumnHovered(self, column):
        self.gameScreen.changeState(AwaitingResponseHover(self.gameScreen))
        msg = Messages.createMessage(MessageType.ReqHover, [column, 0, 0])
        self.gameScreen.sendMessage(msg)
        return

    def onColumnSelected(self, column):
        self.gameScreen.changeState(AwaitingResponseSelect(self.gameScreen))
        msg = Messages.createMessage(MessageType.ReqSelect, [column, 0, 0])
        self.gameScreen.sendMessage(msg)
        return


class AwaitingCpuMoveState(GameState):
    def __init__(self, parent):
        super(AwaitingCpuMoveState, self).__init__(parent)

    def begin(self):
        self.gameScreen.setStatus("Awaiting cpu move")
        return

    def acceptMove(self, column):
        self.gameScreen.makeMove(column, self.gameScreen.currentPlayer)
        return

    def rejectMove(self, column):
        return


class AwaitingResponseHover(GameState):
    def __init__(self, parent):
        super(AwaitingResponseHover, self).__init__(parent)

    def begin(self):
        self.gameScreen.setStatus("Awaiting hover confirmation")
        return

    def acceptMove(self, column):
        self.gameScreen.board.setHoverGood(column)
        self.gameScreen.changeState(AwaitingHumanMoveState(self.gameScreen))
        return

    def rejectMove(self, column):
        self.gameScreen.board.setHoverBad(column)
        self.gameScreen.changeState(AwaitingHumanMoveState(self.gameScreen))
        return


class AwaitingResponseSelect(GameState):
    def __init__(self, parent):
        super(AwaitingResponseSelect, self).__init__(parent)

    def begin(self):
        self.gameScreen.setStatus("Awaiting move confirmation")
        return

    def acceptMove(self, column):
        self.gameScreen.board.setHoverGood(column)
        self.gameScreen.makeMove(column, self.gameScreen.currentPlayer)
        self.gameScreen.columnHovered(column)
        return

    def rejectMove(self, column):
        self.gameScreen.board.setHoverBad(column)
        self.gameScreen.changeState(AwaitingHumanMoveState(self.gameScreen))
        return


class MessageInterpreter:
    def __init__(self, parent):
        self.gameScreen = parent

    def interpretMessage(self, message):
        return


class Interpreter_NoMessage(MessageInterpreter):
    def __init__(self, parent):
        super().__init__(parent)

    def interpretMessage(self, message):
        return


class Interpreter_RespConfirm(MessageInterpreter):
    def __init__(self, parent):
        super().__init__(parent)

    def interpretMessage(self, message):
        return


class Interpreter_RespGoodMove(MessageInterpreter):
    def __init__(self, parent):
        super().__init__(parent)

    def interpretMessage(self, message):
        self.gameScreen.acceptMove(message.data[0])
        return


class Interpreter_RespBadMove(MessageInterpreter):
    def __init__(self, parent):
        super().__init__(parent)

    def interpretMessage(self, message):
        self.gameScreen.rejectMove(message.data[0])
        return


class Interpreter_RespNextPlayer_Human(MessageInterpreter):
    def __init__(self, parent):
        super().__init__(parent)

    def interpretMessage(self, message):
        self.gameScreen.changeState(AwaitingHumanMoveState(self.gameScreen))
        self.gameScreen.nextPlayer()
        return


class Interpreter_RespNextPlayer_Cpu(MessageInterpreter):
    def __init__(self, parent):
        super().__init__(parent)

    def interpretMessage(self, message):
        self.gameScreen.changeState(AwaitingCpuMoveState(self.gameScreen))
        self.gameScreen.nextPlayer()
        return


class Interpreter_RespWinMove(MessageInterpreter):
    def __init__(self, parent):
        super().__init__(parent)

    def interpretMessage(self, message):
        return


class Interpreter_RespDraw(MessageInterpreter):
    def __init__(self, parent):
        super().__init__(parent)

    def interpretMessage(self, message):
        return


class Interpreter_RespField(MessageInterpreter):
    def __init__(self, parent):
        super().__init__(parent)

    def interpretMessage(self, message):
        return
