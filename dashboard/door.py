import statistics
val_constant = 0.8
avg_constant = 0.2
class Door:
    def __init__(self):
        self.low_boundary = 1000
        self.high_boundary = 0
        self.window = []
        self.initialized = False

    def add_data(self, data):
        self.window+=data
        avg = statistics.mean(self.window)
        std = statistics.stdev(self.window, avg)
        if not self.initialized and len(self.window) >= 30:
            self.low_boundary = avg - std
            self.high_boundary = avg + std
            self.initialized = True

        if(avg < (self.low_boundary - std/3)):
            self.low_boundary = val_constant * self.low_boundary + avg_constant * avg
            self.state = True
        elif(avg > (self.high_boundary + std/3)):
            self.high_boundary = val_constant * self.high_boundary + avg_constant * avg
            self.state = False # Open
        else:
            self.low_boundary = self.low_boundary * val_constant + avg_constant * avg
            self.high_boundary = self.high_boundary * val_constant + avg_constant * avg
        return self.state
    """
    if the lower boundary is lower than the lower end of the standard deviation:
        - means the new average is probably the higher boundary
    if the lowe rboundary is higher than the higher end:
        - means that its closer to high boundary
    """
       