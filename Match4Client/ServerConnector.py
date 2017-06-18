import match4server
from Messages import Message, MessageType
import Messages
from PyQt5.QtCore import QObject, pyqtSignal, pyqtSlot, QTimer


class ServerConnector(QObject):
    messageReceived = pyqtSignal(Message)

    def __init__(self):
        super(ServerConnector, self).__init__()
        self.createMessageTypeMap()
        self.serverHandle = 0

        self.messageAwaitTimer = QTimer()
        self.messageAwaitTimer.timeout.connect(self.__checkMessages)

    def createMessageTypeMap(self):
        self.msgClientToServerMap = {}
        self.msgServerToClientMap = {}
        for clientType in MessageType:
            typeName = str(clientType.value)
            serverType = match4server.getMessageType(typeName)
            self.msgClientToServerMap.update([(clientType, serverType)])
            self.msgServerToClientMap.update([(serverType, clientType)])
        return

    def createServer(self):
        self.serverHandle = match4server.createServer()
        self.messageAwaitTimer.start(100)
        return

    def destroyServer(self):
        self.messageAwaitTimer.stop()
        match4server.destroyServer(self.serverHandle)
        self.serverHandle = 0
        return

    def receive(self, msg):
        self.sendToServer(msg)
        return

    @pyqtSlot(Message)
    def sendToServer(self, msg):
        match4server.sendMessage(self.serverHandle,
                                 self.msgClientToServerMap[msg.msgType],
                                 msg.data[0], msg.data[1], msg.data[2])
        return

    def __checkMessages(self):
        response = match4server.checkResponse(self.serverHandle)
        if response is None:
            return
        self.__receiveFromServer(response)
        return

    def __receiveFromServer(self, serverMsg):
        clientMsg = Messages.createMessage(
            self.msgServerToClientMap[serverMsg[0]], [serverMsg[1], serverMsg[2], serverMsg[3]])
        self.messageReceived.emit(clientMsg)
        return
