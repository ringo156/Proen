# -*- coding: utf-8 -*-
import numpy
import cv2

img = cv2.imread('lena.jpg')
cv2.imshow('img', img)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)    # grayscale conversion
cv2.imshow("gray", gray)

# cascade_path='C:\Users\ringo\Anaconda3\pkgs\opencv3-3.1.0-py35_0\Library\etc\haarcascades\haarcascade_frontalcatface.xml'
cascade_path =  'C:/opencv-3.1.0/sources/data/haarcascades/haarcascade_frontalface_default.xml'
cascade = cv2.CascadeClassifier(cascade_path)
face_cascade = cascade.detectMultiScale(img, 1.1, 3)
test = cascade.load("haarcascade_frontalface_default.xml")
# ↑なぞにエラー吐く
if len(face_cascade) > 0:
    for rect in face_cascade:
        cv2.rectangle(img, tuple(rect[0:2]), tuple(rect[0:2]+rect[2:4]), (0,
                      0, 255), thickness=2)
else:
    print ('no face')

cv2.imshow("detect", img)

print(test)
print(face_cascade)
cv2.waitKey(0)
