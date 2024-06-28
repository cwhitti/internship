import sqlite3
from dbClass import DataBase

def main():
    
    # define variables
    db = DataBase()
    # create a table
    query = '''
    CREATE TABLE IF NOT EXISTS users (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        username TEXT NOT NULL,
        email TEXT NOT NULL UNIQUE,
        password TEXT NOT NULL,
        admin INTEGER NOT NULL
    )
    '''
    db.insert(query)
    
    query = '''
    INSERT INTO users (username, email, password, admin) 
    VALUES ('john_doe', 'john@example.com', 'password123', 0)
    '''
    db.insert(query=query)
    
    query = '''
    INSERT INTO users (username, email, password, admin) 
    VALUES ('admin', 'john@example.com', 'adminpasscode1243', 1)
    '''
    db.insert(query=query)

    # Demo menu code
    print("Welcome to the AFRL SQL injection demo.")

    ### ATTACK CODE ###
    username = input("Username: ")

    if ( db.is_admin(username) ):
        print("Logged in as admin.")
    else:
        print("User is not an admin.")

main()
