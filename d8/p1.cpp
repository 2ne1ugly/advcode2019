#include <iostream>
#include <fstream>
#include <array>
#include <unordered_map>

using namespace std;

int main() {
    ifstream inputStream("p1");
    string inputString((istreambuf_iterator<char>(inputStream)),
        istreambuf_iterator<char>());

    const int width = 25;
    const int height = 6;

    unordered_map<int, int> minOccurance;
    minOccurance[0] = 9999;
    unordered_map<int, int> currentOccurance;

    int i = 0;
    for (char c : inputString) {
        int value = c - '0';
        currentOccurance[value]++;
        i++;
        if (i >= height * width)
        {
            if (minOccurance[0] > currentOccurance[0])
                minOccurance = currentOccurance;
            currentOccurance.clear();
            i = 0;
        }
    }
    cout << minOccurance[1] * minOccurance[2] << endl;
}