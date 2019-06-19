from gpiozero import MCP3008, PWMLED
import time
MCP3008(channel=0, clock_pin=11, mosi_pin=10, miso_pin=9, select_pin=8)
#from gpiozero import MCP3008

var=1
while var == 1:
    pot = MCP3008(0)
    print(pot.value)
    time.sleep(2)



