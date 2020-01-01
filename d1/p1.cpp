#include <iostream>
#include <fstream>

using namespace std;

int main () {
    ifstream inputStream("p1");
    int sum = 0;
    while (!inputStream.eof()) {
        int mass;
        inputStream >> mass;
        sum += mass / 3 - 2;
    }
    cout << sum << endl;
}