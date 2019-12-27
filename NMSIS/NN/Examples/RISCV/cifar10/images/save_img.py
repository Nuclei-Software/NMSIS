import os
import sys
import numpy as np
import cv2

if len(sys.argv) > 1:
    files = os.listdir(sys.argv[1])
else:
    files = os.listdir(".")
im_txt = "image_data.h"
with open(im_txt, "w") as imf:
    imf.write("#ifndef _IMAGE_DATA_H_\r\n#define _IMAGE_DATA_H_\r\n")
    for img in files:
        if img.endswith(".png"):
            im = cv2.imread(img)
            im = im[...,::-1] #BGR to RGB
            im = im.reshape((-1))
            #im_txt = img.replace("png", "h")
            #img_data = "image_data.h"
            im_name = img.rstrip(".png")
            print("Adding image data into header file {} for {}".format(img, im_txt))
            #print("generate image header file {} for {}".format(im_txt, img))
            #with open(im_txt, "w") as imf:
            #    imf.write("#ifndef _IMAGE_DATA_H_\r\n#define _IMAGE_DATA_H_")
            #    imf.write("#define IMG_DATA_%s {"%(im_name))
            #    for i in range(im.size):
            #        imf.write(str(im[i]) + " ")
            #        if i % 10 == 0 and i > 0:
            #            imf.write("\\\r\n")
            #    imf.write("}")
            imf.write("//Image data for {}\r\n".format(img))
            imf.write("#define IMG_DATA_%s {"%(im_name))
            for i in range(im.size):
                imf.write(str(im[i]) + ", ")
                if i % 10 == 0 and i > 0:
                    imf.write("\\\r\n")
            imf.write("}\r\n\r\n")
    imf.write("#endif")


