# !/usr/bin/python
import sys
import serial
import socket
import signal

# Make sure usage is correct
if (len(sys.argv) != 4):
    print("usage: ./baseStation.py /dev/tty.usbXXX SENSOR SAMP_RATE")
    print("SENSOR = 'ACC' || 'FLEX'")
    print("SAMP_RATE = '5' || '50'")
    sys.exit(1)

port = sys.argv[1]
sensor = sys.argv[2]
rate = sys.argv[3]

print("Port: ", port)
print("Sensor: ", sensor)
print("Rate: ", rate)


# Register signal and define signal function
def signal_handler(signal, frame):
    print('\nExiting with the highest amount of grace')
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)
