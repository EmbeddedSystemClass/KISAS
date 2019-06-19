##POST Request for HTTP
##Libraries
import requests
import time
from requests.auth import HTTPBasicAuth
import json

##Login/Security
url='http://liandon-meetdata.nl/Kisas/Post.php'
auth_data = {
            'username': '',
            'password': 'NycMVVlqltALghkLrtkyLr2YvLeA6x1k'
        }

headers = {"Content-Type" : "application/json"}

payload = {"Src":100001,"Ptp":2,"Pid":10,"Fmt":0,"DTm": int(time.time()),"MV":[{"Adr":60415,"Val":4.12345},{"Adr":60398,"Val":3.22245}]}
print(time.time())
myrequest = requests.post(url, data=json.dumps(payload), auth=('100001', 'NycMVVlqltALghkLrtkyLr2YvLeA6x1k'))
print(time.time())
print(myrequest.content)
print(myrequest.status_code)



