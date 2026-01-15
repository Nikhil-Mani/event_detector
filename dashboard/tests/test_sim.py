from sim import Simulation, HTTP
# look up pytest fixtures

def test_gen_data():
    sim = Simulation(2, "http://httpbin.org/")
    
def test_http():
    http = HTTP("http://httpbin.org/")