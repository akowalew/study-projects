#!/usr/bin/python

from time import sleep
from ev3dev import *	
from utilityFile import *
import timeit	

def getRstate(rval, rMaxBlack) :
	if rval > rMaxBlack :
		return "w" 
	else :
		return "b"
		
def getLstate(lval, lMaxBlack) :
	if lval > lMaxBlack :
		return "w" 
	else :
		return "b"
		
	
def robotFunction() :
	mmotor.run_to_abs_pos(position_sp = 0)
	
	lintegral = rintegral = lastErrorR = lastErrorL	= 0
	isRunning = False
	isCalibrated = False

	i = 0 
	j = 0
	ostatniSkret = "L"
	
	ltab = [ "", "", ""]
	rtab = [ "", "", ""]
	
	start = 0

	while True:
	
		lval = getLsensor()
		rval = getRsensor()
		sval = getSonar()
		
	
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
				
				lerror = lMaxBlack - lval
				rerror = rMaxBlack - rval

				lintegral = 0.5 * lintegral + lerror
				rintegral = 0.5 * rintegral + rerror

				lderivative = lerror - lastErrorL 
				rderivative = rerror - lastErrorR

				lastErrorL = lerror
				lastErrorR = rerror
				#0.25
				lcontrol = int((6 * lerror + 0.5 * lintegral + 1 * lderivative))
				rcontrol = int((7 * rerror + 0.5 * rintegral + 1 * rderivative))
				
				if lval >= lMaxBlack+1  and rval >= rMaxBlack+1  :
					
					sound.beep()
					
					if ostatniSkret == "R" :
						lmotor.run_forever(speed_sp = 300)
						rmotor.run_forever(speed_sp = -100)
						#print str(lmotor.position) + " l"
						if manualSteer == False :
							start = lmotor.position 
							
						if abs(lmotor.position - start) > 250 :
							while getLsensor() >= lMaxBlack  and getRsensor() >= rMaxBlack:
								lmotor.run_forever(speed_sp = -400)
								rmotor.run_forever(speed_sp = 200)

					else :
						lmotor.run_forever(speed_sp = -50)
						rmotor.run_forever(speed_sp = 300 )
						#print str(rmotor.position) + " r"
						
						if manualSteer == False :
							start = rmotor.position 
						
						if abs(rmotor.position - start) > 250 :
							while getLsensor() >= lMaxBlack  and getRsensor() >= rMaxBlack:
								lmotor.run_forever(speed_sp = 200)
								rmotor.run_forever(speed_sp = -400)
					
					manualSteer = True
					
				else :
					
					if abs(lcontrol-rcontrol) <= 15 :
						lmotor.run_forever(speed_sp = 600 + rcontrol)
						rmotor.run_forever(speed_sp = 600 + lcontrol)
					else :				
						lmotor.run_forever(speed_sp = 500 + rcontrol)
						rmotor.run_forever(speed_sp = 500 + lcontrol)
					
					if manualSteer == True :
						if ostatniSkret == "R" :
							ostatniSkret = "L" 
						else :
							ostatniSkret = "R"
							
						manualSteer = False
					else :
						if rcontrol > lcontrol:
							ostatniSkret = "R"
						else :
							ostatniSkret = "L"
				
								#PRZESZKODA
				if False :
					if isButtonPressed() == True:
						if isRunning == True:
							return
					lmotor.stop()
					rmotor.stop()
					print str(sval)
					mmotor.run_to_abs_pos(position_sp = 90)
					
					rotateRobotSym(90)
					while 'running' in lmotor.state:
						sleep(0.1)
					
					lmotor.run_forever(speed_sp = 100)
					rmotor.run_forever(speed_sp = 100)
					while(sonar.value() < 40):
						sleep(0.1)
						
					lmotor.stop()
					rmotor.stop()
					rotateRobotSym(-90)
					
					while 'running' in lmotor.state:
						sleep(0.1)
						
					while True:
						if isButtonPressed() == True:
							if isRunning == True:
								return
				
				sleep(0.001)
				
# main Loop				
robotFunction()

#unlock motors
lmotor.set(stop_command='coast')
rmotor.set(stop_command='coast')
mmotor.set(stop_command='coast')
