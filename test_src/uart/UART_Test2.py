# Ensure PySerial is installed: pip install pyserial
import serial
import time

# Open serial port
ser = serial.Serial('/dev/ttyTHS1', 115200, timeout=1)

# Check if the serial port is open
if ser.is_open:
    print(f"Serial port {ser.name} opened successfully.")

try:
    # Write data
    ser.write(b'Hello from Jetson\n')
    
    # Wait for data to be transmitted
    time.sleep(1)
    
    # Read data
    while ser.in_waiting > 0:
        incoming_data = ser.readline().decode('ascii')
        print(f"Received: {incoming_data}", end='')

finally:
    # Always close the serial port
    ser.close()
    print("Serial port closed.")


