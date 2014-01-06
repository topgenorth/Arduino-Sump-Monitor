#!/usr/bin/env python
#
# Poll arduino for latest water level in sump pit, post value to dynamodb,
# and send SMS text alert if water depth gets critical.

import datetime, time, serial, sys, boto

# Serial device properties
##arduinodev = "/dev/ttyACM0" # Raspberry Pi settings
arduinodev = "/dev/tty.usbmodem641" # Mac settings
arduinospeed = 115200
# Path to our RRD
##rrdfile = "/home/peter/weather/weather.rrd"
# Path to our log file
##logfile = "/home/peter/weather/weather.log"

# Retrieve sensor data
conn = serial.Serial(arduinodev, arduinospeed, timeout=1)
time.sleep(2)
conn.write("get;")
datain = conn.readline()

# Sensor data is received in the form
# "<channel number>,<water depth>;\n"
# First we split this out to a list of values,
# then cast each to numeric values and assign to variables.
x = datain.split(";")[0].split(",")
if x[0] is "1":
    depth = float(x[1])
    print("Received %s" % (depth))
else:
    print("Unknown channel received: %s" % (x[0]))
    sys.exit(1)

# Figure out the time the recording was taken.
# Need to figure out how to accomodate for DST here
offset = 60*60*6 # we'll subtract 6 hours worth of seconds to adjust timezone
polltime = long(time.time()-offset)

# Write data to Amazon's dynamodb for cloud storage
from boto.dynamodb2.table import Table
from boto.dynamodb2.table import Item
reading = Table('sump')
new_entry = Item(reading, data={
    'Id': 'sump',
    'Date': polltime,
    'Level': depth,
})
new_entry.save()

# Push SMS alert to phone if sump reaches critical level
# Recipient ph # is managed in the AWS SNS admin
# TO-DO: 
    # Check dynamodb to see how many messages sent in last 60 mins.  
    # If 3, mention in message that it will be the last one for x mins.
    # if 4, do not send
    # put url to chart in msg
if depth > 90:
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