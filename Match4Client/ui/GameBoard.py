# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'GameBoard.ui'
#
# Created by: PyQt5 UI code generator 5.8.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_GameBoard(object):
    def setupUi(self, board_frame):
        board_frame.setObjectName("board_frame")
        board_frame.resize(320, 304)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(board_frame.sizePolicy().hasHeightForWidth())
        board_frame.setSizePolicy(sizePolicy)
        board_frame.setMinimumSize(QtCore.QSize(320, 304))
        board_frame.setMaximumSize(QtCore.QSize(320, 304))
        board_frame.setFrameShape(QtWidgets.QFrame.StyledPanel)
        board_frame.setFrameShadow(QtWidgets.QFrame.Raised)

        for i in range(5):
            column = self.columns[i]
            column.setGeometry(QtCore.QRect(i * 64, 0, 64, 302))

        QtCore.QMetaObject.connectSlotsByName(board_frame)
