#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/rational.hpp>
#include <set>

using namespace std;

int main() {
    ifstream inputStream("p1");
    string inputString((istreambuf_iterator<char>(inputStream)),
        istreambuf_iterator<char>());

    vector<string> lines;
    boost::split(lines, inputString, boost::is_any_of("\n"));
    int width = lines.size();
    int height = lines[0].length();

    int numAst = 0;
    for (char c : inputString) {
        if (c == '#')
            numAst++;
    }

    int minMiss = 9999999;
    set<pair<int, int>> occ;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (lines[i][j] == '#') {
                occ.clear();
                int currMiss = 1;
                for (int k = 0; k < height; k++) {
                    for (int l = 0; l < width; l++) {
                        if (!(k == i && l == j) && lines[k][l] == '#') {
                            int y = i - k;
                            int x = j - l;
                            int gcd = boost::gcd(y, x);
                            pair<int, int> val = make_pair(y / gcd, x / gcd);
                            if (occ.count(val))
                                currMiss++;
                            else
                                occ.insert(val);
                        }
                        if (minMiss <= currMiss)
                            break;
                    }
                    if (minMiss <= currMiss)
                        break;  
                }
                if (minMiss > currMiss)
                    cout << i << " " << j << " " << numAst - currMiss <<  endl;
                minMiss = min(minMiss, currMiss);
            }
        }
    }
    cout << numAst - minMiss << endl;
}