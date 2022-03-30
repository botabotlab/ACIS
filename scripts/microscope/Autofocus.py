#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json, cv2, time, sys

camport = 0
maxscan = 12

Focus = json.loads('[[],[],0,0]')
Stop = False

cam = cv2.VideoCapture(camport)
cam.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

def sharpness(cam):
    ret_val, img = cam.read(camport)
    sharp = cv2.Laplacian(img, cv2.CV_64F).var()
    return(sharp)

if cam.isOpened() == False:
    print("Error opening video.")
    sys.stdout.flush()
    Stop = True

while Stop == False:

    while len(Focus[0]) < maxscan:
        measure = sharpness(cam)
        Focus[0].append(measure)
        print("d01")
        sys.stdout.flush()
        time.sleep(0.5)
    
    Focus_max = max(range(len(Focus[0])), key=Focus[0].__getitem__)
    Focus[2] = maxscan - Focus_max
    UpCount = Focus[2]

    while UpCount > 0:
        print("u01")
        sys.stdout.flush()
        time.sleep(0.25)
        UpCount -= 1
    
    print("u005")
    sys.stdout.flush()

    while len(Focus[1]) < maxscan:
        measure = sharpness(cam)
        Focus[1].append(measure)
        print("d001")
        sys.stdout.flush()
        time.sleep(0.5)
    
    Focus_max = max(range(len(Focus[1])), key=Focus[1].__getitem__)
    Focus[3] = maxscan - Focus_max
    UpCount = Focus[3] 

    while UpCount > 0:
        print("u001")
        sys.stdout.flush()
        time.sleep(0.25)
        UpCount -= 1

    print("stop")
    print(Focus)
    sys.stdout.flush()
    Stop = True
        

    

    


    
    
