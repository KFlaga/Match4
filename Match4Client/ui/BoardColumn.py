from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_BoardField(object)
	def setupUi(self, field_button, field_number, column_number)
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


class Ui_BoardColumn(object)
	def setupUi(self, column_button, column_number):
		column_button.setObjectName('column_{0}'.format(column_number))
		column_button.setSizePolicy(sizePolicy)
		column_button.setFocusPolicy(QtCore.Qt.NoFocus)
		column_button.setStyleSheet("background-image: url(:/fields/empty_column.png);\n"
"background-color: rgb(85, 85, 255);")
		column_button.setText("")
		column_button.setFlat(False)
		
		self.field_column = QtWidgets.QFrame(column_button)
		self.field_column.setGeometry(QtCore.QRect(0, 0, 64, 302))
		self.field_column.setcolumn_buttonShape(QtWidgets.QFrame.StyledPanel)
		self.field_column.setcolumn_buttonShadow(QtWidgets.QFrame.Raised)
		self.field_column.setObjectName("field_column_{0}".format(column_number))
		
		self.verticalLayout = QtWidgets.QVBoxLayout(self.field_column)
		self.verticalLayout.setContentsMargins(0, 0, 0, 0)
		self.verticalLayout.setSpacing(0)
		self.verticalLayout.setObjectName("field_column_layout{0}".format(column_number))
		
		fieldsUi = Ui_BoardField()
		self.fields = []
		for i in range(5)
			self.fields.append(QtWidgets.QPushButton(self.field_column))
			fieldsUi.setupUi(self.fields[i], i, column_number)
			self.verticalLayout.addWidget(self.fields[i])
		
		QtCore.QMetaObject.connectSlotsByName(column_button)
