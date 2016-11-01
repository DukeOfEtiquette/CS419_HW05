# !/usr/bin/python
import sys
import serial
import signal
import datetime
import time

# Make sure usage is correct
if len(sys.argv) != 4:
    print("usage: ./baseStation.py /dev/tty.usbXXX SENSOR SAMP_RATE")
    print("SENSOR = 'ACC' || 'FLX'")
    print("SAMP_RATE = '5' || '50'")
    sys.exit(1)


# Register signal and define signal function
def signal_handler(signal, frame):
    print('\nExiting with the highest amount of grace')
    ser.close()
    sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)

# Grab command line args and store them
port = sys.argv[1]
sensor = sys.argv[2]
rate = sys.argv[3]

# Open serial port to arduino
print("Opening serial port...")
try:
    ser = serial.Serial(port, 9600)
    print("Serial port opened successfully.")

    # Take a quick nap
    time.sleep(1)
except:
    print("Unable to open serial port, closing.")
    sys.exit(1)

# Initialize command string that will be sent to the arduino
cmdToSend = ''

# Determine which sensor will be used
# a == accel, f = flex
if sensor[0].lower() == 'a':
    cmdToSend += 'a'
elif sensor[0].lower() == 'f':
    cmdToSend += 'f'
else:
    print("Sensor arg invalid, exiting.")
    ser.close()
    sys.exit(1)

# Determine which rate will be used
# s = slow (5hz), q = quick (50hz)
if rate == '5':
    cmdToSend += 's'
elif rate == '50':
    cmdToSend += 'q'
else:
    print("Rate arg invalid, exiting.")
    ser.close()
    sys.exit(1)

sys.stdin.flush()

# Get input from user on if they want to start or quit
while True:
    usrInput = input("Enter 'start' to start sampling, or 'quit' to quit: ")

    # If they want to start sampling...
    if usrInput[0].lower() == 's':
        # Send command to the arduino
        print("Sampling started. Press ctrl+c to stop sampling.")
        res = ser.write(cmdToSend.encode('utf-8'))
        break;
    elif usrInput[0].lower() == 'q':
        print("Quiting...")
        ser.close()
        sys.exit(1)
    else:
        print("Invalid option.")

fileName = datetime.datetime.now().strftime('%m_%d_%Y_%H_%M_%S_')
fileName += '{0}_{1}.csv'.format(sensor, rate)

try:
    file = open(fileName, 'w')
    print("Results will be saved in {0}.".format(fileName))
except IOError:
    print("Unable to open {0} to save results. Sampling results will not be saved.".format(fileName))

results = ''

while True:
    results = ser.readline().strip()
    results = results.decode()
    sys.stdout.flush()

    if res == '':
        print("Nothing received from arduino, exiting.")
        ser.close()
        sys.exit(1)

    print("Received:", results)

    file.write('{0},{1}\n'.format(results, time.strftime('%H:%M:%S')))

