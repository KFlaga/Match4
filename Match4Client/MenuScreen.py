from PyQt5.QtWidgets import QFrame, QWidget
from PyQt5.QtCore import QObject, pyqtSignal, pyqtSlot
from GameSettings import GameSettings, PlayerColor, PlayerType, Player

import ui.MenuFrame


class MenuScreen(QWidget, ui.MenuFrame.Ui_mainFrame):
    startGame = pyqtSignal(GameSettings)
    endProgram = pyqtSignal()

    def __init__(self, parent=None):
        super(MenuScreen, self).__init__(parent)
        self.setupUi(self)

    def humanVsHumanClicked(self):
        game_settings = GameSettings()
        game_settings.player_1 = Player(0, PlayerType.Human, PlayerColor.Red)
        game_settings.player_2 = Player(1, PlayerType.Human, PlayerColor.Yellow)
        self.startGame.emit(game_settings)
        return

    def humanVsCpuClicked(self):
        game_settings = GameSettings()
        game_settings.player_1 = Player(0, PlayerType.Human, PlayerColor.Red)
        game_settings.player_2 = Player(1, PlayerType.Cpu, PlayerColor.Yellow)
        self.startGame.emit(game_settings)
        return

    def cpuVsCpuClicked(self):
        game_settings = GameSettings()
        game_settings.player_1 = Player(0, PlayerType.Cpu, PlayerColor.Red)
        game_settings.player_2 = Player(1, PlayerType.Cpu, PlayerColor.Yellow)
        self.startGame.emit(game_settings)
        return

    def endClicked(self):
        self.endProgram.emit()
        return
