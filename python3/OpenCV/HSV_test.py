# -*- coding :utf-8 -*-

import cv2
import numpy as np

kernel = np.ones((5,5), np.uint8)
img = cv2.imread('06.jpg')
# フレームをHSVに変換
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

# 取得する色の範囲を指定する
lower_yellow = np.array([0, 40, 40])
upper_yellow = np.array([40, 255, 255])

# 指定した色に基づいたマスク画像の生成
img_mask = cv2.inRange(hsv, lower_yellow, upper_yellow)

# フレーム画像とマスク画像の共通の領域を抽出する。
img_color = cv2.bitwise_and(img, img, mask=img_mask)
opening = cv2.morphologyEx(img_mask, cv2.MORPH_OPEN, kernel)
cv2.imshow("mask", img_mask)
cv2.imshow("morpho", opening)
cv2.imshow("SHOW COLOR IMAGE", img_color)

cv2.waitKey(0)
