from matplotlib import image
import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt
import os
import shutil

alphas = ["0.0"]
for x in range(1,21,1):
    alphas.append(str(x/20.0))

def getImagePath(renderType, alpha, num):
    return "input/" + str(num) + "/" + alpha + "/" + renderType + "_" +  str(num) + "_inord" + "_screen.png"



ref = "Abuffer"
#compareImages = [ "w7", "w9", "w10"]
compareImages = [ "stochastic", "NBuffer", "Phenomonological"]
#compareImages = [ "Phenomonological"]

name = "wynikiOdwrotnaKolejnosc"

os.makedirs(name + "/diff", exist_ok=False)
f = open(name + "/diff/res.csv", "w")
f_short = open(name + "/diff/res_short.csv", "w")

for alpha in alphas:
    for num in range(0, 3):
        refImg = cv.imread(getImagePath(ref, alpha, num))
        print("Calc for images number: " + str(num))
        f.write("Num," + str(num) + "\nAlpha," + alpha + "\n")
        for cmpImgType in compareImages:
            print("Calc for image: " + getImagePath(cmpImgType, alpha, num))
            cmpImg = cv.imread(getImagePath(cmpImgType, alpha, num))
            res = cv.absdiff(cmpImg, refImg)
            
            avgDiff = cv.mean(res) 
            f.write( cmpImgType+ "," + "\"" + str(avgDiff) + "\"," + str(np.linalg.norm(avgDiff, ord=1)) + "\n")
            f_short.write( cmpImgType + "," + str(num) + "," +  alpha + "," + str(np.linalg.norm(avgDiff, ord=1)) + "\n")
            invRes = cv.bitwise_not(res)
            # plt.imshow(invRes)
            # plt.show()
        
        
            # os.makedirs(name + "/diff/" + str(num), exist_ok=True)
            # plt.imsave(name + "/diff/" + str(num) + "/" + cmpImgType + ".png", invRes)
f.close()
f_short.close()
# shutil.copytree("D:/Projects/C++/Master3D/Master3D/testresult", name + "/input")

