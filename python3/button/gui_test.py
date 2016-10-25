#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
from PyQt5.QtWidgets import  *


def main():
    app = QApplication(sys.argv)
    w = QWidget()
    w.resize(250, 150)
    w.setWindowTitle('Window01')
    w.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
