from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_BoardField(object):
    def setupUi(self, field_button, field_number, column_number):
        field_button.setEnabled(True)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.MinimumExpanding, QtWidgets.QSizePolicy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(field_button.sizePolicy().hasHeightForWidth())
        field_button.setSizePolicy(sizePolicy)
        field_button.setMinimumSize(QtCore.QSize(64, 63))
        field_button.setMaximumSize(QtCore.QSize(64, 63))
        field_button.setStyleSheet("background-color: rgba(255, 255, 255, 0);\n"
                                   "background-image: url(:/fields/empty_field_1.png);")
        field_button.setText("")
        field_button.setFlat(False)
        field_button.setObjectName('field_{0}_{1}'.format(column_number, field_number))

        QtCore.QMetaObject.connectSlotsByName(field_button)
