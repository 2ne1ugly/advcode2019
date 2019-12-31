#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <string>

using namespace std;

void print_arr(vector<int>& arr) {
    cout << "--\n";
    for (int val : arr) {
        cout << val << ",";
    }
    cout << endl;
}

int main () {
    ifstream inputStream("p1");
    string input((istreambuf_iterator<char>(inputStream)),
                istreambuf_iterator<char>());

    vector<string> tokens;
    boost::split(tokens, input, boost::is_any_of(","));

    vector<int> codes;
    for (string& token : tokens) {
        codes.push_back(stoi(token));
    }

    for (int i = 0; codes[i] != 99; i += 4) {
        switch (codes[i]) {
        case 1:
            codes[codes[i + 3]] = codes[codes[i + 1]] + codes[codes[i + 2]];
            break;
        case 2:
            codes[codes[i + 3]] = codes[codes[i + 1]] * codes[codes[i + 2]];
            break;
        default:
            break;
        }
    }
    cout << codes[0] << endl;
}