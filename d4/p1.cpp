#include <iostream>
#include <vector>

using namespace std;

vector<int> split(int val) {
    vector<int> ret;
    if (val >= 10) {
       ret = split(val / 10);
    }
    ret.push_back(val % 10);
    return ret;
}

int main () {
    int start = 246540;
    int end = 787419;
    int count = 0;
    for (int i = start; i < end; i++) {
        vector<int> arr = split(i);

        int prev = -1;
        bool repeat = false;
        bool decrease = false;
        for (int val : arr) {
            if (prev == val)
                repeat = true;
            if (prev > val) {
                decrease = true;
                break;
            }
            prev = val;
        }
        if (repeat && !decrease)
        {
            for (int val : arr) {
                cout << val;
            }
            cout << endl;
            count++;
        }
    }
    cout << count << endl;
}