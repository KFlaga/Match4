from PyQt5.QtWidgets import QFrame, QWidget
from PyQt5.QtCore import QObject, pyqtSignal, pyqtSlot
from GameSettings import GameSettings, PlayerColor, PlayerType, Player
from PyQt5 import QtGui
import Messages
from Messages import Message, MessageType, MessageReceiver, MessageSender
import ui.GameFrame
import ui.resources


class GameScreen(QFrame, ui.GameFrame.Ui_Frame):
    endGame = pyqtSignal()

    def __init__(self, parent=None):
        super(GameScreen, self).__init__(parent)
        self.setupUi(self)

        self.iconRed = QtGui.QIcon()
        self.iconRed.addPixmap(QtGui.QPixmap(":/fields/color_red.png"), QtGui.QIcon.Disabled, QtGui.QIcon.Off)
        self.iconYellow = QtGui.QIcon()
        self.iconYellow.addPixmap(QtGui.QPixmap(":/fields/color_red.png"), QtGui.QIcon.Disabled, QtGui.QIcon.Off)

        self.msgReceiver = MessageReceiver()
        self.msgSender = MessageSender(self.msgReceiver)

        self.currentState = AwaitingHumanMoveState(self)

    def startGame(self, gameSettings):
        self.setPlayerColor(gameSettings.player_1.color)
        self.setPlayerType(gameSettings.player_1.type)
        return

    def endClicked(self):
        self.endGame.emit()
        return

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


class AwaitingHumanMoveState(GameState):
    def __init__(self, parent):
        super(AwaitingHumanMoveState, self).__init__(parent)

    def begin(self):
        self.gameScreen.setStatus("Awaiting move")
        return

    def end(self):
        return

    def onColumnHovered(self, column):
        # set state : awaiting for message
        msg = Messages.createMessage(MessageType.SendColumnHovered, [column])
        self.gameScreen.msgSender.send(msg)
        return

    def onColumnSelected(self, column):
        # set state : awaiting for message
        msg = Messages.createMessage(MessageType.SendColumnSelected, [column])
        self.gameScreen.msgSender.send(msg)
        return


class AwaitingResponse(GameState):
    def __init__(self, parent):
        super(AwaitingResponse, self).__init__(parent)

    def begin(self):
        self.gameScreen.setStatus("Awaiting response")
        return

    def end(self):
        return

    def onColumnHovered(self, column):
        return

    def onColumnSelected(self, column):
        return
