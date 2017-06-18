from enum import Enum
from PyQt5.QtCore import QObject, pyqtSlot, pyqtSignal


class MessageType(Enum):
    Unknown = 'Unknown'
    NoMessage = 'NoMessage'
    ReqHover = 'ReqHover'
    ReqSelect = 'ReqSelect'
    ReqSettings = 'ReqSetGameSettings'
    ReqEndGame = 'ReqEndGame'
    RespConfirm = 'RespConfirm'
    RespGoodMove = 'RespGoodMove'
    RespBadMove = 'RespBadMove'
    RespNextPlayer_Human = 'RespNextPlayer_Human'
    RespNextPlayer_Cpu = 'RespNextPlayer_Cpu'
    RespWinMove = 'RespWinMove'
    RespDraw = 'RespDraw'
    RespField = 'RespField'


class Message:
    def __init__(self, msgType=MessageType.Unknown):
        self.msgType = msgType
        self.data = [int(0), int(0), int(0)]
        return


def createMessage(msgType, msgData):
    msg = Message(msgType)
    dataLen = max([3, len(msgData)])
    for i in range(dataLen):
        msg.data[i] = msgData[i]
    return msg
