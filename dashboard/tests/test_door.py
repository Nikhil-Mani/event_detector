import sys
sys.path.insert(0, '..')
from sim import Simulation, HTTP
from door import Door
import pytest
import time
# look up pytest fixtures

closed_val = 12
open_val = 60
spr = 2

sim = Simulation(spr, "10.203.116.51:5001", 0)
door = Door()
for i in range(400):
    d = sim.step()
    door.add_data([float(x['sonar_distance']) for x in d])
    time.sleep(.001)