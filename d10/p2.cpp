#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/rational.hpp>
#include <map>
#include <set>
#include <math.h>

using namespace std;

const int x = 23;
const int y = 19;


struct orbit {
    bool operator()(const pair<int, int>& lhs, const pair<int, int>& rhs) const {
        return atan2(lhs.first, lhs.second) > atan2(rhs.first, rhs.second);
    }
};

struct closer {
    bool operator()(const pair<int, int>& lhs, const pair<int, int>& rhs) const {
        return abs(lhs.first) + abs(lhs.second) < abs(rhs.first) + abs(rhs.second);
    }
};

int main() {
    ifstream inputStream("p1");
    string inputString((istreambuf_iterator<char>(inputStream)),
        istreambuf_iterator<char>());

    vector<string> lines;
    boost::split(lines, inputString, boost::is_any_of("\n"));
    int width = lines[0].length();
    int height = lines.size();
    

    map<pair<int, int>, set<pair<int, int>, closer>, orbit> occ;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!(x == j && y == i) && lines[i][j] == '#') {
                int dx = j - x;
                int dy = i - y;
                int gcd = boost::gcd(dx, dy);
                pair<int, int> val = make_pair(dx / gcd, dy / gcd);
                occ[val].insert(make_pair(dx, dy));
            }
        }
    }

    int i = 0;
    while (true) {
        for (auto& pair : occ) {
            if (!pair.second.empty())
            {
                cout << x + pair.second.begin()->first << " " << y + pair.second.begin()->second << endl;
                if (i == 199)
                {
                    cout << (x + pair.second.begin()->first) * 100 + y + pair.second.begin()->second << endl;
                    return 0;
                }
                pair.second.erase(pair.second.begin());
                i++;
            }
        }
    }
}