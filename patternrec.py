
import cv2
import numpy as np
import time
import serial

ser=serial.Serial('/dev/ttyACM0',baudrate=9600)
minRadius=50
maxRadius=70
threshold = 0.3
logo=cv2.imread('hey.png',cv2.IMREAD_GRAYSCALE)

def pattern_rec(img):
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    ret,thresh = cv2.threshold(gray,100,255,0)
    
    im2,contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    cv2.drawContours(img,contours,-1,(0,255,0),3)
    return img

def logo_rec(src):
    gray_img = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)
    img = cv2.medianBlur(gray_img, 5)
    cimg = src.copy() # numpy function
    mx=0
    retX=-1
    retY=-1
    circles = cv2.HoughCircles(img, cv2.HOUGH_GRADIENT, 1, 10, np.array([]), 100, 30, minRadius, maxRadius)
    if circles is not None: # Check if circles have been found and only then iterate over these and add them to the image
        a, b, c = circles.shape
        mx=0
        for center_x,center_y,radius in circles[0]:
            gray_crop=gray_img[int(center_y-radius):int(center_y+radius),int(center_x-radius):int(center_x+radius)]
            #thres_crop = th = cv2.adaptiveThreshold(gray_crop, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 115, 1)
            cv2.circle(cimg, (center_x, center_y), radius, (0, 0, 255), 3, cv2.LINE_AA)
            try:
                r=gray_crop.shape[1]
                c=gray_crop.shape[0]
                res_logo = cv2.resize(logo,(r,c))
                #print(gray_crop.shape,res_logo.shape)
                
                res = cv2.matchTemplate(gray_crop,res_logo,cv2.TM_CCOEFF_NORMED)
		#print res
                if max(res)>mx:
                    mx=max(res)
                    retX=center_x
		    retY=center_y
                    
                    cv2.circle(cimg, (center_x, center_y), radius, (0, 0, 255), 3, cv2.LINE_AA)
            except:
                pass
            

    return cimg,mx,retX,retY
v=cv2.VideoCapture(1)
def Main():
    global img
    print("cam init")
    print("cam start")
    flag=False
    number=0
    for i in range(200):
        ret,frame=v.read()
        #img=pattern_rec(frame)
        #print(ret)
        img,val,x,y=logo_rec(frame)
        cv2.imshow('window',img)
        if val>threshold:
            number+=1
        if (cv2.waitKey(10)==ord('q')):
            break
        if number>2:
            flag=True
            break
    
    
    return flag,x,y
if __name__=='__main__':
    
    while(1):
    	flag,x,y=Main()
    	if flag:
        	global img
        	print('logo detected at %d'%(x))
		ser.write(str(x))
		time.sleep(1)
        	#cv2.imshow('window',img)
    	else:
        	print('logo not found')

    v.release()
    cv2.waitKey(1000)
    cv2.destroyAllWindows()
