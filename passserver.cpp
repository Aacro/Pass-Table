/* Name: Arturo Calanche
 * Email: aac21p@fsu.edu
 * Class: COP4530-0001.fa24
All work was performed by Arturo Calanche */

#include "passserver.h"
#include <cstring>
#include <unistd.h>
#include <crypt.h>

//private interfaces

string PassServer::encrypt(const string &str) {
    char salt[] = "$1$########";
    char* password = new char[100];

    //use crypt() and copy the result to char array
    strcpy(password, crypt(str.c_str(), salt));
    string encryptedStr(password); //convert to string for easier manipulation
    delete[] password; //free memory

    //extract last 22 characters (the final encrypted password)
    return encryptedStr.substr(encryptedStr.length() - 22);
}

//public interfaces
//functions without comments are adapted from HashTable class

PassServer::PassServer(size_t size) : passTable(size) {}

PassServer::~PassServer() = default; //use HashTable's destructor

bool PassServer::load(const char *filename) {
    return passTable.load(filename);
}

bool PassServer::addUser(pair<string, string> &kv) {
    return !passTable.contains(kv.first) && //check if username doesn't exist
    //add user with username as key and encrypted password as value
    passTable.insert(make_pair(kv.first, encrypt(kv.second)));
}

bool PassServer::addUser(pair<string, string> &&kv) {
    return !passTable.contains(kv.first) && //check if username doesn't exist
    //add user with username as key and encrypted password as value
    passTable.insert(make_pair(move(kv.first), encrypt(kv.second)));
}

bool PassServer::removeUser(const string &k) {
    return passTable.remove(k);
}

bool PassServer::changePassword(const pair<string, string> &p, const string &newpassword) {
    //check if username-password pair exists and if new password is not the same as current one
    return passTable.match(make_pair(p.first, encrypt(p.second))) &&
            encrypt(p.second) != encrypt(newpassword) &&
            //then update password (insert function allows it)
            passTable.insert(make_pair(p.first, encrypt(newpassword)));
}

bool PassServer::find(const string &user) const {
    return passTable.contains(user);
}

void PassServer::dump() {
    passTable.dump();
}

size_t PassServer::size() const {
    return passTable.size();
}

bool PassServer::write_to_file(const char *filename) const {
    return passTable.write_to_file(filename);
}