# coding: utf-8

import cv2
import numpy as np
import os

color = (255, 0, 0) #青

eye_path = 'C:/opencv-3.1.0/sources/data/haarcascades/haarcascade_eye.xml'
cascade_path =  'C:/opencv-3.1.0/sources/data/haarcascades/haarcascade_frontalface_alt.xml'
mouth_path = 'C:/opencv-3.1.0/sources/data/haarcascades/haarcascade_mcs_mouth.xml'
faceCascade = cv2.CascadeClassifier(cascade_path)
eyeCascade = cv2.CascadeClassifier(eye_path)
mouthCascade = cv2.CascadeClassifier(mouth_path)
# img = cv2.imread('lena.jpg')
img = cv2.imread('lena.jpg')
cv2.imshow("origin", img)

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)    # grayscale conversion
face = faceCascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=1,
                                    minSize=(1, 1))
mouth = mouthCascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=1,
                                      minSize=(1, 1))
"""
for (x, y, w, h) in face:
    # 検知した顔を矩形で囲む
    cv2.rectangle(img, (x, y), (x+w, y+h), (0, 0, 255), 2)
    # 顔画像（グレースケール）
    roi_gray = gray[y:y+h, x:x+w]
    # 顔ｇ増（カラースケール）
    roi_color = img[y:y+h, x:x+w]
    # 顔の中から目を検知
    eyes = eyeCascade.detectMultiScale(roi_gray)
    for (ex, ey, ew, eh) in eyes:
        # 検知した目を矩形で囲む
        cv2.rectangle(roi_color, (ex, ey), (ex+ew, ey+eh), (255, 0, 0), 2)
"""
if len(mouth) > 0:
    # 検出した顔を囲む矩形の作成
    for rect in mouth:
        cv2.rectangle(img, tuple(rect[0:2]), tuple(rect[0:2]+rect[2:4]), color, thickness=2)
cv2.imshow("detect", img)
cv2.imwrite('result.jpg', img)
cv2.waitKey(0)
