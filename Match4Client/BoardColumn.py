from BoardField import BoardField
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import pyqtSignal, pyqtSlot, QObject
from ui.BoardColumn import Ui_BoardColumn


class BoardColumn(QtWidgets.QPushButton, Ui_BoardColumn):
    mouseEnter = pyqtSignal(int)
    mouseLeave = pyqtSignal(int)
    mouseClicked = pyqtSignal(int)

    def __init__(self, parent, column_number):
        super(BoardColumn, self).__init__(parent)

        self.number = column_number
        self.setupUi(self, self.number)

        self.fields = [BoardField(self.field_column, i, column_number) for i in range(5)]
        for field in self.fields:
            self.verticalLayout.addWidget(field)
            field.mouseEnter.connect(self.fieldEnter)
            field.mouseLeave.connect(self.fieldLeave)
            field.mousePressed.connect(self.fieldPressed)
            field.mouseReleased.connect(self.fieldReleased)

        self.is_mouse_above = False

    @pyqtSlot(int)
    def fieldEnter(self, field_number):
        if not self.is_mouse_above:
            self.enterEvent(QtCore.QEvent(QtCore.QEvent.Enter))
        return

    @pyqtSlot(int)
    def fieldLeave(self, field_number):
        if not self.underMouse():
            self.leaveEvent(QtCore.QEvent(QtCore.QEvent.Leave))
        return

    @pyqtSlot(int)
    def fieldPressed(self, field_number):
        self.mousePressEvent(QtCore.QEvent(QtCore.QEvent.MouseButtonPress))
        return

    @pyqtSlot(int)
    def fieldReleased(self, field_number):
        if self.underMouse():
            self.mouseReleaseEvent(QtCore.QEvent(QtCore.QEvent.MouseButtonRelease))
        return

    def enterEvent(self, event):
        self.is_mouse_above = True
        self.mouseEnter.emit(self.number)
        return

    def leaveEvent(self, event):
        self.is_mouse_above = False
        self.mouseLeave.emit(self.number)
        return

    def mousePressEvent(self, mouseEvent):
        return

    def mouseReleaseEvent(self, mouseEvent):
        #if self.is_mouse_above:
        self.mouseClicked.emit(self.number)
        return

    def highlightNormal(self):
        self.setStyleSheet("background-image: url(:/fields/empty_column.png);")
        return

    def highlightHover(self):
        self.setStyleSheet("background-image: url(:/fields/empty_column_hover.png);")
        return

    def highlightBad(self):
        self.setStyleSheet("background-image: url(:/fields/empty_column_bad.png);")
        return

    def highlightGood(self):
        self.setStyleSheet("background-image: url(:/fields/empty_column_good.png);")
        return

    def setFieldColor(self, field_number, player_color):
        self.fields[field_number].setColor(player_color)
        return
