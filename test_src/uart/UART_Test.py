# Ensure PySerial is installed: pip install pyserial
import serial
from time import sleep 
# ttyTHS0,ttyTHS2,ttyS0,ttyS1,ttyS2,ttyS3,
ser = serial.Serial('/dev/ttyS1', 115200)  # Open port with baud rate
for i in range (1000000):
	ser.write(b'Hello%d\n'%i)  # Send a string
	sleep(0.1)
ser.close()  # Close port
