import csv
from datetime import datetime
import time

with open('EURUSD_M1_2000_2016.csv', 'rb') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in spamreader:
        s = row[0]+" "+row[1]
        datetime_object = datetime.strptime(s, '%Y.%m.%d %H:%M')
        print '%i,%s' % (time.mktime(datetime_object.timetuple()), row[2])