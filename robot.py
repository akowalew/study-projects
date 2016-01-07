#!/usr/bin/python

from time import sleep
from ev3dev import *	
	
	
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
			
				parametry = calibrateSensors() 
				lBlack = parametry[0]
				lMaxBlack = parametry[1]
				lWhite = parametry[2]
				rBlack = parametry[3]
				rMaxBlack = parametry[4]
				rWhite = parametry[5]	
				
				isCalibrated = True
			else :

				MIN_SPEED = 100 
				MAX_SPEED = 200
				 
				print str(lintegral) + " " + str(rintegral) + " " +str(lastErrorR) + " " + str(lastErrorL)

				lerror = lMaxBlack - lval
				rerror = rMaxBlack - rval

				lintegral = 0.5 * lintegral + lerror
				rintegral = 0.5 * rintegral + rerror

				lderivative = lerror - lastErrorL 
				rderivative = rerror - lastErrorR

				lastErrorL = lerror
				lastErrorR = rerror

				lcontrol = int(3.0 * lerror + 0.5 * lintegral + 2.0 * lderivative)
				rcontrol = int(3.0 * rerror + 0.5 * rintegral + 2.0 * rderivative)
				print str(lcontrol) + " " + str(rcontrol)
				
				lmotor.run_forever(speed_sp = 500 + lcontrol - rcontrol)
				rmotor.run_forever(speed_sp = 500 - lcontrol + rcontrol)
# main Loop				
robotFunction()

#unlock motors
lmotor.set(stop_command='coast')
rmotor.set(stop_command='coast')
mmotor.set(stop_command='coast')
