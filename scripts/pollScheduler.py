#!/usr/bin/env python
import time
loops = 3
count = 0
while (count < 3):
    execfile("/home/johnbanta/Desktop/sumpMonitor/scripts/depthPoll.py")
#    print "Start : %s" % time.ctime()
    if count == loops-1:
        break
    time.sleep( 17 ) # reduce desired delay by 2 seconds since depthPoll sleeps for 2 secs
    count = count + 1
#print "End : %s" % time.ctime()
