from time import sleep
from ev3dev import *

#Inicjalizacja silników
lmotor = large_motor(OUTPUT_D) ; assert lmotor.connected
rmotor = large_motor(OUTPUT_B) ; assert rmotor.connected
mmotor = medium_motor(OUTPUT_C) ; assert mmotor.connected
lmotor.speed_regulation_enabled = 'on'
rmotor.speed_regulation_enabled = 'on'
mmotor.set(speed_regulation_enabled='on', stop_command='hold')
mmotor.set(speed_sp=200)

#Inicjalizacja sensorow
lsensor = light_sensor(); assert lsensor.connected
rsensor = color_sensor(); assert rsensor.connected
sonar = infrared_sensor(); assert sonar.connected
touch = touch_sensor(); assert touch.connected

#Funkcja obracająca sonar o ustalony kąt
def obroc_sonar(pozycja) :
	mmotor.run_to_rel_pos(position_sp = pozycja)

#Funkcja sprawdzająca czy wciśnięty został przycisk
def isButtonPressed():
	return touch.value() == 1

#Obraca robota symetrycznie - koła obracają się o ustalony kąt w przeciwnych kierunkach
#haveToWait ustala czy funkcja ma poczekać aż koła się obrócą do końca
def rotateRobotSym(value, haveToWait, itsSpeed):
	lmotor.run_to_rel_pos(position_sp = value, speed_sp = itsSpeed)
	rmotor.run_to_rel_pos(position_sp = -value, speed_sp = itsSpeed)
	
	if not haveToWait :
		return
		
	while 'running' in lmotor.state or 'running' in rmotor.state :
		sleep(0.01)

#Funkcja zwraca obecną wartość odczytaną z prawego sensora
def getRsensor() :
	return rsensor.value()

#Funkcja zwraca obecną wartość odczytaną  z lewego sensora
def getLsensor() :
	return int(lsensor.float_value())

#Funkcja zwraca obecną wartość odczytaną z sonaru
def getSonar():
	return sonar.value()


#Funkcja zatrzymuje oba silniki stopniowo zmniejszając ich prędkość,
#dzięki temu hamują łagodniej
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
			
		
#Funkcja do odczytania wartości czarnej i białej dla obu czujników - 
#obraca robota symetrycznie w jedną i drugą stronę, zapisuje największą
#i najmniejszą odczytaną wartość - odpowiednio biały i czarny,
#zwraca listę tych wartości
def calibrateSensors() :
	
	#Wartości dla czujnika lewego
	lBlack = 1000
	lWhite = -100

	#Wartości dla czujnika prawego
	rBlack = 1000
	rWhite = -100

	#Określa w jakiej części czujniki mają się znajdować na linii
	eps = 0.75
	
	#Obraca robota i zapisuje nawiększą oraz najmniejszą wartość
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
	rotateRobotSym(90, True, 150)
	
	#Ustalamy wartości dla jakich czujniki znajdują się w zadanej pozycji
	lMaxBlack = lBlack + eps * (lWhite - lBlack)
	rMaxBlack = rBlack + eps * (rWhite - rBlack) 

	return [lMaxBlack, rMaxBlack]
