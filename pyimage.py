#!/bin/python3
import sys
from PIL import Image
from matplotlib import pyplot
import numpy as np
import cv2

def printCarr(data):
    print('{{',end='')
    dat = 0
    b = 0
    i = 0
    for x in range(0,data.shape[0]):
        for y in range(0,int(data.shape[1])):
            c = ','
            if(data[x][y] > 128):
                dat = dat + 128
            if(x*y == ((data.shape[0]-1)*((data.shape[1])-1))):
                c = ''
            if(i == 7):
                if(b == 15 and c != ''):
                    b = 0
                    c = '},{'
                else:
                    b = b +1
                print(dat,end=c)
                dat = 0
                i = 0
            else:
                i = i + 1
            dat = dat >> 1
    print("}}",end='')

cap = cv2.VideoCapture(sys.argv[1])
count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
#count = 800
skip = count-100
print("const char map["+str(int(skip/4))+"][64][16] = {",end='')
while True:
    ret, frame = cap.read()
    if (not ret) or (not count):
        break
    count -= 1
    if(count%4):
        continue
    if(count > skip):
        continue
    image = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
    image = image.resize((128, 64))
    data = np.asarray(image.convert('L'))
    printCarr(data)
    if(count):
        print(",",end='')
print("};")