#include <iostream>
#include <sstream>
#include <string>
#include "TEA.h"

using namespace std;

int main()
{
    cout << "Enter key: ";
    string key;
    getline (cin, key);

    cout << "Enter string: ";
    string str;
    getline (cin, str);

    cout << "Encrypted string: ";

    istringstream iss{ str };
    stringstream ss;
    encipher (iss, ss, key);
    cout << ss.str() << endl;

    cout << "Decrypted string: ";
    decipher (ss, cout , key);
    cout << endl;


    return 0;
}
