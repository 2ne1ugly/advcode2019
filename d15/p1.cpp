#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <string>
#include <queue>
#include <unordered_set>
#include <curses.h>

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
    cpp_int proceed(cpp_int input) {
        try {
			while (true) {
				instruction inst(code[ctr].convert_to<int>());
				cpp_int output;
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
					at(ctr + 1, inst.mode1) = input;
					ctr += 2;
					break;
				case 4:
					output = get(ctr + 1, inst.mode1);
					ctr += 2;
					return output;
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
			}
		} catch (const out_of_range & orange) {
			bHalted = true;				
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

	initscr();
	cbreak();
	noecho();
	clear();
	curs_set(0);

    IntcodeComputer computer;
    computer.setCode(codes);

    queue<cpp_int> input;
    queue<cpp_int> output;

	queue<tuple<IntcodeComputer, pair<int, int>, int>> currentQueue;
	queue<tuple<IntcodeComputer, pair<int, int>, int>> nextQueue;
	unordered_set<pair<int, int>, boost::hash<pair<int,int>>> visited;

	currentQueue.push({computer, {0, 0}, 1});
	int depth = 0;
	int found = 0;
	while(found == 0) {
		mvprintw(0, 0, "%d", depth);
		while (!currentQueue.empty()) {
			tuple<IntcodeComputer, pair<int, int>, int> data = currentQueue.front();
			currentQueue.pop();
			pair<int, int> pos = get<1>(data);
			if (visited.count(pos))
				continue;
			visited.insert(pos);
			mvprintw(1, 0, "%5d %5d", pos.first, pos.second);
			int currentRet = get<2>(data);
			const int offset = 30;
			switch (currentRet) {
			case 0:
				mvaddch(pos.second + offset, pos.first + offset * 3, '#');
				break;
			case 1:
				mvaddch(pos.second + offset, pos.first + offset * 3, '.');
				break;
			case 2:
				mvaddch(pos.second + offset, pos.first + offset * 3, 'O');
				break;
			}
			refresh();

			if (currentRet == 0)
				continue;
			if (currentRet == 2)
			{
				found = depth;
			}


			IntcodeComputer north = get<0>(data);
			IntcodeComputer south = get<0>(data);
			IntcodeComputer west = get<0>(data);
			IntcodeComputer east = get<0>(data);

			cpp_int ret;
			ret = north.proceed(1);
			nextQueue.push({north, {pos.first, pos.second + 1}, ret.convert_to<int>()});
			ret = south.proceed(2);
			nextQueue.push({south, {pos.first, pos.second - 1}, ret.convert_to<int>()});
			ret = west.proceed(3);
			nextQueue.push({west, {pos.first - 1, pos.second}, ret.convert_to<int>()});
			ret = east.proceed(4);
			nextQueue.push({east, {pos.first + 1, pos.second}, ret.convert_to<int>()});
		}
		currentQueue.swap(nextQueue);
		depth++;
	}
	mvprintw(2, 0, "%5d %5d", found);
	getch();
	endwin();
}