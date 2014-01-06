#!/usr/bin/python
import boto
sns = boto.connect_sns()
sns

arn = "arn:aws:sns:us-east-1:361127226605:SumpPumpAlerts"
msg = "Hello there\nI am sending a test"
subj = "SNS Test Msg"
res = sns.publish(arn, msg, subj)
res