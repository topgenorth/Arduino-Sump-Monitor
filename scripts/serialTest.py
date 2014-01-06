#!/usr/bin/python
#
# Let's retrieve some weather data!
import time
import serial

print("Opening serial port")
conn = serial.Serial('/dev/tty.usbmodem641', 9600)

print("sleeping")
time.sleep(2)
print("writing to port")
conn.write('get;')
print("reading from port")
response = conn.readline()
conn.close()

print response
