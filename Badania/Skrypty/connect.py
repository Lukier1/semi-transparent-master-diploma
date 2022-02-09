import os
import cv2
import numpy as np

dirName = 'w10'
dirs = os.listdir(dirName)

img1 = None
for file in dirs:
   if img1 is None: 
       img1 = cv2.imread(dirName + '/' + file)
   else:
       img2 = cv2.imread(dirName + '/' + file)[0:287][0:287]
       img1 = np.concatenate((img1, img2), axis=1)
cv2.imwrite(dirName + '/' + dirName + '_merge.png', img1)