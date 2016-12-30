#!/usr/bin/env python
# coding: UTF-8
import sys
import os
import re
a = 1
#指定する画像フォルダ
files = os.listdir('/home/ringo/Prog/StarField/python3/DNN/Corect/')
for file in files:
    jpg = re.compile("jpg")
    if jpg.search(file):
        os.rename(file, "image%03d.jpg" %(a))
        a+=1
    else:
        pass
