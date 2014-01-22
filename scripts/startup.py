#!/usr/bin/env python
#
# Sends notification via AWS SNS (Amazon's messaging service) every time the raspberry pi starts up

from boto.dynamodb2.table import Table
from boto.dynamodb2.table import Item
import time, boto

sns = boto.connect_sns()
now = long(time.time())
startuptime = time.strftime('%H:%M:%S',  time.localtime(now))
arn = "arn:aws:sns:us-east-1:361127226605:SumpPumpAlerts"
msg = "Telemetry Pi successful startup at " + startuptime
subj = "Telemetry Pi booted at  " + startuptime
sns.publish(arn, msg, subj)

# Store SMS message to DynamoDB for logging purposes:
reading = Table('sns_alerts')
new_entry = Item(reading, data={
    'type': 'bootup',
    'date': now,
    'subject': subj,
    'message': msg,
})
new_entry.save()
True
print subj