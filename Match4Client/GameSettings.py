from enum import Enum


class PlayerColor(Enum):
    Red = 1
    Yellow = 2


class PlayerType(Enum):
    Human = 0
    Cpu = 1


class Player:
    def __init__(self, number, type, color):
        self.number = number
        self.type = type
        self.color = color
        self.difficulty = 0


class GameSettings:
    def __init__(self):
        self.player_1 = Player(0, PlayerType.Human, PlayerColor.Red)
        self.player_2 = Player(1, PlayerType.Human, PlayerColor.Yellow)
