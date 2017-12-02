import serial

port = '/dev/ttyACM0'
baudrate = 9600

ser = serial.Serial(port, baudrate, timeout=1, write_timeout=5)

while True:
    indata = ser.read(200)
    print(indata)