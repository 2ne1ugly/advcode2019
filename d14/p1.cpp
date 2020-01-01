#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <string>
#include <unordered_map>

using namespace std;

struct Reaction {
	int quant;
	unordered_map<string, int> required;
};

int calcOres(unordered_map<string, Reaction>& reactions, string chemical, unordered_map<string, int>& inventory) {
	if (chemical == "ORE")
	{
		inventory["ORE"]++;
		return 1;
	}
	Reaction& reaction = reactions[chemical];
	int sum = 0;
	for (auto& kv : reaction.required) {
		while (inventory[kv.first] < kv.second)
			sum += calcOres(reactions, kv.first, inventory);
		inventory[kv.first] -= kv.second;
	}
	inventory[chemical] += reaction.quant;
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
	cout << calcOres(reactions, "FUEL", inventory) << endl;
}