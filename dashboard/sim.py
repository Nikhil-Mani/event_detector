import random as rand
from datetime import datetime
import http.client
import json
import time 
# by parsing data, I found that the range between the highest and lowest values was 2
# this code works based off that assumption 

closed_vals = [12, 30] # these are the values it should roughly take on when closed, depending on position
open_vals = [63, 122]
r = 2
class HTTP:
    def __init__(self, url: str):
        self.url = url
        self.conn = http.client.HTTPConnection(url)
        self.headers  = {"Content-Type": "application/json"}
    
    def post_data(self, data: list[dict]):
        self.conn.request("POST", "/post", json.dumps(data), self.headers)
        response = self.conn.getresponse()
        print(response.status, response.reason)

class Simulation:

    def __init__(self, spread, url):
        self.spread = spread
        self.state = False # True is open, closed is False
        self.url = url
        self.time_until_switch = rand.randint(3, 8)
        self.val = rand.choice(closed_vals)
        self.http = HTTP(self.url)
    def gen_sonar(self,val):
        return rand.uniform(val-(self.spread/2), val+(self.spread/2))
    
    def gen_time(self):
        t = datetime.now().time()
        return (t.hour * 3600 + t.minute * 60 + t.second) * 1000000 + t.microsecond
    
    def flip_state(self):
        self.state = not self.state
        if(self.state):
            self.val = rand.choice(open_vals)
        else: 
            self.val = rand.choice(closed_vals)
        self.time_until_switch = rand.randint(3, 8)

    def gen_post(self):
        data = []
        for i in range(5):
            data.append({"sonar_distance": f"{self.gen_sonar(self.val)}", "time" : f"{self.gen_time()}"})
        self.http.post_data(data)

    def step(self):
        self.time_until_switch-=1

        if(self.time_until_switch == 0):
            self.flip_state()
            self.gen_post()

sim = Simulation(r, "10.203.116.51:5001")
for i in range(100):
    sim.step()
    time.sleep(.001)