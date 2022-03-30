import cv2

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

cv2.namedWindow('Input', cv2.WINDOW_NORMAL)
cv2.setWindowProperty('Input', cv2.WND_PROP_FULLSCREEN,cv2.WINDOW_FULLSCREEN)
#cv2.resizeWindow('Input',1080,1920)

# Check if the webcam is opened correctly
if not cap.isOpened():
    raise IOError("Cannot open webcam")

while True:
    ret, frame = cap.read()
    frame = cv2.flip(frame,  0)
    frame = cv2.flip(frame, 1)
    #cv2.line(frame, (1255,680), (1132, 680), (0,0,255), 5)
    #cv2.putText(frame,"100um", (1147,710), cv2.FONT_HERSHEY_SIMPLEX, 1,(0, 0, 255),3) 
    cv2.imshow('Input',frame)

    c = cv2.waitKey(1)
    if c == 27:
        break

cap.release()
cv2.destroyAllWindows()
