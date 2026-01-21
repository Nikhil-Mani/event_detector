import statistics

val_constant = 0.1
avg_constant = 0.9
window_size = 10
class Door:
    def __init__(self):
        self.low_boundary = 50
        self.high_boundary = 50
        self.window = []
        self.initialized = False

    def add_data(self, data):
        print(data)
        self.window+=data
        #print("--window data here--")
        #print(self.window)
        avg = statistics.mean(self.window)
        std = statistics.stdev(self.window, avg)

        if not self.initialized and len(self.window) >= window_size:
            self.low_boundary = avg - std
            self.high_boundary = avg + std
            self.window = self.window[:window_size]
            self.initialized = True
            print(f"Initializing: low boundary = {self.low_boundary}, high boundary = {self.high_boundary}")
        if len(self.window)>=window_size:
            self.window = self.window[:window_size]
        print(f"low boundary: {self.low_boundary}, high boundary: {self.high_boundary}")
        print(avg)

        if(avg < (self.low_boundary)):
            self.low_boundary = val_constant * self.low_boundary + avg_constant * avg
            self.state = True
            print('bye')
        if(avg > (self.high_boundary)):
            print('hi')
            self.high_boundary = val_constant * self.high_boundary + avg_constant * avg
            self.state = False # Open
        # else:
        #     self.low_boundary = self.low_boundary * val_constant + avg_constant * avg
        #     self.high_boundary = self.high_boundary * val_constant + avg_constant * avg
        
    """
    if the lower boundary is lower than the lower end of the standard deviation:
        - means the new average is probably the higher boundary
    if the lowe rboundary is higher than the higher end:
        - means that its closer to high boundary
    """
    def get_state(self):
        return self.state