#!/usr/bin/python

from time import sleep
from ev3dev import *

from threading import Event, Thread
from utilityFile import *

def call_repeatedly(interval, func):
    stopped = Event()
    def loop():
        while not stopped.wait(interval): # the first call is in `interval` secs
            func()
    Thread(target=loop).start()    
    return stopped.set

#stale
READ_SENSOR_TIME = 0.02 ;
PRINT_VALUES_TIME = 0.51 ;


isReaded = False

	
def readSensors() :
	
	lval = getLsensor()
	rval = getRsensor()
	isReaded = True
	
def printValues() :
	print str(lval) + " " + str(rval)
	

def robotFunction() :
	global lval, rval
	lval = rval = 0
	
	lintegral = rintegral = lastErrorR = lastErrorL	= 0
	isRunning = False
	isCalibrated = False
	
	global timerReadSensors, timerPrintValues

	while True:
		if isButtonPressed() == True:
			if isRunning == True:
				return
			if isRunning == False:
				isRunning = True
				sleep(3.0)
		if isRunning == True:
			if isCalibrated == False:
				global lBlack, lWhite, rBlack, rWhite, eps, lMaxBlack, rMaxBlack
	
				parametry = calibrateSensors() 
				lBlack = parametry[0]
				lMaxBlack = parametry[1]
				lWhite = parametry[2]
				rBlack = parametry[3]
				rMaxBlack = parametry[4]
				rWhite = parametry[5]	
				
				
				
				isCalibrated = True
				timerReadSensors = call_repeatedly(READ_SENSOR_TIME, readSensors)
				timerPrintValues = call_repeatedly(PRINT_VALUES_TIME, printValues) 
			else :

				MIN_SPEED = 100 
				MAX_SPEED = 200
				 
				print str(lintegral) + " " + str(rintegral) + " " +str(lastErrorR) + " " + str(lastErrorL)

				lerror = lMaxBlack - lval
				rerror = rMaxBlack - rval

				lintegral = lintegral + lerror
				rintegral = rintegral + rerror

				lderivative = lerror - lastErrorL 
				rderivative = rerror - lastErrorR

				lastErrorL = lerror
				lastErrorR = rerror

				lcontrol = int(3.0 * lerror + 0.5 * lintegral + 2.0 * lderivative)
				rcontrol = int(3.0 * rerror + 0.5 * rintegral + 2.0 * rderivative)
				print str(lcontrol)
				#lcontrol = int((lval - lBlack) / (lWhite - lBlack))
				#rcontrol = int((rval - rBlack) / (rWhite - rBlack))

				#lspeed = MIN_SPEED + int(lcontrol * (MAX_SPEED - MIN_SPEED))
				#rspeed = MIN_SPEED + int(rcontrol * (MAX_SPEED - MIN_SPEED))

				lmotor.run_forever(speed_sp = 50 + lcontrol)
				rmotor.run_forever(speed_sp = 50 + rcontrol)
				
			
# main Loop				
robotFunction()

#kill threads
timerReadSensors()
timerPrintValues()

#unlock motors
lmotor.set(stop_command='coast')
rmotor.set(stop_command='coast')
mmotor.set(stop_command='coast')
