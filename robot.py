#!/usr/bin/python

from time import sleep
from ev3dev import *

lmotor = large_motor(OUTPUT_D) ; assert lmotor.connected
rmotor = large_motor(OUTPUT_B) ; assert rmotor.connected
mmotor = medium_motor(OUTPUT_C) ; assert mmotor.connected

lmotor.set(speed_regulation_enabled='off', stop_command='brake')
rmotor.set(speed_regulation_enabled='off', stop_command='brake')
mmotor.set(speed_regulation_enabled='off', stop_command='hold')
mmotor.set(duty_cycle_sp=60)

lsensor = light_sensor(); assert lsensor.connected
rsensor = color_sensor(); assert rsensor.connected
sonar = infrared_sensor(); assert sonar.connected
touch = touch_sensor(); assert touch.connected

#btn = Button()

def obroc_sonar(pozycja) :
	mmotor.run_to_abs_pos(position_sp = pozycja)

def czy_sonar_obrocony() :
	return not mmotor.state

def isButtonPressed():
	return touch.value() == 1

def rotateRobotSym(value):
	lmotor.run_to_rel_pos(position_sp = value, duty_cycle_sp = 40)
	rmotor.run_to_rel_pos(position_sp = -value, duty_cycle_sp = 40)


def robotFunction():
	isRunning = False
	isCalibrated = False
	lblack = -100;
	lwhite = 1000;
	rblack = -100;
	rwhite = 1000;
	lerror = 20;
	rerror = 1;
	while True:
		#print str(isButtonPressed())
		if isButtonPressed() == True:
			if isRunning == True:
				return
			if isRunning == False:
				isRunning = True
				sleep(3.0)
		if isRunning == True:
			if isCalibrated == False:
				rotateRobotSym(90)
				while 'running' in lmotor.state:
					ltmp = lsensor.value()
					rtmp = rsensor.value()
					if ltmp>lblack:
						lblack = ltmp
					if ltmp<lwhite:
						lwhite = ltmp
					if rtmp>rblack:
						rblack = rtmp
					if rtmp<rwhite:
						rwhite = rtmp
				print str(lblack) + " "+ str(lwhite) +" " +str(rblack) + " "+ str(rwhite)	
				
				rotateRobotSym(-180)
				while 'running' in lmotor.state:
					ltmp = lsensor.value()
					rtmp = rsensor.value()
					if ltmp>lblack:
						lblack = ltmp
					if ltmp<lwhite:
						lwhite = ltmp
					if rtmp>rblack:
						rblack = rtmp
					if rtmp<rwhite:
						rwhite = rtmp
				
				print str(lblack) + " "+ str(lwhite) +" " +str(rblack) + " "+ str(rwhite)
				
				rotateRobotSym(90)
				isCalibrated = True
			#if isCalibrated == True:
				
							
						
		#mmotor.run_to_abs_pos(position_sp=0)
		#print("motor state" ,  mmotor.state , mmotor.position, "\n")	
		#print("sonar", touch.value(), "\n")
		#if isButtonPressed():
		#	print "Lewy: " + str(lsensor.value()) + " Prawy: " + str(rsensor.value()) + "\n"
		#	sleep(3)
			


robotFunction()
lmotor.set(stop_command='coast')
rmotor.set(stop_command='coast')
mmotor.set(stop_command='coast')
