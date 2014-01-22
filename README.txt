Contact Info
Author:		John Banta
Email:		john.banta@gmail.com
Build Info: http://bantacafe.com/2013/04/monitoring-sump-pit-water-level-wth-arduino/
Credits:	To be added

This collection of scripts was written for an Arduino Uno microcontroller and Raspberry Pi to remotely monitor the water level in my basement sump pit.  The water depth is read using a ultrasonic rangefinder and data is logged to Amazon's DynamoDB for cloud storage.

A python script also monitors for rising water levels and triggers an SMS message if the water depth exceeds a critical level. 

Hardware for this project includes:
a. Arduino Uno
b. Raspberry Pi
c. HC-SR04 ultrasonic rangefinder
d. Piezo buzzer
e. Mini breadboard

The need came about because of flooding in our neighborhood and having witnessed several of our neighbors suffering from flooded basements after their sump pump was overwhelmed with incoming water or the pump simply failed and never came on.  

I wanted a way to be able to know how full my sump pit was without removing the lid.  It includes remote logging and monitoring so that water level data is pushed to an online data collector and available for alarming if level exceeds a set threshold.

Basic Flow:
a. Python script on Raspberry Pi solicits Arduino for current water depth measurement
b. Current depth and timestamp are written to Amazon's DynamoDB for online storage
c. If depth exceeds a critical value, script triggers an alert to Amazon's SNS which in turn notifies subscribed devices via SMS or email
d. Repeat process every 20 seconds
e. Accessing html page queries DynamoDB for recent trends and displays chart

Files:
- charts
 -- index.html		Dashboard webpage that queries for data from DynamoDB and displays recent water line trend
- README.txt		This file
- schematic.fzz		Schematic diagram showing how the hardware is connected
- scripts
 -- aws
 -- db2.py			Basic test script to ensure you can write to AWS DynamoDB
 -- depthPoll.py	Script that polls arduino for distance and pushes data to AWS and SNS when water level high
 -- pollScheduler.py	Scheduler script that runs every minute via cron.  It in turn calls depthPoll.py twice at 20 second intervals.  This gets around cron's limitation of running no more frequently than once per minute.
 -- serialTest.py	Test script that calls Arduino for water depth and ensures responses are received via serial.
 -- sns.py			Test script to push a message to Amazon SNS for alerting via SMS, email, etc.
- sumpMonitor.ino	Arduino sketch which listens for request from Raspberry Pi and returns distance from sensor to water level