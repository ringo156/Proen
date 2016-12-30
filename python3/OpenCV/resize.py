# -*- coding: utf-8 -*-
import cv2
import os
import re

#指定する画像フォルダ
files = os.listdir('/home/ringo/Prog/StarField/python3/OpenCV/test/')
for file in files:
    jpg = re.compile("jpg")
    print(file)
    if jpg.search(file):
        img = cv2.imread(file)
        cv2.imshow("img", img)
        resized = cv2.resize(img, (32,32))
        cv2.imwrite(file,resized)
