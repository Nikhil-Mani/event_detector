import random as rand
from datetime import datetime
import http.client
import json
import time 
# by parsing data, I found that the range between the highest and lowest values was 2
# this code works based off that assumption 

closed_vals = [12, 30] # these are the values it should roughly take on when closed, depending on position
open_vals = [63, 122]
spr = 2
class HTTP:
    def __init__(self, url: str):
        self.url = url
        self.conn = http.client.HTTPConnection(url)
        self.headers  = {"Content-Type": "application/json"}
    
    def post_data(self, data: list[dict]):
        self.conn.request("POST", "/post", json.dumps(data), self.headers)
        response = self.conn.getresponse()
        return (response.status, response.reason)

class Simulation:

    def __init__(self, spread, url, single=-1, delay=0.01):
        self.spread = spread
        self.state = False # True is open, closed is False
        self.url = url
        self.delay = delay
        self.time_until_switch = round(rand.uniform(1, 2) * (1/self.delay))
        self.val = rand.choice(closed_vals)
        self.http = HTTP(self.url)
        self.single = single # if we are using a single val from the vals array or not
        
    def gen_sonar(self, val):
        return rand.uniform(val-(self.spread/2), val+(self.spread/2))
    
    def gen_time(self):
        t = datetime.now().time()
        return (t.hour * 3600 + t.minute * 60 + t.second) * 1000000 + t.microsecond
    
    def flip_state(self):
        self.state = not self.state
        if self.single >= 0:
            if(self.state):
                self.val = open_vals[self.single]
            else: 
                self.val = closed_vals[self.single]
        else:
            if(self.state):
                self.val = rand.choice(open_vals)
            else: 
                self.val = rand.choice(closed_vals)
        self.time_until_switch = round(rand.uniform(1,2) * (1/self.delay))

    def gen_post(self):
        data = []
        for i in range(5):
            data.append({"sonar_distance": f"{self.gen_sonar(self.val)}", "time" : f"{self.gen_time()}"})
        #self.http.post_data(data)
        return data

    def step(self):
        self.time_until_switch-=1
        x=self.gen_post()
        #print(self.time_until_switch)
        if(self.time_until_switch == 0):
            self.flip_state()
            self.gen_post()
        return x

# sim = Simulation(spr, "10.203.116.51:5001", 0)

# for i in range(100):
#     sim.step()
#     time.sleep(.001)

