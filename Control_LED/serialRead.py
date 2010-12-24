import serial
ser = serial.Serial('COM7', 9600, timeout=.2)
ser.close()

if ser:
        print 'port opened'
        ser.open()
        
if ser.write('e'):
        print 'Write success'
else:
        print 'write failed'
        
while 1:
	print(ser.readline())
