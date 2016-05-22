from time import sleep
import time

from ev3dev import *	
from utilityFile import *
import timeit	

#Główna funkcja sterująca robotem
def robotFunction() :
	
	#Inicjalizacja początkowych zmiennych
	lintegral = rintegral = lastErrorR = lastErrorL	= 0
	isRunning = False 
	isCalibrated = False
	ostatniSkret = "L"
	start = 0
	
	#Główna pętla sterowania robotem
	while True:
		#Pobranie obecnych wartości wszyskich sensorów
		lval = getLsensor()
		rval = getRsensor()
		sval = getSonar()
		
		#Wykrywanie wciśniętego przycisku, rozpoczyna lub kończy jazdę
		if isButtonPressed() == True:
			if isRunning == True:
				return
			if isRunning == False:
				isRunning = True
				sleep(0.5)
		
		
		if isRunning == True:
			#Kalibruje czujniki o ile nie zostały już skalibrowane
			if isCalibrated == False:
				parametry = calibrateSensors()
				lMaxBlack = parametry[0]
				rMaxBlack = parametry[1]
				isCalibrated = True
				manualSteer = False
			else :		
				
				#Obliczanie uchybu
				lerror = lMaxBlack - lval
				rerror = rMaxBlack - rval
				
				#Obliczanie całki
				lintegral = 0.5 * lintegral + lerror
				rintegral = 0.5 * rintegral + rerror
				
				#Obliczanie pochodnej
				lderivative = lerror - lastErrorL 
				rderivative = rerror - lastErrorR
				
				#Zapisanie poprzedniego uchybu
				lastErrorL = lerror
				lastErrorR = rerror
				
				#Obliczenie sterowania dla każdego z silników
				lcontrol = int((2.6 * lerror + 1 * lintegral + 2.5 * lderivative))
				rcontrol = int((2.6 * rerror + 1 * rintegral + 2.5 * rderivative))
				
				#Manualne sterowanie - jeżli robot wyjedzie za
				#linię to skręca mocno w stronę ostatniego zakrętu
				if lval >= lMaxBlack+1  and rval >= rMaxBlack+1 :
					
					if ostatniSkret == "R" :
						lmotor.run_forever(speed_sp = 450)
						rmotor.run_forever(speed_sp = -20)
						
						if manualSteer == False :
							start = lmotor.position 
							
						if abs(lmotor.position - start) > 270 :
							while getLsensor() >= lMaxBlack  and getRsensor() >= rMaxBlack:
								lmotor.run_forever(speed_sp = -400)
								rmotor.run_forever(speed_sp = 200)

					else :
						lmotor.run_forever(speed_sp = -20)
						rmotor.run_forever(speed_sp = 450 )
						
						if manualSteer == False :
							start = rmotor.position 
						
						if abs(rmotor.position - start) > 270 :
							while getLsensor() >= lMaxBlack  and getRsensor() >= rMaxBlack:
								lmotor.run_forever(speed_sp = 200)
								rmotor.run_forever(speed_sp = -400)
					manualSteer = True
					
				else :
					#Ustalenie prędkości bazowej - im mniejsza różnica
					#sterowania tym proporcjonalnie wyższa prędkość
					predkosc = 500 + int((-1) * (abs(rcontrol - lcontrol)))
					
					#Wysterowanie poszczególnych silników
					lmotor.run_forever(speed_sp = predkosc + (rcontrol-lcontrol)/2)
					rmotor.run_forever(speed_sp = predkosc + (lcontrol-rcontrol)/2)
					
					#Wyłączenie manualnego sterowania
					if manualSteer == True :
						if ostatniSkret == "R" :
							ostatniSkret = "L" 
						else :
							ostatniSkret = "R"
						manualSteer = False
					#Zapisanie ostatniego zakrętu
					else :
						if rcontrol > lcontrol:
							ostatniSkret = "R"
						else :
							ostatniSkret = "L"
				
				#Agorytm omijania przeszkodzy
				if sonar.value() < 11 :
					#Zatrzymanie się
					stopMotors()
					
					#Obrócenie sonaru w kierunku przeszkody
					obroc_sonar(90)
					sleep(0.3)
					
					#Obrócenie robota o 90 stopni
					rotateRobotSym(90, True, 200)
					sleep(0.3) 
					
					#Pobranie pierwszego wskazania sonaru
					wskazanie = sonar.value()
					
					#Jazda w poprzek przeszkody
					lmotor.run_forever(speed_sp = 500)
					rmotor.run_forever(speed_sp = 500)
					while( sonar.value() < 1.3 * wskazanie):
						continue	
					sound.beep()
					sleep(0.08)
					
					#Obrócenie robota, tak aby ustawił się równolegle do przeszkody
					lmotor.run_to_rel_pos(position_sp = 25, speed_sp = 400)
					rmotor.run_to_rel_pos(position_sp = 280, speed_sp = 400)
					while 'running' in lmotor.state or 'running' in rmotor.state :
						sleep(0.01)
					sleep(0.1)
					
					#Pobranie drugiego wskazania sonaru
					wskazanie = sonar.value()
					
					#Jazda wzdłuż przeszkody
					lmotor.run_forever(speed_sp = 550)
					rmotor.run_forever(speed_sp = 550)
					
					#Wykrycie początkowej krawędzi przezkody
					while( sonar.value() > 0.6 * wskazanie) :
						continue
					
					#Wykrycie końca przeszkody
					while( sonar.value() <= wskazanie*0.7) :
						continue
					
					#Obrócenie robota aby wrócił na linię
					lmotor.run_to_rel_pos(position_sp = 25, speed_sp = 400)
					rmotor.run_to_rel_pos(position_sp = 250, speed_sp = 400)
					while 'running' in lmotor.state or 'running' in rmotor.state :
						sleep(0.01)
					
					#Powrót sonaru do pozycji początkowej
					obroc_sonar(-90)
					sleep(0.1)
					
					#Jazda aż do wykrycia linii
					lmotor.run_forever(speed_sp = 500)
					rmotor.run_forever(speed_sp = 500)
					while getRsensor() >= rMaxBlack or getLsensor() >= lMaxBlack :
						continue
				
				#Częstotliwość próbkowania
				sleep(0.001)
				
# Wywołanie głównej funkcji sterującej				
robotFunction()
