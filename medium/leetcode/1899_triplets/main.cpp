// https://leetcode.com/problems/merge-triplets-to-form-target-triplet/

#include <vector>
using namespace std;

bool mergeTriplets(vector<vector<int>>& triplets, vector<int>& target) {
    vector<bool> flags{false, false, false};
    for (const auto& triplet : triplets) {
        for (int i = 0; i < 3; ++i) {
            int neighbour1 = (i + 1) % 3;
            int neighbour2 = (i + 2) % 3;
            if (triplet[i] == target[i] &&
                triplet[neighbour1] <= target[neighbour1] &&
                triplet[neighbour2] <= target[neighbour2]) {
                flags[i] = true;
            }
        }
    }
    if (flags == vector<bool>{true, true, true}) {
        return true;
    }
    return false;
}