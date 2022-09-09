// https://leetcode.com/explore/interview/card/top-interview-questions-medium/110/sorting-and-searching/798/

#include <iostream>
#include <vector>

using namespace std;

class Solution {
   public:
    void sortColors(vector<int>& nums) {
        if (nums.size() <= 1) {
            return;
        }
        int zeros_ind = 0;
        int twos_ind = nums.size() - 1;
        int i = 0;
        while (i <= twos_ind) {
            if (nums[i] == 1) {
                ++i;
                continue;
            }
            if (nums[i] == 0) {
                // todo: inf cycle here
                swap(nums[i], nums[zeros_ind]);
                ++zeros_ind;
                ++i;
                continue;
            }
            // nums[i] == 2
            swap(nums[i], nums[twos_ind]);
            --twos_ind;
        }
    }
};

int main() { cout << "Hello world!" << endl; }