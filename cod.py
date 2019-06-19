import iota
from iota import TryteString, Iota, Address
from iota.crypto.types import Seed
import json
from pprint import pprint
NodeURL="https://nodes.thetangle.org:443"
Theoseed = Seed.random()
pprint(Theoseed)
api=iota.Iota(NodeURL, seed=Theoseed)
result=api.get_new_addresses(index = 0, count = 3, security_level= 2)
pprint(result)
##
##
##
##
##
##
addr= result['addresses']
pprint(addr)

addressphilip = str(addr[0].with_valid_checksum())
helo= iota.TryteString.from_unicode('5V 1A 4A 6A')
pprint(helo)
pt = iota.ProposedTransaction(address = iota.Address(addressphilip), message = helo, value = 0)
pprint('Please wait...')
FinalBundle = api.send_transfer(depth = 3, transfers = [pt], min_weight_magnitude=14)['bundle']
pprint('Done ')
print('Funds send from', addr[0])