// https://leetcode.com/explore/interview/card/top-interview-questions-medium/103/array-and-strings/776/

class Solution {
   public:
    struct VecHasher {
        int operator()(const vector<int>& vec) const {
            std::hash<int> hasher;
            size_t seed = 0;
            seed ^= hasher(vec[0]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= hasher(vec[1]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= hasher(vec[2]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };

    vector<vector<int>> threeSum(vector<int>& nums) {
        unordered_map<int, vector<int>>
            map;  // value -> all indices with that value
        for (int i = 0; i < nums.size(); ++i) {
            if (map[nums[i]].size() <= 3) {
                map[nums[i]].push_back(i);
            }
        }
        unordered_set<vector<int>, VecHasher> set;
        vector<vector<int>> result;
        unordered_map<int, bool> seen;
        for (int i = 0; i < nums.size() - 1; ++i) {
            if (seen[nums[i]]) {
                continue;
            }
            seen[nums[i]] = true;
            for (int j = i + 1; j < nums.size(); ++j) {
                int cur_sum = nums[i] + nums[j];
                for (int k : map[-cur_sum]) {
                    if (k != i && k != j) {
                        vector<int> triplet({nums[i], nums[j], nums[k]});
                        sort(triplet.begin(), triplet.end());
                        if (set.count(triplet) == 0) {
                            result.emplace_back(triplet);
                            set.insert(std::move(triplet));
                        }
                    }
                }
            }
        }
        return result;
    }
};