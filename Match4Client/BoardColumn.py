from ui.BoardColumn import Ui_BoardColumn
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import pyqtSignal, pyqtSlot


class BoardColumn(QtWidgets.QPushButton, Ui_BoardColumn)
	def __init__(self, parent, column_number)
		super(BoardColumn, self).__init__(parent)
		
		self.number = column_number
		self.setupUi(self, self.number)
		
		for i in range(5)
			self.exitButton.hover.connect(self.hover)
			self.exitButton.clicked.connect(self.clicked)
			
	@pyqtSlot()
	def onHoverEnter(self)
		return
		
	@pyqtSlot()
	def onHoverLeave(self)
		return
		
	@pyqtSlot()
	def onPressed(self)
		return
		
	@pyqtSlot()
	def onReleased(self)
		return
		
	@pyqtSlot()
	def onClicked(self)
		return