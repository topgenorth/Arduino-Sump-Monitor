Contact Info
Author:		John Banta
Email:		john.banta@gmail.com
Build Info: http://bantacafe.com/2013/04/monitoring-sump-pit-water-level-wth-arduino/
Credits:	To be added

This script was written for an Arduino Uno microcontroller for the purpose of 
remotely monitoring the water level in my basement sump pit.

Hardware for this project includes:
a. Arduino Uno
b. Arduino ethernet shield (for sending data to cloud)
c. Capacitive Sensor - made from approx $15 in parts from Home Depot
d. LCD screen (for local display of water depth)
e. Potentiometer for controlling LCD contrast
f. various resistors

The need came about because of flooding in our neighborhood and having 
witnessed several of our neighbors suffering from flooded basements
after their sump pump was overwhelmed with incoming water or the pump 
simply failed and never came on.  

I wanted a way to be able to 'see' how full my sump pit was without 
removing the lid.  As the project progressed, I added remote monitoring 
so that water level data is pushed to an online data collector and 
available for alarming if level exceeds a set threshold.

-John
