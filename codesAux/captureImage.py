import cv2
import numpy as np
import time
import sys
from PyQt4.QtGui import QPixmap, QApplication

def getImage():
    img=QPixmap.grabWindow(QApplication.desktop().winId()).toImage()
    width=img.width()
    height=img.height()
    i=0

    ret = np.zeros((height,width,3), np.uint8)
    ind=0
   

    return ret
    
timeStep=1.0/30.0
app = QApplication(sys.argv)
while(True):
    cv2.imshow('frame',getImage())
    time.sleep(timeStep)
