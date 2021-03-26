COM_PORT = "COM5"
BAUD_RATE = 230400
DELAY_MS = 0

import serial
from serial import SerialException
import sys
import time


try:
    serialPort = serial.Serial(COM_PORT, BAUD_RATE, bytesize=8, timeout=0, stopbits=serial.STOPBITS_ONE,
                               rtscts = False,dsrdtr = False,write_timeout=1,inter_byte_timeout=None)
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

print("------------------------------------------\n")

while serialPort.readable() == False or serialPort.writable == False:
    time.sleep(1)

time.sleep(4)
serialPort.reset_output_buffer()
serialPort.reset_input_buffer()

start = time.time()
count = 0
while serialPort.isOpen():
    newByte = serialPort.read()
    if newByte == B';':
        end = time.time()
        totalBytes = len(packedBytes)
        msg = packedBytes.decode('utf-8')
        print("Received data: " + str(totalBytes) + " ; " +
              "Transfer speed: {:.1f}".format(totalBytes / (end - start)) + " Bytes/s ;" +
              "Elapsed time: " + str(end-start))
        # print("CIAA>" + msg)
        packedBytes.clear()
        start = time.time()

    else:
        packedBytes += newByte

serialPort.close()
sys.exit()
