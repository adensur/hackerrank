// example: https://leetcode.com/problems/shopping-offers/
// given a list of counts for n items, iterate over all possible counts less
// than given

// state: vector<int>
#include <iostream>
#include <vector>

using namespace std;

void PrintVector(const vector<int> &vec) {
    for (int x : vec) {
        cout << x << " ";
    }
    cout << endl;
}

int main() {
    // read in required counts
    int N;
    cin >> N;
    vector<int> target(N, 0);
    for (int i = 0; i < N; ++i) {
        cin >> target[i];
    }
    int i = 0;
    vector<int> current(N, 0);
    while (true) {
        if (current[i] >= target[i]) {
            if (i == target.size()) {
                return 0;
            }
            current[i] = 0;
            ++i;
        } else {
            ++current[i];
            i = 0;
            PrintVector(current);
        }
    }
    return 0;
}