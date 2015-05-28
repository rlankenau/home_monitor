import serial
import httplib 
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

while True:
    line = ser.readline()
    if line:
        print line
        try:
            thingspeak = httplib.HTTPSConnection('api.thingspeak.com')
            thingspeak.request("GET", '/update?key=LNCS1U0RV05E9XH7&' + line)
            res = thingspeak.getresponse()
            print res.status, res.reason
            data = res.read()
            thingspeak.close();
            print data
        except httplib.BadStatusLine as e:
            print "Server returned something we didn't understand: " + e.__str__()
        except Exception as e:
            print "Error submitting request.  Skipping."
    else:
        print "No data."

