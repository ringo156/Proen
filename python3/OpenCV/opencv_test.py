# -*- coding: utf-8 -*-

import cv2

img = cv2.imread('lena.jpg')
cv2.imshow('img', img)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)    # grayscale conversion
cv2.imshow("gray", gray)

cascade_path='C:/Users/ringo/Anaconda3/Library/etc/haarcascades/harcascade_frontalface_alt.xml'
faceCascade = cv2.CascadeClassifier(cascade_path)
test = faceCascade.load("haarcascade_frontalface_alt.xml")
print(test)

cv2.waitKey(0)
