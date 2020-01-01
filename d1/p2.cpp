#include <iostream>
#include <fstream>

using namespace std;

int calc_fuel(int mass) {
    int ret = mass / 3 - 2;
    if (ret <= 0) {
        return 0;
    }
    return calc_fuel(ret) + ret;
}

int main () {
    ifstream inputStream("p2");
    int sum = 0;
    while (!inputStream.eof()) {
        int mass;
        inputStream >> mass;
        sum += calc_fuel(mass);
    }
    cout << sum << endl;
}