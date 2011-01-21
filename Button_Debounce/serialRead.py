import serial
ser = serial.Serial(5)

if ser:
        print 'port opened'
        ser.open()
        
if ser.write('1'):
        print 'Write success'
else:
        print 'write failed'
        
while 1:
	print(ser.readline())
