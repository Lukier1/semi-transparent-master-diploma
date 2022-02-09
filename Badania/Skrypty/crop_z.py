import os
import cv2
import numpy as np

img_close = cv2.imread('0.png')
img_far = cv2.imread('1.png')

cv2.imshow("original", img_close)
name = 'w10'

cropped_img_close = img_close[227:514, 565:852]
cropped_img_far = img_far[327:398, 622:693]
cropped_img_far_scaled = cv2.resize(cropped_img_far, [288, 288])

# cv2.imshow("cropped_image", cropped_img_close_scaled)
# cv2.imshow("cropped_image_far", cropped_img_far)
# cv2.waitKey(0)
# cv2.destroyAllWindows()

os.mkdir(name)
cv2.imwrite(name+"/"+name + "_far.png", cropped_img_far_scaled)
cv2.imwrite(name+"/"+name + "_close.png", cropped_img_close)