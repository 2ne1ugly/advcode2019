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

	unordered_map<pair<int, int>, int, boost::hash<pair<int, int>>> board;
    queue<cpp_int> input;
    queue<cpp_int> output;
	pair<int, int> position = make_pair<int, int>(0, 0);
	pair<int, int> direction = make_pair<int, int>(0, 1);
	pair<int, int> minimum;
	pair<int, int> maximum;
	board[position] = 1;
	input.push(1);
    while (!computer.isHalted()) {
        computer.step(input, output);
		while (output.size() >= 2) {
			int colorOut = output.front().convert_to<int>();
			output.pop();
			switch (colorOut) {
			case 0:
				board[position] = 0;
				break;
			case 1:
				board[position] = 1;
				break;
			}
			int dirOut = output.front().convert_to<int>();
			output.pop();
			int x = direction.first;
			int y = direction.second;
			switch (dirOut) {
			case 0:
				direction.first = -y;
				direction.second = x;
				break;
			case 1:
				direction.first = y;
				direction.second = -x;
				break;
			}
			position.first += direction.first;
			position.second += direction.second;
			input.push(board.count(position) ? board[position] : 0);
			minimum.first = min(minimum.first, position.first);
			minimum.second = min(minimum.second, position.second);
			maximum.first = max(maximum.first, position.first);
			maximum.second = max(maximum.second, position.second);
		}
    }

	for (int i = maximum.second; i >= minimum.second; i--) {
		for (int j = minimum.first; j <= maximum.first; j++) {
			switch(board[pair<int, int>(j, i)]) {
			case 0:
				cout << ".";
				break;
			case 1:
				cout << "#";
				break;
			}
		}
		cout << endl;
	}
}