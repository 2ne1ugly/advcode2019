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

int process(vector<int>& codes) {
    for (int i = 0;; i += 4) {
        if (i >= codes.size())
            return -9999999;
        if (codes[i] == 99)
            break;
        
        if (i >= codes.size() - 3 ||
            codes[i + 1] >= codes.size() ||
            codes[i + 2] >= codes.size() ||
            codes[i + 3] >= codes.size())
            return -9999999;

        switch (codes[i]) {
        case 1:
            codes[codes[i + 3]] = codes[codes[i + 1]] + codes[codes[i + 2]];
            break;
        case 2:
            codes[codes[i + 3]] = codes[codes[i + 1]] * codes[codes[i + 2]];
            break;
        default:
            return -9999999;
        }
    }
    return codes[0];
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

    for (int noun = 0; noun < codes.size(); noun++) {
        for (int verb = 0; verb < codes.size(); verb++) {
            vector<int> test = codes;
            test[1] = noun;
            test[2] = verb;
            int ret = process(test);
            if (ret == 19690720) {
                cout << 100 * noun + verb << endl;
                return 0;
            }
        }
    }
}