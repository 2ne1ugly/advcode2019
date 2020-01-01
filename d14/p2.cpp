#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <string>
#include <unordered_map>
#include <math.h>
using namespace std;

struct Reaction {
	int quant;
	unordered_map<string, int> required;
};

int calcOres(unordered_map<string, Reaction>& reactions, string chemical, int count, unordered_map<string, int>& inventory) {
	if (chemical == "ORE") {
		return count;
	}

	if (count <= inventory[chemical]) {
		inventory[chemical] -= count;
		return 0;
	}
	count -= inventory[chemical];
	inventory[chemical] = 0;

	Reaction& reaction = reactions[chemical];
	int sum = 0;
	int requiredReactions = ceil((float)count / reaction.quant);

	for (auto& kv : reaction.required) {
		sum += calcOres(reactions, kv.first, kv.second * requiredReactions, inventory);
	}
	inventory[chemical] += reaction.quant * requiredReactions;
	inventory[chemical] -= count;
	return sum;
}

int main() {
    ifstream inputStream("p1");
    string inputString((istreambuf_iterator<char>(inputStream)),
        istreambuf_iterator<char>());

	boost::erase_all(inputString, ",");

    vector<string> tokens;
    boost::split(tokens, inputString, boost::is_any_of("\n"));

	unordered_map<string, Reaction> reactions;

	for (string& token : tokens) {
		Reaction reaction;

		vector<string> sides;
		boost::split(sides, token, boost::is_any_of("=>"));
		string& lhs = sides[0];
		string& rhs = sides[2];
		boost::trim(lhs);
		boost::trim(rhs);

		vector<string> rhsTokens;

		boost::split(rhsTokens, rhs, boost::is_any_of(" "));
		reaction.quant = stoi(rhsTokens[0]);

		vector<string> lhsTokens;
		boost::split(lhsTokens, lhs, boost::is_any_of(" "));
		for (int i = 0; i < lhsTokens.size(); i += 2)
			reaction.required[lhsTokens[i + 1]] = stoi(lhsTokens[i]);

		reactions[rhsTokens[1]] = reaction;
	}

	unordered_map<string, int> inventory;

	// cout << calcOres(reactions, "FUEL", 1, inventory) << endl;
	long long ores = 1000000000000;
	long long prev = ores;
	int fuels = 0;
	while (ores >= 0) {
		prev = ores;
		ores -= calcOres(reactions, "FUEL", 10000, inventory);
		fuels += 10000;
		if (fuels % 10000 == 0)
			cout << ores << endl;
	}
	fuels -= 10000;
	ores = prev;
	while (ores >= 0) {
		ores -= calcOres(reactions, "FUEL", 1, inventory);
		fuels++;
	}
	fuels--;
	cout << fuels << endl;
}