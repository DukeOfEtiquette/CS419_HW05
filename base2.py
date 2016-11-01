#!/usr/bin/python

import serial
import time
import sys

arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=5)
time.sleep(2)

arduino.flush()
arduino.write('N')

time.sleep(.1)

arduino.flush()
arduino.write('F')

while True:
	msg = arduino.read(arduino.inWaiting())
	time.sleep(1)
	sys.stdout.flush()
	print(msg)
