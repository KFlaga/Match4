from ui.BoardField import Ui_BoardField
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import pyqtSignal, pyqtSlot, QObject
from GameSettings import PlayerColor


class BoardField(QtWidgets.QPushButton, Ui_BoardField):
    mouseEnter = pyqtSignal(int)
    mouseLeave = pyqtSignal(int)
    mousePressed = pyqtSignal(int)
    mouseReleased = pyqtSignal(int)

    def __init__(self, parent, field_number, column_number):
        super(BoardField, self).__init__(parent)
        self.field_number = field_number
        self.column_number = column_number
        self.setupUi(self, field_number, column_number)
        self.color = PlayerColor.NoPlayer

    def enterEvent(self, event):
        self.mouseEnter.emit(self.field_number)
        return

    def leaveEvent(self, event):
        self.mouseLeave.emit(self.field_number)
        return

    def mousePressEvent(self, mouseEvent):
        self.mousePressed.emit(self.field_number)
        return

    def mouseReleaseEvent(self, mouseEvent):
        self.mouseReleased.emit(self.field_number)
        return

    def setColor(self, color):
        if color == PlayerColor.Red:
            self.setStyleSheet("background-color: rgba(255, 255, 255, 0);\n"
                               "background-image: url(:/fields/red_field_1.png);")
        elif color == PlayerColor.Yellow:
            self.setStyleSheet("background-color: rgba(255, 255, 255, 0);\n"
                               "background-image: url(:/fields/yellow_field_1.png);")
        else:
            self.setStyleSheet("background-color: rgba(255, 255, 255, 0);\n"
                               "background-image: url(:/fields/empty_field_1.png);")
        self.color = color
        return

    def getColor(self):
        return self.color
