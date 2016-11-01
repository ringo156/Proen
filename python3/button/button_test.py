#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import QMainWindow, QPushButton, QApplication


class Button01(QMainWindow):
    flg = False

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
        if self.flg == True:
            self.statusBar().showMessage(sender.text() + ' off ')
            self.flg = False
        else:
            self.statusBar().showMessage(sender.text() + ' on ')
            self.flg = True

if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = Button01()
    sys.exit(app.exec_())
