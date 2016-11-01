# !/usr/bin/python
import sys
import serial
import socket
import signal
import time

# Make sure usage is correct
if (len(sys.argv) != 4):
    print("usage: ./baseStation.py /dev/tty.usbXXX SENSOR SAMP_RATE")
    print("SENSOR = 'ACC' || 'FLEX'")
    print("SAMP_RATE = '5' || '50'")
    sys.exit(1)


# Register signal and define signal function
def signal_handler(signal, frame):
    print('\nExiting with the highest amount of grace')
    sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)
port = sys.argv[1]
sensor = sys.argv[2]
rate = sys.argv[3]

sys.stdout.flush()
ser = serial.Serial(port, 9600)

ser.flushOutput()
ser.flushInput()

if(ser.isOpen()):
    print("GOODTOGO")

res = ser.write(sensor.encode())
ser.flushOutput()
ser.flushInput()

print("res: ", res)
out = ''

time.sleep(1)

#print("waiting...")
while True:
    print("hi")
    out += ser.readline()
    print(">> ", out)

#if out != '':

#print("Msg: ", msg.decode())
print("Port: ", port)
print("Sensor: ", sensor)
print("Rate: ", rate)

