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

#reading.put_item(data={
#    'Id': {'S': 'sump'},
#    'Date': {'N': 2246810},
#    'Level': {'S': '22'},
#},
#expected={
#    'Date': {'Exists': False},
})

new_entry.save()
True