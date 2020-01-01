#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <string>
#include <queue>
#include <unordered_map>

using namespace std;
using namespace boost::multiprecision;

struct instruction {
    instruction(int val) {
        opcode = val % 100;
        val /= 100;
        mode1 = val % 10;
        val /= 10;
        mode2 = val % 10;
        val /= 10;
        mode3 = val % 10;
    };
    int opcode;
    int mode1;
    int mode2;
    int mode3;
};

class IntcodeComputer {
public:
    void setCode(vector<cpp_int>& inputCode) {
        code = inputCode;
    }
    void step(queue<cpp_int>& inputs, queue<cpp_int>& outputs) {
        try {
            instruction inst(code[ctr].convert_to<int>());
            switch (inst.opcode) {
            case 1:
                at(ctr + 3, inst.mode3) =
                    get(ctr + 1, inst.mode1) +
                    get(ctr + 2, inst.mode2);
                ctr += 4;
                break;
            case 2:
                at(ctr + 3, inst.mode3) =
                    get(ctr + 1, inst.mode1) *
                    get(ctr + 2, inst.mode2);
                ctr += 4;
                break;
            case 3:
                at(ctr + 1, inst.mode1) = inputs.front();
                inputs.pop();
                ctr += 2;
                break;
            case 4:
                outputs.push(get(ctr + 1, inst.mode1));
                ctr += 2;
                break;
            case 5:
                if (get(ctr + 1, inst.mode1))
                    ctr = get(ctr + 2, inst.mode2).convert_to<int>();
                else
                    ctr += 3;
                break;
            case 6:
                if (!get(ctr + 1, inst.mode1))
                    ctr = get(ctr + 2, inst.mode2).convert_to<int>();
                else
                    ctr += 3;
                break;
            case 7:
                at(ctr + 3, inst.mode3) =
                    get(ctr + 1, inst.mode1) <
                    get(ctr + 2, inst.mode2);
                ctr += 4;
                break;
            case 8:
                at(ctr + 3, inst.mode3) =
                    get(ctr + 1, inst.mode1) ==
                    get(ctr + 2, inst.mode2);
                ctr += 4;
                break;
            case 9:
                relBase += get(ctr + 1, inst.mode1).convert_to<int>();
                ctr += 2;
                break;
            case 99:
                bHalted = true;
                break;
            default:
                throw invalid_argument("Hmm");
            }
        } catch (const out_of_range & orange) {
            
        } catch (const invalid_argument & iarg) {
            bHalted = true;
        }
    }
    bool isHalted() {
        return bHalted;
    }
private:
    cpp_int& at(int index, int mode) {
        if (index >= code.size())
            code.resize(index + 1, cpp_int(0));
        cpp_int value = code[index];
        int idx;
        switch (mode) {
        case 0:
            idx = value.convert_to<int>();
            if (idx >= code.size())
                code.resize(idx + 1, cpp_int(0));
            return code[idx];
        case 2:
            idx = relBase + value.convert_to<int>();
            if (idx >= code.size())
                code.resize(idx + 1, cpp_int(0));
            return code[idx];
        case 1:
        default:
            throw invalid_argument("hmm");
        }
        throw invalid_argument("hmm");
    }
    cpp_int get(int index, int mode) {
        if (index >= code.size())
            code.resize(index + 1, cpp_int(0));
        cpp_int value = code[index];
        int idx;
        switch (mode) {
        case 0:
            idx = value.convert_to<int>();
            if (idx >= code.size())
                code.resize(idx + 1, cpp_int(0));
            return code[idx];
        case 1:
            return value;
        case 2:
            idx = relBase + value.convert_to<int>();
            if (idx >= code.size())
                code.resize(idx + 1, cpp_int(0));
            return code[idx];
        default:
            throw invalid_argument("hmm");
        }
        throw invalid_argument("hmm");
    }

    int ctr = 0;
    int relBase = 0;
    vector<cpp_int> code;
    bool bHalted = false;
};


int main() {
    ifstream inputStream("p1");
    string inputString((istreambuf_iterator<char>(inputStream)),
        istreambuf_iterator<char>());

    vector<string> tokens;
    boost::split(tokens, inputString, boost::is_any_of(","));

    vector<cpp_int> codes;
    for (string& token : tokens) {
        codes.push_back(cpp_int(token));
    }

    IntcodeComputer computer;
    computer.setCode(codes);

    queue<cpp_int> input;
    queue<cpp_int> output;
	unordered_map<pair<int, int>, int, boost::hash<pair<int, int>>> board;
	int maxx = -999;
	int maxy = -999;
	int minx = 999;
	int miny = 999;
	int count = 0;
	while (!computer.isHalted()) {
		computer.step(input, output);
	}

	while (!output.empty()) {
		int x = output.front().convert_to<int>();
		output.pop();
		int y = output.front().convert_to<int>();
		output.pop();
		int tile = output.front().convert_to<int>();
		output.pop();
		if (board[{x, y}] == 2)
			count--;
		board[{x, y}] = tile;
		if (tile == 2)
			count++;
		maxx = max(x, maxx);
		maxy = max(y, maxy);
		minx = min(x, minx);
		miny = min(y, miny);
	}
	cout << count << endl;
	cout << maxx << " " << maxy << endl;
	cout << minx << " " << miny << endl;
}