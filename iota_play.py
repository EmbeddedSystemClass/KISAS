#Librarii
import iota
from iota import TryteString, Iota, Address
from iota.crypto.types import Seed
import json
from pprint import pprint
import sys
from gpiozero import MCP3008, PWMLED
import time
import unicodedata
##
##Node
NodeURL="https://nodes.thetangle.org:443"
##
def data_transfer(msg,sed,adr):
    sys.stdout = open('data_transfer_log.txt', 'wt')
    print("Data has been collected.",file=open("data_transfer_log.txt","a"))
    api=iota.Iota(NodeURL, seed=sed)
    pt = iota.ProposedTransaction(address = iota.Address(adr), message = msg, value = 0)
    print("Data:",file=open("data_transfer_log.txt","a"))
    print(msg,file=open("data_transfer_log.txt","a"))
    print("Address:",file=open("data_transfer_log.txt","a"))
    print(adr,file=open("data_transfer_log.txt","a"))
    FinalBundle = api.send_transfer(depth = 3, transfers = [pt], min_weight_magnitude=14)['bundle']
    print("Done.",file=open("data_transfer_log.txt","a"))
    return;
    

##Convert data to Alien Language
def text_man(your_data_string):
    return iota.TryteString.from_unicode(your_data_string);


##The end of my Magic Book...



#print(text_)
my_address = "MAPHGBPNGBK9IHDMCCARYCALRJG9PUPMEXGGMISLYPLYGICGWEJ9OJAFUVKFLYDRETCVJYXH9VNQZAXP9ARNAYUCD9"
my_seed = "BVGZAJBCQKOZPXKDCXYIADRBRLD9L9LKSHMZUOSAQZATOQWHXU9BMPWWGCOFKNFKWVFPZJSCOHMKMAJQH"
print ("allgood",my_address)

for x in range(0,10):
    text_ = input("IoTA message?")
    text_=text_man(text_)
    data_transfer(text_,my_seed,my_address)

