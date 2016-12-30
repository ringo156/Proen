# coding : utf-8

import cv2
import numpy as np
import os
from math import ceil

temp_face_img_path = './result/'

# 顔判定で使うxmlファイルを指定する。
cascade_path =  "/home/ringo/.pyenv/versions/anaconda3-4.2.0/pkgs/opencv3-3.1.0-py35_0/share/OpenCV/haarcascades/haarcascade_frontalface_alt2.xml"

class faceDetect:
    def __init__(self):
        print("init")

    # 画像から顔を切り取り、保存、パスを返す
    def crop_face(self, img_path):
        # ファイル名解析
        base_name = os.path.basename(img_path)
        name, ext = os.path.splitext(base_name)
        if (ext != '.jpg') and (ext != '.jpeg') :
            print('not a jpg image')
            return

        img_src = cv2.imread(img_path, 1)
        # グレースケールに変換
        img_gray = cv2.cvtColor(img_src, cv2.COLOR_BGR2GRAY)
        cascade = cv2.CascadeClassifier(cascade_path)

        org_width = img_src.shape[1]
        org_height = img_src.shape[0]
        i = 0

        for j in range(-10,10):
            # 拡大画像の作成
            big_img = np.zeros((org_height * 2, org_width * 2 ,3), np.uint8)
            big_img[ceil(org_height/2.0):ceil(org_height/2.0*3.0), ceil(org_width/2.0):ceil(org_width/2.0*3.0)] = img_src

            # 画像の中心位置
            center = tuple(np.array([big_img.shape[1] * 0.5, big_img.shape[0] * 0.5]))

            # 画像サイズの取得(横, 縦)
            size = tuple(np.array([big_img.shape[1], big_img.shape[0]]))

            # 回転させたい角度
            angle = 5.0 * float(j)
            # 拡大比率
            scale = 1.0

            # 回転変換行列の算出
            rotation_matrix = cv2.getRotationMatrix2D(center, angle, scale)

            # アフィン変換
            img_rot = cv2.warpAffine(big_img, rotation_matrix, size, flags=cv2.INTER_CUBIC)
            rot_gray = cv2.cvtColor(img_rot, cv2.COLOR_BGR2GRAY)

            #顔判定
            faces = cascade.detectMultiScale(img_rot, scaleFactor=1.2, minNeighbors=2, minSize=(50, 50))
            # 顔があった場合
            if len(faces) > 0:
                for (x,y,w,h) in faces:
                    face = img_rot[y:y+h, x:x+w]
                    if(self.HSV_func(face) == True):
                        file_name =  name + "_face_" + str(i) + ext
                        cv2.imwrite(temp_face_img_path + file_name, face )
                        i += 1
        return

    def HSV_func(self, img):
        kernel = np.ones((5,5), np.uint8)
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
        # print("Moment[px]:", m)
        # print("Percent[%]:", per)
        if((40.0 <= per) and (per <= 80.0)):
            return True
        else:
            return False

    def main():
        classfaicedetect = faceDetect()
        classfaicedetect.crop_face('12.jpg')

if __name__ == '__main__':
    faceDetect.main()
