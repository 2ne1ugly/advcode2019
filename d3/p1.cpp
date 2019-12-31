#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>
#include <string>

using namespace std;

int main () {
    ifstream inputStream("p1");
    string input((istreambuf_iterator<char>(inputStream)),
                istreambuf_iterator<char>());

    vector<string> wires;
    boost::split(wires, input, boost::is_any_of("\n"));

    unordered_map<pair<int,int>,set<int>,boost::hash<pair<int, int>>> table;

    int minDist = 99999999;

    int id = 0;

    for (string& wire : wires){
        vector<string> commands;
        boost::split(commands, wire, boost::is_any_of(","));

        pair<int, int> curr = make_pair(0, 0);
        for (string command : commands) {
            char direction = command[0];
            command[0] = '0';
            int amount = stoi(command);
            pair<int, int> delta;
            switch (direction) {
            case 'U':
                delta = make_pair(0, 1);
                break;
            case 'R':
                delta = make_pair(1, 0);
                break;
            case 'D':
                delta = make_pair(0, -1);
                break;
            case 'L':
                delta = make_pair(-1, 0);
                break;
            }

            for (int i = 0; i < amount; i++) {
                curr.first += delta.first;
                curr.second += delta.second;
                table[curr].insert(id);
                if (table[curr].size() >= 2 && curr != make_pair(0, 0)) {
                    minDist = min(minDist, abs(curr.first) + abs(curr.second));
                }
            }
        }
        id++;
    }
    cout << minDist << endl;
}