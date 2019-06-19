##Inlcude part

##Def
##Node
NodeURL="https://nodes.thetangle.org:443"
seed =
address = 
##Functions (if there are any)

def main():
    ##Main starts here
    ##Configure SPI
    ConfigureGPIO()
    no_of_devices = ConfigureSPI_0() #ADE set
    #giveback the number of devices
    
    ConfigureSPI_1()# *Optional communication
    ConfigureSerial()# CDMA communication
    
    #Build a data set template that will be used from now on in the message composition.
    transformerMap = BuildDataMap(no_of_devices)
    
    
    #Fill up the map with the readings
    for x in range no_of_devices:
        transformerMap[x] = ReadADE(x)
        
    #Convert the map to Trytes (Tangle message)   
    message = Bytestotrytes(transformerMap)
    
    #Data Transfer
    data_transfer(message,seed,address)
    
    #Safety check
    ReadDHT11(inside)
    ReadDHT11(outside)
    
    #Set PWM to the new level
    DeterminePWM()

    ##Main ends here
    

main() #run the main    
