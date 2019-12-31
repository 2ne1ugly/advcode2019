#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <string>
#include <array>
#include <queue>

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

class Amplifier {
public:
    Amplifier(vector<int> code) : code(code) {}

    void proceed(queue<int>& input, queue<int>& output) {
        try {
            while (true) {
                instruction inst = parse(code[counter]);
                switch (inst.opcode) {
                case 1:
                    getRefValue(code, code[counter + 3], inst.mode3) =
                        getValue(code, code[counter + 1], inst.mode1) +
                        getValue(code, code[counter + 2], inst.mode2);
                    counter += 4;
                    break;
                case 2:
                    getRefValue(code, code[counter + 3], inst.mode3) =
                        getValue(code, code[counter + 1], inst.mode1) *
                        getValue(code, code[counter + 2], inst.mode2);
                    counter += 4;
                    break;
                case 3:
                    if (input.empty())
                        return;
                    getRefValue(code, code[counter + 1], inst.mode1) =
                        input.front();
                    input.pop();
                    counter += 2;
                    break;
                case 4:
                    lastOutput = getValue(code, code[counter + 1], inst.mode1);
                    output.push(lastOutput);
                    counter += 2;
                    break;
                case 5:
                    if (getValue(code, code[counter + 1], inst.mode1))
                        counter = getValue(code, code[counter + 2], inst.mode2);
                    else
                        counter += 3;
                    break;
                case 6:
                    if (!getValue(code, code[counter + 1], inst.mode1))
                        counter = getValue(code, code[counter + 2], inst.mode2);
                    else
                        counter += 3;
                    break;
                case 7:
                    getRefValue(code, code[counter + 3], inst.mode3) =
                        getValue(code, code[counter + 1], inst.mode1) <
                        getValue(code, code[counter + 2], inst.mode2);
                    counter += 4;
                    break;
                case 8:
                    getRefValue(code, code[counter + 3], inst.mode3) =
                        getValue(code, code[counter + 1], inst.mode1) ==
                        getValue(code, code[counter + 2], inst.mode2);
                    counter += 4;
                    break;
                case 99:
                    bHalted = true;
                    return;
                default:
                    throw invalid_argument("Hmm");
                }
            }
        }
        catch (const out_of_range & orange) {
            bHalted = true;
        }
        catch (const invalid_argument & iarg) {
            bHalted = true;
        }
        bHalted = true;
    }

    bool isHalted() {
        return bHalted;
    }

    int getLastOutput() {
        return lastOutput;
    }
private:
    int counter = 0;
    int lastOutput = 0;
    bool bHalted = false;
    vector<int> code;
};

int main() {
    ifstream inputStream("p2");
    string inputString((istreambuf_iterator<char>(inputStream)),
        istreambuf_iterator<char>());

    vector<string> tokens;
    boost::split(tokens, inputString, boost::is_any_of(","));

    vector<int> codes;
    for (string& token : tokens) {
        codes.push_back(stoi(token));
    }

    int maxThrust = numeric_limits<int>::min();
    vector<int> settings = { 5, 6, 7, 8, 9 };

    do {
        vector<Amplifier> amplifiers(5, Amplifier(codes));
        vector<queue<int>> inputs(5);
        for (int i = 0; i < 5; i++) {
            inputs[i].push(settings[i]);
        }
        inputs[0].push(0);
        int i = 0;
        while (!amplifiers[i].isHalted() && !inputs[i].empty()) {
            amplifiers[i].proceed(inputs[i], inputs[(i + 1) % 5]);
            i++;
            i %= 5;
        }
        maxThrust = max(maxThrust, amplifiers[4].getLastOutput());
    } while (next_permutation(settings.begin(), settings.end()));

    cout << maxThrust << endl;
}