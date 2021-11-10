#!/usr/bin/env python3
import serial 	#Imports the serial library 

if __name__ == '__main__':
	#Initialize serial communication ('serial device', baud rate, timeout period)
	#serial.Serial() returns an object that can be used for all serial operations, we store it to ser 
	#timeout is the time that the device will wait to read from serial
	ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
	ser.flush() #flushes any input and output in the buffer 

	while True: 
		if ser.in_waiting>0: #check if data is avalible 
			line = ser.readline().decode('utf-8').rstrip() #readline() reads all bytes untill a newline character is detected, rstrip() removes any trailing characters (\r \n, etc.) 
			print(line) 
