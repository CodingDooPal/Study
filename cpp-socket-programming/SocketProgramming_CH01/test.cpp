#include <iostream>
#include <string>

using namespace std;

const string expected_password = "boogeyman";

int main(void)
{
    string password;

    cout << "Enter password: " << endl;
    cin >> password;

    if (password == expected_password)
        cout << "Success." << endl;
    else
        cout << "Failure." << endl;

    return 0;
}
