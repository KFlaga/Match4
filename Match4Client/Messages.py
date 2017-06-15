from enum import Enum
from PyQt5.QtCore import QObject, pyqtSlot, pyqtSignal


class MessageType(Enum):
    Unknown = 0
    SendColumnHovered = 1
    SendColumnSelected = 2
    SendEndGame = 3
    RecGoodMove = 4
    RecBadMove = 5
    RecNextTurn = 6
    RecCpuTurn = 7
    RecCpuMove = 8
    RecWinMove = 9


class Message:
    def __init__(self):
        self.msgType = int(MessageType.Unknown)
        self.data = [int(0), int(0), int(0)]
        return


def createMessage(msgType, msgData):
    msg = Message()
    msg.msgType = int(msgType)
    dataLen = max([3, len(msgData)])
    for i in range(dataLen):
        msg.data[i] = msgData[i]
    return msg


class MessageSender(QObject):
    def __init__(self, msgReceiver):
        super(QObject, self)
        self.receiver = msgReceiver
        return

    @pyqtSlot(Message)
    def send(self, msg):
        self.receiver.receive(msg)
        return


class MessageReceiver:
    def __init__(self):
        return

    def receive(self, msg):
        return
