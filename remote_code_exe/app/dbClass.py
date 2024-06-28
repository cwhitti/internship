import sqlite3

DB_NAME = "database.db"

class DataBase:

    '''
    PUBLIC FUNCTIONS
    '''
    # Not implemented yet
    def create_db( self, name, columns ):

        # PASS
        pass

    def insert( self, query=None, query_list=None ):

        # initialize variables
        insert_success=False

        # Check for bad input
        if query == None and query_list == None:

            print("""(!) NO insertion data provided
                    \t\tUse: insert( self, query=None, query_list=None )
                    """)
            # Fail out early
            return False

        # Check that connection is successful
        if self._connect_db():
            
            # List was indeed provided
            if query_list != None:

                # insert everything
                for q in query_list:

                    # Call insert method
                    insert_success = self._insert( q )

                    # Die if something failed
                    if not insert_success:
                        break
            
            # Insert one query
            if query != None:

                # Call insert method
                insert_success = self._insert( query )

            # Commit
            self._commit_db()

            # After everything, close db
            self._close_db()
        
        # Something failed
        return insert_success

    def is_admin(self, username):

        return self._is_admin(username)
    
    '''
    PRIVATE FUNCTIONS
    '''
    def _close_db( self ):
        
        if self._conn != None:

            self._conn.close()
            print(f"(!) Closed connection to '{self.db_name}'.")

        else:
            print(f"(!) '{self.db_name}' not currently open.")

    def _commit_db( self ): # Simple command: commit to db
        self._conn.commit()
        
    def _connect_db( self ): # Simple command: Connect to to db

        self._conn = sqlite3.connect( self.db_name )

        if self._conn != None:
            self._cursor = self._conn.cursor()
            print(f"(!) Opened connection to '{self.db_name}'.")
            return True
        
        self._cursor = None
        print(f"(!) Connection with '{self.db_name}' failed.")
        return False

    def _insert( self, query ): # Simple command: insert to db

        # execute query
        try:
            self._conn.execute( query )
        
        # catches integrity error
        except sqlite3.IntegrityError as e:
            print(f"Integrity error: {e}")
            return False
        
        # catches operational
        except sqlite3.OperationalError as e:
            print(f"Operational error: {e}")
            return False
        
        # catches database error
        except sqlite3.DatabaseError as e:
            print(f"Database error: {e}")
            return False
        
        # Catches any other unexpected errors
        except Exception as e:
            print(f"An unexpected error occurred: {e}")
            return False
        
        return True

    # BAD EXAMPLE. DON'T DO THIS!
    def _is_admin(self, username):

        if self._connect_db():

            self._cursor.execute("""
                    SELECT
                        admin
                    FROM
                        users
                    WHERE
                        username = '%s'
                """ % username)
            
            result = self._cursor.fetchone()

            self._close_db()

            if result is None:
                # User does not exist
                return False

        admin, = result

        return admin
    # INITIALIZE DB
    def __init__( self ): 

        # initialize variables
        self.db_name = DB_NAME
        self._conn = None
        self._cursor = None

        # initialize database
        self._connect_db()
        self._close_db()