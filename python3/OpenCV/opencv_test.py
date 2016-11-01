# -*- coding: utf-8 -*-

import cv2

img = cv2.imread('lena.jpg')
cv2.imshow('img', img)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)    # grayscale conversion
cv2.imshow("gray", gray)
cv2.waitKey(0)
