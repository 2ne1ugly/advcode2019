#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>

using namespace std;

int traverse(unordered_map<string, set<string>>& links, const string& node) {
    if (node == "YOU" || node == "SAN")
        return 1;
    if (links.count(node) == 0)
        return 0;
    bool found = false;
    int foundValue = 0;
    for (const string& to : links[node]) {
        int ret = traverse(links, to);
        if (ret > 0) {
            if (found) {
                throw foundValue + ret;
            }
            found = true;
            foundValue = ret;
        }
    }
    return found ? foundValue + 1 : 0;
}

int main () {
    ifstream inputStream("p2");
    string input((istreambuf_iterator<char>(inputStream)),
                istreambuf_iterator<char>());

    vector<string> tokens;
    boost::split(tokens, input, boost::is_any_of("\n"));

    unordered_map<string, set<string>> links;
    unordered_set<string> roots;
    unordered_set<string> notRoots;

    for (string& token : tokens) {
        vector<string> link;
        boost::split(link, token, boost::is_any_of(")"));
        links[link[0]].insert(link[1]);
        if (!notRoots.count(link[0]))
            roots.insert(link[0]);
        if (roots.count(link[1])) 
            roots.erase(link[1]);
        notRoots.insert(link[1]);
    }

    string root = *roots.begin();
    try {
        traverse(links, root);
    } catch (const int& output) {
        cout << output - 2 << endl;
    }
}