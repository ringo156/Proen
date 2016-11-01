# coding: utf-8

import cv2
import numpy as np
import os

# cascade_path = "haarcascades/haarcascade_frontalface_alt.xml"
# faceCascade = cv2.CascadeClassifier('haarcascade_frontalface_alt.xml')

# cascade_path = os.path.dirname((os.path.abspath(__file__)) +
#                               "/haarcascades/haarcascade_frontalface_alt.xml")
cascade_path='C:/Users/ringo/Anaconda3/Library/etc/haarcascades/harcascade_frontalface_alt.xml'

faceCascade = cv2.CascadeClassifier(cascade_path)

img = cv2.imread('lena.jpg')

# cv2.imshow('img', img)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)    # grayscale conversion
# face = faceCascade.detectMultiScale(gray, 1.3, 5)
face = faceCascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=1,
                                    minSize=(1, 1))

if len(face) > 0:
    for rect in face:
        cv2.rectangle(img, tuple(rect[0:2]), tuple(rect[0:2]+rect[2:4]), (0,
                      0, 255), thickness=2)
else:
    print ('no face')

cv2.imshow("detect", img)
cv2.imshow("gray", gray)
cv2.waitKey(0)
