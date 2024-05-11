import serial
from time import sleep 
# ttyTHS0 --> UART1(Pin.8-Txd,Pin.10-Rxd) (40pin Expansion Header)
# ttyUSB0-FTDI IC[V:0403:D:6001] OK!!
# Open the serial port (replace '/dev/ttyTHS1' with the appropriate device file)
ser = serial.Serial('/dev/ttyTHS1', baudrate=115200, timeout=1)

# Send data
for i in range (10):
	ser.write(b'Hello, UART%d!\r\n'%i)
	print("send:%d",i)
	sleep(0.033)

# Read data
for i in range (1000):
      #  ser.write(b'0123456789')
	data = ser.read(10)  # Read 10 bytes
	print("Received:", data)
	ser.write(b'0123456789')
	sleep(0.5)
# Close the serial port
ser.close()


