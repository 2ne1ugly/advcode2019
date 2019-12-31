#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <string>
#include <array>

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
    switch (mode) {
    case 0:
        return codes[value];
    case 1:
    default:
        throw invalid_argument("hmm");
    }
    throw invalid_argument("hmm");
}

int getValue(vector<int>& codes, int value, int mode) {
    switch (mode) {
    case 0:
        return codes[value];
    case 1:
        return value;
    default:
        throw invalid_argument("hmm");
    }
    throw invalid_argument("hmm");
}

int process(vector<int>& codes, vector<int> inputs) {
    try {
        int output = 0;
        int i = 0;
        int inputIndex = 0;
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
                    inputs[inputIndex++];
                i += 2;
                break;
            case 4:
                output = getValue(codes, codes[i + 1], inst.mode1);
                i += 2;
                break;
            case 5:
                if (getValue(codes, codes[i + 1], inst.mode1))
                    i = getValue(codes, codes[i + 2], inst.mode2);
                else
                    i += 3;
                break;
            case 6:
                if (!getValue(codes, codes[i + 1], inst.mode1))
                    i = getValue(codes, codes[i + 2], inst.mode2);
                else
                    i += 3;
                break;
            case 7:
                getRefValue(codes, codes[i + 3], inst.mode3) =
                    getValue(codes, codes[i + 1], inst.mode1) <
                    getValue(codes, codes[i + 2], inst.mode2);
                i += 4;
                break;
            case 8:
                getRefValue(codes, codes[i + 3], inst.mode3) =
                    getValue(codes, codes[i + 1], inst.mode1) ==
                    getValue(codes, codes[i + 2], inst.mode2);
                i += 4;
                break;
            case 99:
                return output;
            default:
                throw invalid_argument("Hmm");
            }
        }
    }
    catch (const out_of_range & orange) {
        return numeric_limits<int>::min();
    }
    catch (const invalid_argument & iarg) {
        return numeric_limits<int>::min();
    }
    return numeric_limits<int>::min();
}

int main() {
    ifstream inputStream("p1");
    string inputString((istreambuf_iterator<char>(inputStream)),
        istreambuf_iterator<char>());

    vector<string> tokens;
    boost::split(tokens, inputString, boost::is_any_of(","));

    vector<int> codes;
    for (string& token : tokens) {
        codes.push_back(stoi(token));
    }

    int maxThrust = numeric_limits<int>::min();
    array<int, 5> settings = { 0, 1, 2, 3, 4 };
    do {
        int input = 0;
        for (int i = 0; i < 5; i++) {
            vector<int> software = codes;
            input = process(software, {settings[i], input});
        }
        maxThrust = max(maxThrust, input);
    } while (next_permutation(settings.begin(), settings.end()));

    cout << maxThrust << endl;
}