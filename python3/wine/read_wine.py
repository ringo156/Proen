# -*- coding: UTF-8 -*-

for line in open('wine.data', 'r'):
#    itemList = line[:-1].split(',')
#    print (itemList)
    itemList = line.split(',')
    numbers = []
    for item in itemList:
        numbers.append( float(item) )
    print (numbers)
print (numbers[1])
