from time import sleep
from ev3dev import *

#inicjacja motorow
lmotor = large_motor(OUTPUT_D) ; assert lmotor.connected
rmotor = large_motor(OUTPUT_B) ; assert rmotor.connected
mmotor = medium_motor(OUTPUT_C) ; assert mmotor.connected
lmotor.speed_regulation_enabled = 'on'
rmotor.speed_regulation_enabled = 'on'

#lmotor.set(speed_regulation_enabled='off', stop_command='coast')
#rmotor.set(speed_regulation_enabled='off', stop_command='coast')
mmotor.set(speed_regulation_enabled='off', stop_command='hold')
mmotor.set(duty_cycle_sp=60)

#inicjacja sensorow
lsensor = light_sensor(); assert lsensor.connected
rsensor = color_sensor(); assert rsensor.connected
sonar = infrared_sensor(); assert sonar.connected
touch = touch_sensor(); assert touch.connected

def obroc_sonar(pozycja) :
	mmotor.run_to_abs_pos(position_sp = pozycja)

def czy_sonar_obrocony() :
	return not mmotor.state

def isButtonPressed():
	return touch.value() == 1

def rotateRobotSym(value):
	lmotor.run_to_rel_pos(position_sp = value, speed_sp = 100)
	rmotor.run_to_rel_pos(position_sp = -value, speed_sp = 100)
		
def getRsensor() :
	return rsensor.value()
	
def getLsensor() :
	return int(lsensor.float_value())

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
	lMaxBlack = 0
	rMaxBlack = 0
	
	rotateRobotSym(90)
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
	
	rotateRobotSym(-180)
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
	
	rotateRobotSym(90)
	sleep(1.0)

	lista = [lBlack, lMaxBlack, lWhite, rBlack, rMaxBlack, rWhite]
	sound.speak("FUCK YOU! FUCK YOU!", True)
	return lista
