from time import sleep
from ev3dev import *

#inicjacja motorowsds
lmotor = large_motor(OUTPUT_D) ; assert lmotor.connected
rmotor = large_motor(OUTPUT_B) ; assert rmotor.connected
mmotor = medium_motor(OUTPUT_C) ; assert mmotor.connected
lmotor.speed_regulation_enabled = 'on'
rmotor.speed_regulation_enabled = 'on'

#lmotor.set(speed_regulation_enabled='off', stop_command='coast')
#rmotor.set(speed_regulation_enabled='off', stop_command='coast')
mmotor.set(speed_regulation_enabled='on', stop_command='hold')
mmotor.set(speed_sp=200)

#inicjacja sensorow
lsensor = light_sensor(); assert lsensor.connected
rsensor = color_sensor(); assert rsensor.connected
sonar = infrared_sensor(); assert sonar.connected
touch = touch_sensor(); assert touch.connected

def obroc_sonar(pozycja) :
	mmotor.run_to_rel_pos(position_sp = pozycja)

def isButtonPressed():
	return touch.value() == 1

def rotateRobotSym(value, haveToWait, itsSpeed):
	lmotor.run_to_rel_pos(position_sp = value, speed_sp = itsSpeed)
	rmotor.run_to_rel_pos(position_sp = -value, speed_sp = itsSpeed)
	
	if not haveToWait :
		return
		
	while 'running' in lmotor.state or 'running' in rmotor.state :
		sleep(0.01)
		
def getRsensor() :
	return rsensor.value()
	
def getLsensor() :
	return int(lsensor.float_value())

def getSonar():
	return sonar.value()

def stopMotors() :
	while 'running' in lmotor.state or 'running' in rmotor.state :
		if lmotor.speed > 50 :
			lmotor.run_forever(speed_sp = lmotor.speed - 300)
		else :
			lmotor.stop()
			
		if rmotor.speed > 50 :
			rmotor.run_forever(speed_sp = rmotor.speed - 300)
		else :
			rmotor.stop()
			
		

def calibrateSensors() :
	
	# kalbiracja czujnikow
	#czujnik lewy
	lBlack = 1000
	lWhite = -100

	#	czujnik prawy
	rBlack = 1000
	rWhite = -100

	#procentowa dokladnosc
	eps = 0.75


	#graniczne wartosci
	#lMaxBlack = getLsensor()
	rMaxBlack = getRsensor()
	
	rotateRobotSym(90, False, 150)
	while 'running' in lmotor.state:
		ltmp = getLsensor()
		rtmp = getRsensor()
		if ltmp>lWhite:
			lWhite = ltmp
		if ltmp<lBlack:
			lBlack = ltmp
		if rtmp>rWhite:
			rWhite = rtmp
		if rtmp<rBlack:
			rBlack = rtmp
	
	rotateRobotSym(-180, False, 150)
	while 'running' in lmotor.state:
		ltmp = getLsensor()
		rtmp = getRsensor()
		if ltmp>lWhite:
			lWhite = ltmp
		if ltmp<lBlack:
			lBlack = ltmp
		if rtmp>rWhite:
			rWhite = rtmp
		if rtmp<rBlack:
			rBlack = rtmp
	

	
	lMaxBlack = lBlack + eps * (lWhite - lBlack)
	rMaxBlack = rBlack + eps * (rWhite - rBlack) 
	
	print str(lBlack) + " " + str(lWhite) + " " + str(rBlack) + " " + str(rWhite) + " max " + str(lMaxBlack) + " " + str(rMaxBlack)
	
	rotateRobotSym(85, True, 150)
	sleep(0.5)

	lista = [lBlack, lMaxBlack, lWhite, rBlack, rMaxBlack, rWhite]
	return lista
