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
        int repeatCombo = 0;
        bool repeats = false;
        bool decrease = false;
        for (int val : arr) {
            if (prev == val) {
                repeatCombo++;
            } else {
                if (repeatCombo == 2)
                    repeats = true;
                repeatCombo = 1;
            }
            if (prev > val) {
                decrease = true;
                break;
            }
            prev = val;
        }
        if (repeatCombo == 2)
            repeats = true;
        if (repeats && !decrease)
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