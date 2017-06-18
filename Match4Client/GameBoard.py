from BoardColumn import BoardColumn
from ui.GameBoard import Ui_GameBoard
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import pyqtSignal, pyqtSlot, QObject
from PyQt5.QtWidgets import QFrame


class GameBoard(QFrame, Ui_GameBoard):
    columnHover = pyqtSignal(int)
    columnClicked = pyqtSignal(int)

    def __init__(self, parent):
        super(GameBoard, self).__init__(parent)

        self.columns = [BoardColumn(self, i) for i in range(5)]
        self.setupUi(self)

        for column in self.columns:
            column.mouseEnter.connect(self.onColumnHoverEnter)
            column.mouseLeave.connect(self.onColumnHoverLeave)
            column.mouseClicked.connect(self.onColumnClicked)

    @pyqtSlot(int)
    def onColumnHoverEnter(self, column_number):
        self.columnHover.emit(column_number)
        return

    @pyqtSlot(int)
    def onColumnHoverLeave(self, column_number):
        self.columns[column_number].highlightNormal()
        return

    @pyqtSlot(int)
    def onColumnClicked(self, column_number):
        self.columnClicked.emit(column_number)
        return

    @pyqtSlot()
    def resetHover(self):
        for column in self.columns:
            column.highlightNormal()
        return

    @pyqtSlot(int)
    def setHoverNormal(self, column_number):
        self.columns[column_number].highlightNormal()
        return

    @pyqtSlot(int)
    def setHoverGood(self, column_number):
        self.resetHover()
        self.columns[column_number].highlightGood()
        return

    @pyqtSlot(int)
    def setHoverBad(self, column_number):
        self.resetHover()
        self.columns[column_number].highlightBad()
        return

    @pyqtSlot(int, int, int)
    def setFieldColor(self, column_number, field_number, player_color):
        self.columns[column_number].setFieldColor(field_number, player_color)
        return
