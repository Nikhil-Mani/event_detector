import sqlite3

# Initializes database with sonar table
def init_db():
    con = sqlite3.connect("data/sonar.db")
    cur = con.cursor()
    try:
        cur.execute("CREATE TABLE sonar(time, distance)")
    except:
        print("sonar table already exists")
# Checks if database already exists
# def add_data()