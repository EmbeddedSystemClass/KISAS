import RPi.GPIO as GPIO
from DHT11_Python import dht11
import time
import datetime
from numpy import *
from matplotlib.pyplot import plot, show
import numpy as np



# initialize GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(13, GPIO.OUT) #pwm

# read data using pin 14
instance = dht11.DHT11(pin=14) #DHT data line
pwm = GPIO.PWM(13,100)
#Main Program
x= arange(0,100,1)
while True:
    for buffer in range (100):
        result = instance.read()
        if result.is_valid():
            print("Last valid input: " + str(datetime.datetime.now()))
            print("Temperature: %d C" % result.temperature)
            print("Humidity: %d %%" % result.humidity)       
            y[buffer]=result.temperature
    time.sleep(1)
    #continue()


plot (x,y)
show()