#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import QMainWindow, QPushButton, QApplication


class Button01(QMainWindow):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        btn1 = QPushButton("Button01", self)
        btn1.clicked.connect(self.button01Clicked)

        self.statusBar()

        self.setWindowTitle('kadai5')
        self.show()

    def button01Clicked(self):
        sender = self.sender()
        self.statusBar().showMessage(sender.text() + ' Pushu Button01')
        print("on")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = Button01()
    sys.exit(app.exec_())
