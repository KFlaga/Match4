# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'MenuFrame.ui'
#
# Created by: PyQt5 UI code generator 5.8.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_mainFrame(object):
    def setupUi(self, mainFrame):
        mainFrame.setObjectName("mainFrame")
        mainFrame.resize(348, 365)
        self.gridLayout = QtWidgets.QGridLayout(mainFrame)
        self.gridLayout.setObjectName("gridLayout")
        spacerItem = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem, 1, 0, 1, 1)
        spacerItem1 = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.gridLayout.addItem(spacerItem1, 0, 1, 1, 1)
        spacerItem2 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem2, 1, 2, 1, 1)
        self.menuFrame = QtWidgets.QFrame(mainFrame)
        self.menuFrame.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.menuFrame.setFrameShadow(QtWidgets.QFrame.Raised)
        self.menuFrame.setObjectName("menuFrame")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.menuFrame)
        self.verticalLayout.setObjectName("verticalLayout")
        self.humanVsHumanButton = QtWidgets.QPushButton(self.menuFrame)
        self.humanVsHumanButton.setObjectName("humanVsHumanButton")
        self.verticalLayout.addWidget(self.humanVsHumanButton)
        self.humanVsCpuButton = QtWidgets.QPushButton(self.menuFrame)
        self.humanVsCpuButton.setObjectName("humanVsCpuButton")
        self.verticalLayout.addWidget(self.humanVsCpuButton)
        self.cpuVsCpuButton = QtWidgets.QPushButton(self.menuFrame)
        self.cpuVsCpuButton.setObjectName("cpuVsCpuButton")
        self.verticalLayout.addWidget(self.cpuVsCpuButton)
        self.exitButton = QtWidgets.QPushButton(self.menuFrame)
        self.exitButton.setObjectName("exitButton")
        self.verticalLayout.addWidget(self.exitButton)
        self.gridLayout.addWidget(self.menuFrame, 1, 1, 1, 1)
        spacerItem3 = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.gridLayout.addItem(spacerItem3, 2, 1, 1, 1)

        self.retranslateUi(mainFrame)
        self.humanVsHumanButton.clicked.connect(mainFrame.humanVsHumanClicked)
        self.humanVsCpuButton.clicked.connect(mainFrame.humanVsCpuClicked)
        self.cpuVsCpuButton.clicked.connect(mainFrame.cpuVsCpuClicked)
        self.exitButton.clicked.connect(mainFrame.endClicked)
        QtCore.QMetaObject.connectSlotsByName(mainFrame)

    def retranslateUi(self, mainFrame):
        _translate = QtCore.QCoreApplication.translate
        mainFrame.setWindowTitle(_translate("mainFrame", "Frame"))
        self.humanVsHumanButton.setText(_translate("mainFrame", "Human vs Human"))
        self.humanVsCpuButton.setText(_translate("mainFrame", "Human vs Cpu"))
        self.cpuVsCpuButton.setText(_translate("mainFrame", "Cpu vs Cpu"))
        self.exitButton.setText(_translate("mainFrame", "Exit"))

