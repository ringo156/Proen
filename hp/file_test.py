# -*- coding: utf-8 -*-

import csv
import json

f=open("testpost.md", "r")
# 最初の2行を捨てる
line = f.readline()
line = f.readline()

dic = {}
while line:
    # 改行コードを捨てて読み込み
    line = f.readline().strip("\r\n")
    # ヘッダーが終わったら終了
    if '-->' in line:
        break
    # keyとvalに分けてリスト化
    key, val = line.split(":")
    # jsonにしたいので辞書型に
    dic[key] = val
    print(line)
f.close    
print(dic)

