all: robot utilityFile 

robot: robot.py
	scp robot.py robot@10.42.0.3:py/robot.py

utilityFile: utilityFile.py
	scp utilityFile.py robot@10.42.0.3:py/utilityFile.py

