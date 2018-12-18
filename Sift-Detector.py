import cv2
import numpy as np
import time
import serial

MIN_MATCH_COUNT=20
k=.6
detector=cv2.xfeatures2d.SIFT_create()
h=0
w=0
#ser=serial.Serial('/dev/ttyACM0',baudrate=9600)
FLANN_INDEX_KDITREE=0
flannParam=dict(algorithm=FLANN_INDEX_KDITREE,tree=5)
flann=cv2.FlannBasedMatcher(flannParam,{})

trainImg=cv2.imread("logo.jpeg",0)
trainKP,trainDesc=detector.detectAndCompute(trainImg,None)

cam=cv2.VideoCapture(0)

print(cam.get(cv2.CAP_PROP_FPS))
while True:
    ret, QueryImgBGR=cam.read()
    QueryImg=cv2.cvtColor(QueryImgBGR,cv2.COLOR_BGR2GRAY)
    queryKP,queryDesc=detector.detectAndCompute(QueryImg,None)
    matches=flann.knnMatch(queryDesc,trainDesc,k=2)
    goodMatch=[]
    for m,n in matches:
        if(m.distance<k*n.distance):
            goodMatch.append(m)
    if(len(goodMatch)>MIN_MATCH_COUNT):
        tp=[]
        qp=[]
        for m in goodMatch:
            tp.append(trainKP[m.trainIdx].pt)
            qp.append(queryKP[m.queryIdx].pt)
        tp,qp=np.float32((tp,qp))
        H,status=cv2.findHomography(tp,qp,cv2.RANSAC,3.0)
        h,w=trainImg.shape
        trainBorder=np.float32([[[0,0],[0,h-1],[w-1,h-1],[w-1,0]]])
        queryBorder=cv2.perspectiveTransform(trainBorder,H)
        cv2.polylines(QueryImgBGR,[np.int32(queryBorder)],True,(0,255,0),5)
	#ser.write(str(queryBorder[0][0][0]))
	print "position in x plane-"+str( queryBorder[0][0][0])
    else:
        print "Not found"
    cv2.imshow('result',QueryImgBGR)
    if cv2.waitKey(10)==ord('q'):
        break
cam.release()
cv2.destroyAllWindows()
