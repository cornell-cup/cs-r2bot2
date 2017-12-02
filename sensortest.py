import serial
import time
import io
import sys
import R2Protocol

port = '/dev/ttyACM1'
baudrate = 9600
ser = serial.Serial(port, baudrate, timeout=1, write_timeout=0)
#request1 = R2Protocol.encode(b"PI", b"DRAWER", b"", b"O")
#request2 = R2Protocol.encode(b"DRAWER1", b"PI", b"", b"T")
#print (request1)
#decoded = R2Protocol.decode(request1)
#print (decoded)
#ctr = 0

while True:
    #if ctr == 0:
        #ser.write(request1)
        #print ("sent request 1")
    indata = ser.read(200)
#    print (indata)
    indexFront = indata.find(b'G00')
    indexEnd = indata.find(b'G01')
    if indexFront < indexEnd and indexFront != -1 and indexEnd != -1:
        indata = indata[indexFront:indexEnd+3]
    else:
        indata = None
    if (indata != None):
        decodedData = R2Protocol.decode(indata)
        print (R2Protocol.decode(indata))
#    ctr+=1
#    time.sleep(.5)
