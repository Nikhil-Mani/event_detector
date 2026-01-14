import random as rand
from datetime import datetime
import http.client
import json

# by parsing data, I found that the range between the highest and lowest values was 2
# this code works based off that assumption 

closed_vals = [12, 30] # these are the values it should roughly take on when closed, depending on position
r = 2
def gen_sonar(val, spread):
    return rand.uniform(val-(spread/2), val+(spread/2))

def gen_time():
    t = datetime.now().time()
    return (t.hour * 3600 + t.minute * 60 + t.second) * 1000000 + t.microsecond
def gen_post(val, r):
    data = []
    for i in range(5):
        data.append({"sonar_distance": f"{gen_sonar(val, r)}", "time" : f"{gen_time()}"})
    return data
class HTTP:
    def __init__(self, url: str):
        self.url = url
        self.conn = http.client.HTTPConnection(url)
        self.headers  = {"Content-Type": "application/json"}
    
    def post_data(self, data: list[dict]):
        self.conn.request("POST", "/post", json.dumps(data), self.headers)
        response = self.conn.getresponse()
        print(response.status, response.reason)

x = HTTP("10.203.116.51:5001")
data = gen_post(closed_vals[0], r)
x.post_data(data)