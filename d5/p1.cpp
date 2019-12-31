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
    cout << "--\n";
}

struct instruction {
    int opcode;
    int mode1;
    int mode2;
    int mode3;
};

instruction parse(int val) {
    instruction ret;
    ret.opcode = val % 100;
    val /= 100;
    ret.mode1 = val % 10;
    val /= 10;
    ret.mode2 = val % 10;
    val /= 10;
    ret.mode3 = val % 10;
    return ret;
}

int& getRefValue(vector<int>& codes, int value, int mode) {
    switch(mode) {
    case 0:
        return codes[value];
    case 1:
    default:
        throw invalid_argument("hmm");
    }
    throw invalid_argument("hmm");
}

int getValue(vector<int>& codes, int value, int mode) {
    switch(mode) {
    case 0:
        return codes[value];
    case 1:
        return value;
    default:
        throw invalid_argument("hmm");
    }
    throw invalid_argument("hmm");
}

int process(vector<int>& codes, int input) {
    try {
        int output = 0;
        int i = 0;
        while (true) {
            instruction inst = parse(codes[i]);
            switch (inst.opcode) {
            case 1:
                getRefValue(codes, codes[i + 3], inst.mode3) =
                    getValue(codes, codes[i + 1], inst.mode1) +
                    getValue(codes, codes[i + 2], inst.mode2);
                i += 4;
                break;
            case 2:
                getRefValue(codes, codes[i + 3], inst.mode3) =
                    getValue(codes, codes[i + 1], inst.mode1) *
                    getValue(codes, codes[i + 2], inst.mode2);
                i += 4;
                break;
            case 3:
                getRefValue(codes, codes[i + 1], inst.mode1) =
                    input;
                i += 2;
                break;
            case 4:
                output = getValue(codes, codes[i + 1], inst.mode1);
                i += 2;
                break;
            case 99:
                return output;
            default:
                throw invalid_argument("hmm");
            }
        }
    } catch (const out_of_range& orange) {
        return -9999999;
    } catch (const invalid_argument& iarg) {
        return -9999999;
    }
    return -9999999;
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

    cout << process(codes, 1) << endl;
}