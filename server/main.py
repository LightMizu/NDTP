from persondetection import DetectorAPI
import cv2
import urllib.request
import numpy as np

detect_api = DetectorAPI()

vibro1 = 0
vibro2 = 0
vibro3 = 0
cap = cv2.VideoCapture(0)

while True:
    img_resp=urllib.request.urlopen(f"http://172.20.10.14?vibro={vibro1}{vibro2}{vibro3}")
    vibro1 = 0
    vibro2 = 0
    vibro3 = 0
    imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
    im = cv2.imdecode(imgnp,-1)
    image = cv2.imdecode(imgnp,-1)
    boxes, scores, classes, num = detect_api.processFrame(image=image)
    for i in range(len(boxes)):
        if classes[i] == 1 and scores[i] > 0.8:
            box = boxes[i]
            cv2.rectangle(image, (box[1], box[0]), (box[3], box[2]), (255, 0, 0), 2)
            ran = box[1]/800+(box[3]/800-box[1]/800)
            if ran > 0.80:
                vibro1 = 1
            elif ran < 0.40:
                vibro3 = 1
            else:
                vibro1 = 1
                vibro3 = 1
    print(vibro1, vibro2, vibro3)
    cv2.imshow("Capture",image)
    key = cv2.waitKey(1)
    if key & 0xFF == ord('q'):
        break