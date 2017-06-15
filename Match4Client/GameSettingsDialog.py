from PyQt5.QtWidgets import QFrame, QWidget, QDialog
from PyQt5.QtCore import QObject, pyqtSignal, pyqtSlot
from GameSettings import GameSettings, PlayerColor, PlayerType, Player

import ui.GameSettingsDialog


class GameSettingsDialog(QDialog, ui.GameSettingsDialog.Ui_Dialog):
    settingsAccepted = pyqtSignal(GameSettings)

    def __init__(self, gameSettings, parent=None):
        super(QDialog, self).__init__(parent)

        self.gameSettings = gameSettings
        addDiffPanel_1 = gameSettings.player_1.type == PlayerType.Cpu
        addDiffPanel_2 = gameSettings.player_2.type == PlayerType.Cpu

        self.setupUi(self, addDiffPanel_1, addDiffPanel_2)

        self.accepted.connect(self.startGame)

    def show(self):
        if (self.gameSettings.player_1.type == PlayerType.Human and
             self.gameSettings.player_2.type == PlayerType.Human):
            self.accept()
            return

        super(QDialog, self).show()
        return

    @pyqtSlot()
    def startGame(self):
        self.settingsAccepted.emit(self.gameSettings)
        return

    @pyqtSlot(int)
    def difficulty_cpu_1_changed(self, newDiff):
        self.gameSettings.player_1.difficulty = newDiff
        return

    @pyqtSlot(int)
    def difficulty_cpu_2_changed(self, newDiff):
        self.gameSettings.player_2.difficulty = newDiff
        return