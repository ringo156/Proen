# -*- coding :utf-8 -*-

import cv2
import numpy as np

kernel = np.ones((5,5), np.uint8)
img = cv2.imread('./test/10_face_36.jpg')
# フレームをHSVに変換
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

# 取得する色の範囲を指定する
lower_yellow = np.array([0, 30, 80])
upper_yellow = np.array([15, 200, 255])

# 指定した色に基づいたマスク画像の生成
img_mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
# フレーム画像とマスク画像の共通の領域を抽出する。
img_color = cv2.bitwise_and(img, img, mask=img_mask)
opening = cv2.morphologyEx(img_mask, cv2.MORPH_OPEN, kernel)

m = cv2.countNonZero(opening)
h, w = opening.shape
per = round(100 * float(m) / (w * h), 1)
print("Moment[px]:", m)
print("Percent[%]:", per)

cv2.imshow("mask", img_mask)
cv2.imshow("morpho", opening)
cv2.imshow("SHOW COLOR IMAGE", img_color)

cv2.waitKey(0)
