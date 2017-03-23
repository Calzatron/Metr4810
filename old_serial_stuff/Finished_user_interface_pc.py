#### Import required libraries
## numpy and cv2 for imagery display
## serial and time for serial communication
import numpy as np
import cv2
import serial
import time

#### Function that passes current action
def nothing(x):
    pass

#### Window creator
## Establishes camera feed source
## Creates window with the name frame
## creates a trackbar to keep track of motor control
## power level
cap = cv2.VideoCapture(0)
cv2.namedWindow("frame")
cv2.createTrackbar("power", "frame", 3, 4, nothing)

#### Com port connection establisher
## Informs us when it is trying to connect
## Connects to specified port with a baud rate of 9600
################ Change this based on Device Manager and
################ where the serial transmitter is connected to
## Informs us when it has successfully connected
print("Connecting...")
chip = serial.Serial('COM8', 9600)
print("Connection established successfully")

#### Operational while loop
while(True):
    #### Frame refresher
    ## Reads a frame from the video source
    ## Frames it for required window size
    ## Displays it
    ## Waits for and reads keyboard input k
    ret, frame = cap.read()
    frame = cv2.resize(frame, (720, 540))
    frame = cv2.flip(frame,0)
    frame = cv2.flip(frame,1)
    cv2.imshow('frame',frame)    
    k = cv2.waitKey(2) & 0xFF

    #### Motor Control
    ## w a s d control used
    ## Sends user input through serial for firmware to recieve    
    if k == 119: #w key
        print("Forward")
        chip.write('w')
    elif k == 97: #a key
        print("Left Turn")
        chip.write('a')
    elif k == 100: #d key
        print("Right Turn")
        chip.write('d')
    elif k == 115: #s key
        print("Backward")
        chip.write('s')

    #### Servo Control
    ## q e control
    ## Sends user input through serial for firmware to recieve 
    elif k == 113: #q key
        print("Tighten Claw")
        chip.write('q')
    elif k == 101: #e key
        print("Loosen Claw")
        chip.write('e')

    #### Program Exiter
    ## On pressing 'ESC' key, closes the current session
    elif k == 27: #esc key        
        cv2.destroyAllWindows()
        print("Exiting")
        break

    #### Motor control power level
    ## Numbers 1-5, power control, 1 being minimum power and 5 being max
    ## Sends user input through serial for firmware to recieve
    ## Each number corresponds to a different level of fine motor control
    elif k == 49: #1 key
        cv2.setTrackbarPos("power", "frame", 0)
        chip.write('1')
        print("Power: 1")
    elif k == 50: #2 key
        cv2.setTrackbarPos("power", "frame", 1)
        chip.write('2')
        print("Power: 2")
    elif k == 51: #3 key
        cv2.setTrackbarPos("power", "frame", 2)
        chip.write('3')
        print("Power: 3")
    elif k == 52: #4 key
        cv2.setTrackbarPos("power", "frame", 3)
        chip.write('4')
        print("Power: 4")
    elif k == 53: #5 key
        cv2.setTrackbarPos("power", "frame", 4)
        chip.write('5')
        print("Power: 5")

    #### General stop command
    ## Unless user input has occured, the x character is sent
    ## When x is recieved by firmware: both motors are stopped
    ## Servo holds current angle
    else:
        chip.write('x')
