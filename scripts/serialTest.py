#!/usr/bin/python
#
# Let's retrieve some weather data!
import time
import serial

print("Opening serial port")
conn = serial.Serial('/dev/ttyACM0', 115200)
#conn = serial.Serial('/dev/tty.usbmodem641', 9600)

print("sleeping")
time.sleep(2)

print("Triggering ping sensor")
conn.write('get;')
print("reading from ping sensor")
pingresponse = conn.readline()
print pingresponse

print("Triggering pressure sensor")
conn.write('kpa;')
print("reading from pressure sensor")
pressureresponse = conn.readline()
print pressureresponse

conn.close()