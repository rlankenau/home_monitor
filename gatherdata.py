import serial
import httplib 
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
thingspeak = httplib.HTTPSConnection('api.thingspeak.com')

while True:
    line = ser.readline()
    if line:
        print line
        thingspeak.request("GET", '/update?key=LNCS1U0RV05E9XH7&' + line)
        res = thingspeak.getresponse()
        print res.status, res.reason
        data = res.read()
        print data
    else:
        print "No data."

