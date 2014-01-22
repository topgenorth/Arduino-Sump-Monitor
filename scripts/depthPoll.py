#!/usr/bin/env python
#
# Poll arduino for latest water level in sump pit, post value to dynamodb,
# and send SMS text alert if water depth reaches critical level.

import datetime, time, serial, sys, boto

## Script Configurations ##
killswitch = 0 			            # Master kill switch: set to 1 to disable the script from running
sumpdepth = 60 			            # Total height (in cm) of the sump pit
alertthreshold = 30 		        # Water level (in cm) must reach this height for the SMS alert feature to activate
notificationkillswitch = 0          # Kill switch to disable SMS notifications: set to 1 to disable
arduino = "/dev/ttyACM0" 	        # Serial port for Arduino on Raspberry Pi
#arduino = "/dev/tty.usbmodem641"   # Serial port for Arduino on Mac
arduinospeed = 115200               # Baud rate with Arduino 

# Path to our RRD
##rrdfile = "/home/peter/weather/weather.rrd"
# Path to our log file
##logfile = "/home/peter/weather/weather.log"

# Retrieve sensor data
conn = serial.Serial(arduino, arduinospeed, timeout=1)
time.sleep(2)
conn.write("req;")
datain = conn.readline()

# Sensor data is received in the form
# "<channel number>,<water depth>,<pressure sensor value>;\n"
# First we split this out to a list of values,
# then cast each to numeric values and assign to variables.
x = datain.split(";")[0].split(",")
if x[0] is "1":
    depth = sumpdepth - int(x[1]) #calculate water depth as sump pit depth minus distance to waterline (returned from arduino)
    print("Received %s from hc-3604" % (depth))
    pressure = int(x[2]) #report value from pressure sensor to determine inches of water exerting force on airtube at bottom of pit (returned from arduino)
    print("Received %s from mpx5050dp" % (pressure))
else:
    print("Unknown channel received: %s" % (x[0]))
    sys.exit(1)

# Figure out the time the recording was taken.
# Need to figure out how to accomodate for DST
# offset = 60*60*6 # we'll subtract 6 hours worth of seconds to adjust timezone from GMT
polltime = long(time.time())

# Write data to Amazon's dynamodb for cloud storage
from boto.dynamodb2.table import Table
from boto.dynamodb2.table import Item
reading = Table('sump')
new_entry = Item(reading, data={
    'Id': 'sump',
    'Date': polltime,
    'Level': depth,
    'Pressure': pressure,
})
new_entry.save()

# Push SMS alert to phone if sump reaches critical level
# Recipient ph # is managed in the AWS SNS admin
# TO-DO: 
    # Check dynamodb to see how many messages sent in last 60 mins.  
    # If 3, mention in message that it will be the last one for x mins.
    # if 4, do not send
    # put url to chart in msg
if notificationkillswitch == 0:
    if depth > alertthreshold:
        sns = boto.connect_sns()
        alerttime = time.strftime('%H:%M:%S',  time.gmtime(polltime))
        arn = "arn:aws:sns:us-east-1:361127226605:SumpPumpAlerts"
        msg = "Water level reached " + str(depth) + "cm at " + alerttime
        subj = "Water level: " + str(depth) + "cm at " + alerttime
        sns.publish(arn, msg, subj)
        # Store SMS message to DynamoDB for logging purposes:
        message = Table('sns_alerts')
        new_entry = Item(message, data={
            'type': 'sump',
            'date': polltime,
            'subject': subj,
            'message': msg,
        })
        new_entry.save()

print("collected at %s" % (polltime))

# Write values to RRD
##rrdtool.update(rrdfile,
##               "--template", "temperature:humidity",
##               "%s:%s:%s" % (polltime, temperature, humidity))
# Write values to logfile
##now = datetime.datetime.now()
##lfh = open(logfile, 'a')
##lfh.write("%s %s:%s:%s\n" % (now.strftime("%b %d %H:%M:%S"), polltime,
##                           temperature, humidity))
##lfh.close()
