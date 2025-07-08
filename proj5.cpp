/* Name: Arturo Calanche
 * Email: aac21p@fsu.edu
 * Class: COP4530-0001.fa24
All work was performed by Arturo Calanche */

#include <iostream>
#include <string>
#include <limits>
#include "passserver.h"

using namespace std;

void Menu()
{
    cout << "\n\n";
    cout << "l - Load From File" << endl;
    cout << "a - Add User" << endl;
    cout << "r - Remove User" << endl;
    cout << "c - Change User Password" << endl;
    cout << "f - Find User" << endl;
    cout << "d - Dump HashTable" << endl;
    cout << "s - HashTable Size" << endl;
    cout << "w - Write to Password File" << endl;
    cout << "x - Exit program" << endl;
    cout << "\nEnter choice : ";
}

int main() {
    size_t s;
    char c;
    string username, password, newpassword, file;
    //prompt the user until a valid size is entered
    while (true) {
        cout << "Enter preferred hash table capacity (integer): ";
        if (cin >> s) {
            break;
        } else { //if user does not enter a number
            //clear error flags and input buffer
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear input buffer

    PassServer ps(s); //create hash table, size = largest prime under s (HashTable class)

    do { //start program loop until 'x' is inputted
        Menu(); //print menu
        cin.get(c); //get choice character from input
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear input buffer
        cout << "choice: " << c << endl;

        switch (c)
        {
            case 'l': //load from file
                //prompt for filename
                cout << "Enter password file name to load from: ";
                getline(cin, file);

                //write file content into hash table
                if (!ps.load(file.c_str())) { //error if file can't be opened
                cout << "Error: Cannot open file " << file << endl;
                }
                break;

            case 'a': //add a new user
                //prompt for username and password
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);

                //add user if user doesn't exist in the table
                if (ps.addUser(make_pair(username, password))) {
                    cout << "User " << username << " added." << endl;
                } else {
                    cout << "*****Error: User already exists. Could not add user." << endl;
                }
                break;

            case 'r': //remove a user
                //prompt for username
                cout << "Enter username: ";
                getline(cin, username);

                //remove user if user exists in the table
                if (ps.removeUser(username)) {
                    cout << "User " << username << " deleted." << endl;
                } else {
                    cout << "*****Error: User not found.  Could not delete user." << endl;
                }
                break;

            case 'c': //change a user's password
                //prompt for username, current password
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                cout << "Enter new password: ";
                getline(cin, newpassword);

                //change password if username and current password are correct
                if (ps.changePassword(make_pair(username, password), newpassword)) {
                    cout << "Password changed for user " << username << endl;
                } else {
                    cout << "*****Error: Could not change user password." << endl;
                }
                break;

            case 'f': //find a user
                //prompt for username
                cout << "Enter username: ";
                getline(cin, username);

                //state if username exists or not
                if (ps.find(username)) {
                    cout << "User '" << username << "' found." << endl;
                } else {
                    cout << "User '" << username << "' not found." << endl;
                }
                break;

            case 'd': //print the hash table
                ps.dump();
                break;

            case 's': //print the current hash table size
                cout << "Size of hashtable: " << ps.size() << endl;
                break;

            case 'w': //write the hash table to a file
                cout << "Enter password file name to write to: ";
                getline(cin, file);

                //open a file and write content into it
                if (!ps.write_to_file(file.c_str())) { //error if file can't be opened
                    cout << "Error: Cannot open file to write." << endl;
                }
                break;

            case 'x': //exit program
                break;

            default: //any other input is invalid
                cout << "*****Error: Invalid entry.  Try again." << endl;
        }
    } while (c != 'x');
}
