COM_PORT = "COM5"
BAUD_RATE = 230400
DELAY_MS = 0

BEGIN_DELIMITER = '!'
END_DELIMITER = ';'

LOREM_IPSUM = "Lorem ipsum dolor sit amet, consectetur adipiscing elit sed do eiusmod tempor incididunt" \
              " ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco" \
              " laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in" \
              " voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat" \
              " non proident, sunt in culpa qui officia deserunt mollit anim id est laborum? "

import serial
from serial import SerialException
import sys
import time
import threading
import logging

try:
    serialPort = serial.Serial(COM_PORT, BAUD_RATE, bytesize=8, timeout=0, stopbits=serial.STOPBITS_ONE,
                               rtscts = False,dsrdtr = False,write_timeout=0,inter_byte_timeout=None)
except SerialException:
    print("Serial connection not available")
    sys.exit()

if not serialPort.isOpen():
    serialPort.open()

#Inicializo el buffer de recepcion
packedBytes = bytearray()

print("Connection established")
print('\t' + "Port: " + COM_PORT)
print('\t' + "Baud Rate: " + str(BAUD_RATE) + '\n')
print("Starting transmission...")
serialPort.reset_output_buffer()
serialPort.reset_input_buffer()
time.sleep(2)
print("------------------------------------------\n")

start = time.time()
while serialPort.isOpen():
    msgTx = "250,30"
    msgTx = BEGIN_DELIMITER + msgTx + END_DELIMITER
    serialPort.write(msgTx.encode('utf-8'))

    newByte = serialPort.read()
    if newByte == B'?':
        msg = packedBytes.decode('utf-8')
        if msg:
            print(msg)
            print("Received bytes: " + str(len(packedBytes)))
            end = time.time()
            print("Transfer speed: " + str(len(packedBytes)/float(end - start)))
            start = time.time()
        packedBytes.clear()
    else:
        packedBytes += newByte

serialPort.close()
sys.exit()
