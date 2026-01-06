import sqlite3
from typing import TypedDict
from datetime import date, time, datetime


class Entry(TypedDict):
    sonar_distance: float
    time: int

# Initializes database with sonar table
def init_db() -> None:
    con = sqlite3.connect("data/sonar.db")
    cur = con.cursor()
    # Checks if database already exists
    try:
        cur.execute("CREATE TABLE sonar(time, distance)")
    except:
        print("sonar table already exists")
    con.close()

def format_time(data)-> list[datetime]:
    td = date.today()
    datetimes = []
    for entry in data:
        seconds = entry['time'] // 1000000
        ms = entry['time'] % 1000000
        min = seconds // 60
        hour = min // 60
        min = seconds % 60
        seconds = seconds % 1000000 
        time = datetime.time(hour=hour, minute=min, second=seconds, microseconds = ms)
        # divide by a mil and thats the amt of seconds (like the whole number division)
        # remainder or modulus of a mil is micro seconds
        # same thought process for hours/minutes
        datetimes.append(datetime.combine(td, time))
    return datetimes


def add_data(data: list[Entry])->None:
    con = sqlite3.connect("data/sonar.db")
    cur = con.cursor()
    for entry in data:
        cur.execute("INSERT INTO sonar VALUES(?,?)",(entry["time"], entry["sonar_distance"]))
    con.commit()
    con.close()

def read_data():
    con = sqlite3.connect("data/sonar.db")
    cur = con.cursor()
    for row in cur.execute("SELECT time, distance FROM sonar"):
        print(row)

def handle_data(data):
    #init_db()
    add_data(data)

read_data()

# have to delete all of 1767716333 and 1767716433