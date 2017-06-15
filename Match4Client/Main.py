import sys, random
from PyQt5.QtWidgets import QMainWindow, QFrame, QApplication, QDialog
from PyQt5.QtCore import Qt, QBasicTimer, pyqtSignal, pyqtSlot
from PyQt5 import QtCore

import ui.MainWindow
from MenuScreen import MenuScreen
from GameSettings import GameSettings, Player, PlayerType
from GameSettingsDialog import GameSettingsDialog
from GameScreen import GameScreen


class Main(QMainWindow, ui.MainWindow.Ui_MainWindow):
	globalMainInstance = None

	def __init__(self):
		super(Main, self).__init__()

		Main.globalMainInstance = self
		self.__menuScreen = MenuScreen(self)
		self.__gameScreen = None

		self.setupUi(self)

	def run(self):
		self.__menuScreen.endProgram.connect(self.endProgram)
		self.__menuScreen.startGame.connect(self.prepareGame)
		self.setCentralWidget(self.__menuScreen)
		self.show()

	@pyqtSlot(GameSettings)
	def prepareGame(self, gameSettings):
		# Show dialog
		game_setting_dialog = GameSettingsDialog(gameSettings, self)
		game_setting_dialog.settingsAccepted.connect(self.startGame)

		game_setting_dialog.show()

	@pyqtSlot()
	def endProgram(self):
		self.close()
		return

	@pyqtSlot(GameSettings)
	def startGame(self, gameSettings):
		# Create game screen and game server
		# Register message sender/receiver to server and game screen	
		self.__gameScreen = GameScreen(self)
		self.__gameScreen.endGame.connect(self.endGame)
		
		self.__menuScreen.hide()
		self.setCentralWidget(self.__gameScreen)
		self.__gameScreen.startGame(gameSettings)
		return

	@pyqtSlot()
	def endGame(self):
		self.__gameScreen.hide()
		self.__menuScreen = MenuScreen(self)
		self.__menuScreen.endProgram.connect(self.endProgram)
		self.__menuScreen.startGame.connect(self.prepareGame)
		self.setCentralWidget(self.__menuScreen)
		return

if __name__ == '__main__':
	app = QApplication([])
	game = Main()
	game.run()
	sys.exit(app.exec_())
