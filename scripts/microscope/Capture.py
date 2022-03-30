import cv2, time, sys

camport = 0
stabdelay = 1

def capture():
    cam = cv2.VideoCapture(camport)
    cam = cv2.VideoCapture(camport, cv2.CAP_DSHOW)

    cam.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
    cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

    if not cam.isOpened():
        print("Cannot open webcam")
        return False
    
    name =str(int(time.time()))

    if(len(sys.argv)>1):
        name = str(sys.argv[1])

    time.sleep(stabdelay)
    _, img = cam.read()

    img = cv2.flip(img,  0)
    img = cv2.flip(img, 1)
    cv2.imwrite(name +".png", img)

    cv2.line(img,(1210,680),(1132,680),(0,0,255),5)
    cv2.putText(img,"100um", (1112,710), cv2.FONT_HERSHEY_SIMPLEX, 1,(0, 0, 255),3)
    cv2.imwrite(name +"_scale.png", img)

    cam.release()
    cv2.destroyAllWindows()

capture()
