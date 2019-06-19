#ADC to Tangle
##
##
#Librarii
import iota
from iota import TryteString, Iota, Address
from iota.crypto.types import Seed
import json
from pprint import pprint
import sys
from gpiozero import MCP3008, PWMLED
import time
##
##Node
NodeURL="https://nodes.thetangle.org:443"
##

#Functions
##Create a Wallet
def create_one_miniwallet():
    sys.stdout = open('wallet.txt', 'wt')
    miniwallet_seed = Seed.random()
    print("This is your new Seed:",file=open("wallet.txt","a"))
    print(miniwallet_seed,file=open("wallet.txt","a"))
    api=iota.Iota(NodeURL, seed=miniwallet_seed)
    address_set = api.get_new_addresses(index = 0, count = 2, security_level= 2)
    address_set = address_set['addresses']
    print("This is your new Set of Addresses:",file=open("wallet.txt","a"))
    address_one = str(address_set[0].with_valid_checksum())
    address_two = str(address_set[1].with_valid_checksum())
    print("Your Brand new Address 1 :",file=open("wallet.txt","a"))
    print(address_one,file=open("wallet.txt","a"))
    print("Your Brand new Address 2 :",file=open("wallet.txt","a"))
    print(address_two,file=open("wallet.txt","a"))
    return (address_one,address_two,miniwallet_seed);


##Transfer Data with the Tangle
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

My_new_fkin_wallet = create_one_miniwallet()
var=1
led1 = PWMLED(5)
led2 = PWMLED(6)
led3 = PWMLED(13)
led4 = PWMLED(19)
led5 = PWMLED(26)
while var == 1:
    pot  = MCP3008(0)
    print(pot.value*10)
    time.sleep(10)
    print("Reading:")
    ##tatara = ascii(pot.values)
    ##tatara=str(float(tatara))
    led1.source = pot.values
    led2.source = pot.values
    led3.source = pot.values
    led4.source = pot.values
    led5.source = pot.values
    a_piece_of_spam = text_man("SPAM")
    data_transfer(a_piece_of_spam,My_new_fkin_wallet[2],My_new_fkin_wallet[0])
    