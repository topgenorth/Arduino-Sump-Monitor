#!/usr/bin/env python
import time
loops = 4
count = 0
while (count < 3):
    #execfile("depthPoll.py")
#    print "Start : %s" % time.ctime()
    if count == loops-1:
        break
    time.sleep( 10 )
    count = count + 1
#print "End : %s" % time.ctime()