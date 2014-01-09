Contact Info
Author:		John Banta
Email:		john.banta@gmail.com
Build Info: http://bantacafe.com/2013/04/monitoring-sump-pit-water-level-wth-arduino/
Credits:	To be added

This collection of scripts was written for an Arduino Uno microcontroller and Raspberry Pi for the purpose of remotely monitoring the water level in my basement sump pit.  The water depth is read using a ultrasonic rangefinder and data is logged to Amazon's DynamoDB for cloud storage.

The python script also monitors for rising water levels and triggers an SMS message if the water depth exceeds a critical level. 

Hardware for this project includes:
a. Arduino Uno
b. Raspberry Pi
c. HC-SR04 ultrasonic rangefinder
d. Piezo buzzer
e. Mini breadboard

The need came about because of flooding in our neighborhood and having 
witnessed several of our neighbors suffering from flooded basements
after their sump pump was overwhelmed with incoming water or the pump 
simply failed and never came on.  

I wanted a way to be able to 'see' how full my sump pit was without 
removing the lid.  As the project progressed, I added remote monitoring 
so that water level data is pushed to an online data collector and 
available for alarming if level exceeds a set threshold.

Basic Flow:
a. initiate ethernet connection
b. tweet startup message acknowledging activation of sump monitor
<loop>
c. Read water depth sensor (currently capacitive sensor but switching to sonar or pressure differential method)
d. Write water depth value to cloud (currently thingspeak but switching to AWS DynamoDB)
e. If water depth > alert threshold, tweet alert of sump filling up
f. if water depth > warning threshold, tweet alert and trigger audible alarm
g. pause 15 seconds before looping
</loop>