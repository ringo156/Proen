# -*- coding: utf-8 -*-


READ_FILE_LINE = 4

import json
import linecache

# f = open('testpost.md')

# data = f.read()
# f.close()
# print(data)

for num in range(3, 7):
    target_line = linecache.getline('testpost.md', num)
    print(target_line)
    linecache.clearcache()

