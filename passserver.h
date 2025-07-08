/* Name: Arturo Calanche
 * Email: aac21p@fsu.edu
 * Class: COP4530-0001.fa24
All work was performed by Arturo Calanche */

#ifndef C___PROGRAMMING_PASSSERVER_H
#define C___PROGRAMMING_PASSSERVER_H

#include "hashtable.h"

using namespace std;
using namespace cop4530;

class PassServer {
private:
    HashTable<string, string> passTable;
    string encrypt(const string & str); //encrypt the parameter str and return the encrypted string

public:
    PassServer(size_t size = 101); //constructor
    ~PassServer(); //destructor
    bool load(const char *filename); //load a password file into the HashTable object
    bool addUser(pair<string, string> & kv); //add a new username and password
    bool addUser(pair<string, string> && kv); //move version of addUser.
    bool removeUser(const string & k); //delete an existing user with username k.
    bool changePassword(const pair<string, string> &p, const string & newpassword);
    //change an existing user's password
    bool find(const string & user) const; //check if user is in the hash table.
    void dump(); //show the structure and contents of the HashTable object to the screen
    size_t size() const; //return the number of username/password pairs in the table
    bool write_to_file(const char *filename) const; //save username and password combination into a file
};


#endif //C___PROGRAMMING_PASSSERVER_H
