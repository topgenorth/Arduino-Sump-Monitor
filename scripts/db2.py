from boto.dynamodb2.table import Table
from boto.dynamodb2.table import Item
import time
reading = Table('sump')
#print reading

polltime = long(time.time())
level = 30

new_entry = Item(reading, data={
    'Id': 'sump',
    'Date': polltime,
    'Level': level,
})

new_entry.save()
True