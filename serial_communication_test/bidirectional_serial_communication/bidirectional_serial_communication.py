#!/usr/bin/env python3

import serial	#Import Serial library
import time 	#Import Time library 

if __name__ == '__main__':
	#Inializes Serial comunication (device, baud rate, timeout period) and saves a serial object to ser
	ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
	ser.flush() 
	
	while True:
		ser.write(b"Sent from Raspberry Pi \n")		#sends data to the Arduino, the b encodes the string to bytes 
		line = ser.readline().decode('utf-8').rstrip() 	#Read data in from the buffer 
		print(line)					#Print the data to the terminal
		time.sleep(1)					#Wait for 1 second 
