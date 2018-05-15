# -*- coding: utf-8 -*-

import json


profile1 = {"first":"Taro", "last":"Tanaka", "old":22}
profile2 = {"first":"Hanako", "last":"Yamada", "old":23}
profile3 = {"first":"Mahiru", "last":"Tsuyusaki", "old":19}

# val = mydic["apple"]
# print(val)

dic = {}
dic["profile1"] = profile1
dic["profile2"] = profile2

print(dic)

list = []
list=[profile1, profile2, profile3]

print(list)
a = open('sample.json' , 'w')
json.dump(list, a, indent = 4)
