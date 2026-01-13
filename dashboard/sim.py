import random as rand
from datetime import datetime
import http.client
import json

# by parsing data, I found that the range between the highest and lowest values was 2
# this code works based off that assumption 

closed_vals = [12, 30] # these are the values it should roughly take on when closed, depending on position
range = 2
def gen_sonar(val, range):
    return rand.uniform(val-(range/2), val+(range/2))

def gen_time():
    t = datetime.now().time()
    return (t.hour * 3600 + t.minute * 60 + t.second) * 1000000 + t.microsecond

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
data = [{"sonar_distance": f"{gen_sonar(closed_vals[0], range)}", "time" : f"{gen_time()}" }]
x.post_data(data)