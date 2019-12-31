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

    array<array<char, width>, height> img;
    {
        array<char, width> line;
        line.fill('.');
        img.fill(line);
    }
    unordered_map<int, int> currentOccurance;
    //0 is '#'
    //1 is ' '
    //2 is '.'
    int i = 0;
    for (char c : inputString) {
        if (img[i / width][i % width] == '.') {
            int value = c - '0';
            char color = 'm';
            switch (value) {
            case 0:
                color = '#';
                break;
            case 1:
                color = ' ';
                break;
            case 2:
                color = '.';
                break;
            }
            img[i / width][i % width] = color;
        }
        i++;
        if (i >= height * width)
        {
            i = 0;
        }
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << img[i][j];
        }
        cout << endl;
    }
}