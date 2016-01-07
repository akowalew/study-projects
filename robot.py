#!/usr/bin/python

from time import sleep
from ev3dev import *	
from utilityFile import *
	
def robotFunction() :
	
	lintegral = rintegral = lastErrorR = lastErrorL	= 0
	isRunning = False
	isCalibrated = False

	i = 0 
	j = 0
	ostatniSkret = "L"

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
				manualSteer = False
			else :

				MIN_SPEED = 100 
				MAX_SPEED = 200
				 
				lval = getLsensor()
				rval = getRsensor()

				lerror = lMaxBlack - lval
				rerror = rMaxBlack - rval

				lintegral = 0.5 * lintegral + lerror
				rintegral = 0.5 * rintegral + rerror

				lderivative = lerror - lastErrorL 
				rderivative = rerror - lastErrorR

				lastErrorL = lerror
				lastErrorR = rerror
				#				0.5		2.0
				lcontrol = int((4.0 * lerror + 0.25 * lintegral + 1.5 * lderivative))
				rcontrol = int((5.0 * rerror + 0.25 * rintegral + 1.5 * rderivative))
				
				if lval > lMaxBlack and rval > rMaxBlack :
				
					if ostatniSkret == "R" :
						lmotor.run_forever(speed_sp = 400)
						rmotor.run_forever(speed_sp = -200)
					else :
						lmotor.run_forever(speed_sp = -200)
						rmotor.run_forever(speed_sp = 400 )
					manualSteer = True
					
				else :
					
					if abs(lcontrol-rcontrol) <= 15 :
						lmotor.run_forever(speed_sp = 600 + rcontrol)
						rmotor.run_forever(speed_sp = 600 + lcontrol)
					else :				
						lmotor.run_forever(speed_sp = 300 + rcontrol)
						rmotor.run_forever(speed_sp = 300 + lcontrol)
					
					if manualSteer == True :
						if ostatniSkret == "R" :
							ostatniSkret = "L" 
						else :
							ostatniSkret = "R"
							
						manualSteer = False
					else :
						if rcontrol > lcontrol :
							ostatniSkret = "R"
						else :
							ostatniSkret = "L"
				
				sleep(0.005)
				i = i + 0.01
				if i >= 0.2 :
					print str(lcontrol) + " " + str(rcontrol) + " . " + str(lval) + " " + str(rval)
					i = 0
# main Loop				
robotFunction()

#unlock motors
#lmotor.set(stop_command='coast')
#rmotor.set(stop_command='coast')
mmotor.set(stop_command='coast')
