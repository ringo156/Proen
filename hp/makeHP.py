# -*- coding: utf-8 -*-

import csv
import json

def getDict(fileName):
    
    f=open(fileName, "r")
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
        # print(line)
    f.close    
    # print(dic)
    return dic

def main():
    f = "testpost.md"
    list = []
    list.append(getDict(f))
    print(list)

    # sorted(dumps.items(), key=lambda x: x[1]["date"],reverse=True)

if __name__=="__main__":
    main()