import serial
import sys

if __name__ == "__main__":
    s = serial.Serial(sys.argv[1], 9600, timeout=1)
    s.write("\nL\0\nR\0\n")
    s.close()

