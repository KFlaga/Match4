# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'GameSettingsDialog.ui'
#
# Created by: PyQt5 UI code generator 5.8.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dialog(object):
	def setupUi(self, Dialog, addDiff_1, addDiff_2):
		Dialog.setObjectName("Dialog")
		Dialog.setWindowModality(QtCore.Qt.ApplicationModal)
		Dialog.resize(203, 185)
		sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
		sizePolicy.setHorizontalStretch(0)
		sizePolicy.setVerticalStretch(0)
		sizePolicy.setHeightForWidth(Dialog.sizePolicy().hasHeightForWidth())
		Dialog.setSizePolicy(sizePolicy)
		Dialog.setModal(True)
		self.gridLayout = QtWidgets.QGridLayout(Dialog)
		self.gridLayout.setObjectName("gridLayout")
		spacerItem = QtWidgets.QSpacerItem(20, 20, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Minimum)
		self.gridLayout.addItem(spacerItem, 3, 0, 1, 1)
		spacerItem1 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Minimum)
		self.gridLayout.addItem(spacerItem1, 1, 2, 1, 1)
		spacerItem2 = QtWidgets.QSpacerItem(20, 20, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Minimum)
		self.gridLayout.addItem(spacerItem2, 0, 0, 1, 1)
			
		if (addDiff_1 is True):
			self.frame_1 = QtWidgets.QFrame(Dialog)
			self.frame_1.setFrameShape(QtWidgets.QFrame.StyledPanel)
			self.frame_1.setFrameShadow(QtWidgets.QFrame.Raised)
			self.frame_1.setObjectName("frame_1")
			self.horizontalLayout = QtWidgets.QHBoxLayout(self.frame_1)
			self.horizontalLayout.setObjectName("horizontalLayout")
			self.difficultyLabel_1 = QtWidgets.QLabel(self.frame_1)
			self.difficultyLabel_1.setLayoutDirection(QtCore.Qt.LeftToRight)
			self.difficultyLabel_1.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
			self.difficultyLabel_1.setObjectName("difficultyLabel_1")
			self.horizontalLayout.addWidget(self.difficultyLabel_1)
			self.difficultySettingBox_1 = QtWidgets.QSpinBox(self.frame_1)
			self.difficultySettingBox_1.setObjectName("difficultySettingBox_1")
			self.horizontalLayout.addWidget(self.difficultySettingBox_1)
			self.gridLayout.addWidget(self.frame_1, 1, 0, 1, 2)
			self.difficultySettingBox_1.valueChanged['int'].connect(Dialog.difficulty_cpu_1_changed)
			self.difficultyLabel_1.setBuddy(self.difficultySettingBox_1)
			
		if addDiff_2 is True:
			self.frame_2 = QtWidgets.QFrame(Dialog)
			self.frame_2.setFrameShape(QtWidgets.QFrame.StyledPanel)
			self.frame_2.setFrameShadow(QtWidgets.QFrame.Raised)
			self.frame_2.setObjectName("frame_2")
			self.horizontalLayout_2 = QtWidgets.QHBoxLayout(self.frame_2)
			self.horizontalLayout_2.setObjectName("horizontalLayout_2")
			self.difficultyLabel_2 = QtWidgets.QLabel(self.frame_2)
			self.difficultyLabel_2.setLayoutDirection(QtCore.Qt.LeftToRight)
			self.difficultyLabel_2.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
			self.difficultyLabel_2.setObjectName("difficultyLabel_2")
			self.horizontalLayout_2.addWidget(self.difficultyLabel_2)
			self.difficultySettingBox_2 = QtWidgets.QSpinBox(self.frame_2)
			self.difficultySettingBox_2.setObjectName("difficultySettingBox_2")
			self.horizontalLayout_2.addWidget(self.difficultySettingBox_2)
			self.gridLayout.addWidget(self.frame_2, 2, 0, 1, 2)
			self.difficultySettingBox_2.valueChanged['int'].connect(Dialog.difficulty_cpu_2_changed)
			self.difficultyLabel_2.setBuddy(self.difficultySettingBox_2)
			
		spacerItem3 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
		self.gridLayout.addItem(spacerItem3, 2, 2, 1, 1)
		self.buttonBox = QtWidgets.QDialogButtonBox(Dialog)
		self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
		self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.Ok)
		self.buttonBox.setObjectName("buttonBox")
		self.gridLayout.addWidget(self.buttonBox, 4, 0, 1, 3)

		self.retranslateUi(Dialog)
		self.buttonBox.accepted.connect(Dialog.accept)
		self.buttonBox.rejected.connect(Dialog.reject)
		QtCore.QMetaObject.connectSlotsByName(Dialog)

	def retranslateUi(self, Dialog):
		_translate = QtCore.QCoreApplication.translate
		Dialog.setWindowTitle(_translate("Dialog", "Dialog"))
		if hasattr(self, 'difficultyLabel_2') is True:
			self.difficultyLabel_2.setText(_translate("Dialog", "Cpu_2 Difficulty"))
		if hasattr(self, 'difficultyLabel_1') is True:
			self.difficultyLabel_1.setText(_translate("Dialog", "Cpu_1 Difficulty"))

