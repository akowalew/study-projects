#!/usr/bin/python

from time import sleep
import time

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
	
	lintegral = rintegral = lastErrorR = lastErrorL	= 0
	isRunning = False
	isCalibrated = False

	i = 0 
	j = 0
	ostatniSkret = "L"
	
	ltab = [ "", "", ""]
	rtab = [ "", "", ""]
	
	start = 0

	czas = 0 
	odliczanie = False

	while True:
	
		lval = getLsensor()
		rval = getRsensor()
		sval = getSonar()
		
		if isButtonPressed() == True:
			if isRunning == True:
				return
			if isRunning == False:
				isRunning = True
				sleep(0.5)
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
				
				#				1.25
				lcontrol = int((2.4 * lerror + 1 * lintegral + 2.5 * lderivative))
				rcontrol = int((2.4 * rerror + 1 * rintegral + 2.5 * rderivative))
				
				if lval >= lMaxBlack+1  and rval >= rMaxBlack+1 :
					
					if ostatniSkret == "R" :
						lmotor.run_forever(speed_sp = 400)
						rmotor.run_forever(speed_sp = 0)
						
						if manualSteer == False :
							start = lmotor.position 
							
						if abs(lmotor.position - start) > 250 :
							while getLsensor() >= lMaxBlack  and getRsensor() >= rMaxBlack:
								lmotor.run_forever(speed_sp = -400)
								rmotor.run_forever(speed_sp = 200)

					else :
						lmotor.run_forever(speed_sp = 0)
						rmotor.run_forever(speed_sp = 400 )
						#print str(rmotor.position) + " r"
						
						if manualSteer == False :
							start = rmotor.position 
						
						if abs(rmotor.position - start) > 250 :
							while getLsensor() >= lMaxBlack  and getRsensor() >= rMaxBlack:
								lmotor.run_forever(speed_sp = 200)
								rmotor.run_forever(speed_sp = -400)
					manualSteer = True
				else :
					predkosc = 0
					
					# y = (-1) * x + 500
						
					predkosc = 500 + int((-2) * (abs(rcontrol - lcontrol)))
					
					print str(predkosc) + " : " + str(rcontrol) + " , " + str(lcontrol)
					
					lmotor.run_forever(speed_sp = predkosc + (rcontrol-lcontrol)/2)
					rmotor.run_forever(speed_sp = predkosc + (lcontrol-rcontrol)/2)
					
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
				if False :#sonar.value() < 11 :
					if isButtonPressed() == True:
						if isRunning == True:
							return
					
					stopMotors()
					
					obroc_sonar(90)
					sleep(0.1)
					
					rotateRobotSym(90, True, 200)
					sleep(0.1)
					
					wskazanie = sonar.value()
					print "Pierwsze " + str(wskazanie)
					
					lmotor.run_forever(speed_sp = 400)
					rmotor.run_forever(speed_sp = 400)
					while( sonar.value() < 1.3 * wskazanie):
						sleep(0.1)	
					sound.beep()					
						
					sleep(0.3)
					stopMotors() 
					rotateRobotSym(-90, True, 200)
					
					wskazanie = sonar.value()
					print "Drugie " + str(wskazanie)
					lmotor.run_forever(speed_sp = 400)
					rmotor.run_forever(speed_sp = 400)
					
					while( sonar.value() > 0.3 * wskazanie) :
						sleep(0.1)
					sound.beep()
						
					while( sonar.value() <= wskazanie*0.8) :
						sleep(0.1)
					sound.beep()
					
					sleep(0.3)
					
					stopMotors()
					
					rotateRobotSym(-60, True, 200)
					lmotor.run_forever(speed_sp = 400)
					rmotor.run_forever(speed_sp = 400)
					obroc_sonar(-90)
					while getRsensor() >= rMaxBlack or getLsensor() >= lMaxBlack :
						continue
				
				sleep(0.001)
				
# main Loop				
robotFunction()

#unlock motors
obroc_sonar(0)
lmotor.set(stop_command='coast')
rmotor.set(stop_command='coast')
mmotor.set(stop_command='coast')
