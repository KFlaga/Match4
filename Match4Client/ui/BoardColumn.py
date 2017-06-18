from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_BoardColumn(object):
    def setupUi(self, column_button, column_number):
        column_button.setObjectName('column_{0}'.format(column_number))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(column_button.sizePolicy().hasHeightForWidth())
        column_button.setSizePolicy(sizePolicy)
        column_button.setFocusPolicy(QtCore.Qt.NoFocus)
        column_button.setStyleSheet("background-image: url(:/fields/empty_column.png);")
        column_button.setText("")
        column_button.setFlat(False)

        self.field_column = QtWidgets.QFrame(column_button)
        self.field_column.setGeometry(QtCore.QRect(0, 0, 64, 302))
        self.field_column.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.field_column.setFrameShadow(QtWidgets.QFrame.Raised)
        self.field_column.setObjectName("field_column_{0}".format(column_number))

        self.verticalLayout = QtWidgets.QVBoxLayout(self.field_column)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setSpacing(0)
        self.verticalLayout.setObjectName("field_column_layout{0}".format(column_number))

        QtCore.QMetaObject.connectSlotsByName(column_button)
