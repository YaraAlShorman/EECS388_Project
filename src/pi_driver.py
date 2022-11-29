# By Yara Al-Shorman
# 11/29/2023
import serial

ser1 = serial.Serial('/dev/ttyAMA1', 115200)

while True:
    speed = raw_input("Enter speed: ")
    angle = raw_input("Enter angle: ")
    command = "speed: " + speed + "angle: " +  angle
    ser1.write(bytes(command))

ser1.close()

