#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>

using namespace std;

int traverse(unordered_map<string, set<string>>& links, int depth, const string& node) {
    if (links.count(node) == 0) {
        return depth;
    }
    int sum = depth;
    for (const string& to : links[node]) {
        sum += traverse(links, depth + 1, to);
    }
    return sum;
}

int main () {
    ifstream inputStream("p1");
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
    cout << traverse(links, 0, root) << endl;
}