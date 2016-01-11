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
				
				sval = getSonar()
				
				lval = getLsensor()
				rval = getRsensor()

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
					
					#lmotor.run_forever(speed_sp = 100)
					#rmotor.run_forever(speed_sp = 100)
					#if(sval > 40):
					#	lmotor.stop()
					#	rmotor.stop()
					while True:
						if isButtonPressed() == True:
							if isRunning == True:
								return
				
				lerror = lMaxBlack - lval
				rerror = rMaxBlack - rval

				lintegral = 0.5 * lintegral + lerror
				rintegral = 0.5 * rintegral + rerror

				lderivative = lerror - lastErrorL 
				rderivative = rerror - lastErrorR

				lastErrorL = lerror
				lastErrorR = rerror
				#				0.5		2.0
				lcontrol = int((5 * lerror + 0.5 * lintegral + 0.75 * lderivative))
				rcontrol = int((6 * rerror + 0.5 * rintegral + 0.75 * rderivative))
				
				if lval >= lMaxBlack+2  and rval >= rMaxBlack+2  :
				
					if ostatniSkret == "R" :
						lmotor.run_forever(speed_sp = 400)
						rmotor.run_forever(speed_sp = -100)
						#print str(lmotor.position) + " l"
						if manualSteer == False :
							start = lmotor.position 
							
						if abs(lmotor.position - start) > 175 :
							while getLsensor() >= lMaxBlack  and getRsensor() >= rMaxBlack:
								lmotor.run_forever(speed_sp = -400)
								rmotor.run_forever(speed_sp = 200)

					else :
						lmotor.run_forever(speed_sp = -100)
						rmotor.run_forever(speed_sp = 400 )
						#print str(rmotor.position) + " r"
						
						if manualSteer == False :
							start = rmotor.position 
						
						if abs(rmotor.position - start) > 175 :
							while getLsensor() >= lMaxBlack  and getRsensor() >= rMaxBlack:
								lmotor.run_forever(speed_sp = 200)
								rmotor.run_forever(speed_sp = -400)
						
					
					manualSteer = True
					
				else :
					
					if abs(lcontrol-rcontrol) <= 15 :
						lmotor.run_forever(speed_sp = 500 + rcontrol)
						rmotor.run_forever(speed_sp = 500 + lcontrol)
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
						if rcontrol > lcontrol and abs(rcontrol - lcontrol) > 5:
							ostatniSkret = "R"
						else :
							ostatniSkret = "L"
				
				sleep(0.001)
				#i = i + 0.01
				#if i >= 0.2 :
					#print str(lcontrol) + " " + str(rcontrol) + " . " + str(lval) + " " + str(rval)
					#i = 0
# main Loop				
robotFunction()

#unlock motors
lmotor.set(stop_command='coast')
rmotor.set(stop_command='coast')
mmotor.set(stop_command='coast')
