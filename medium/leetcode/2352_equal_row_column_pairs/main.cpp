#include <iostream>
#include <vector>
using namespace std;

// implement hash for vector<int>
struct VectorHash
{
    std::size_t operator()(const vector<int> &vec) const
    {
        using std::hash;
        using std::size_t;
        using std::string;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:
        std::size_t seed = vec.size();
        for (auto &i : vec)
        {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

int equalPairs(vector<vector<int>> &grid)
{
    std::unordered_map<vector<int>, int, VectorHash> counts;
    for (const auto &col : grid)
    {
        ++counts[col];
    }
    int count = 0;
    for (int r = 0; r < grid.size(); ++r)
    {
        vector<int> row;
        for (int c = 0; c < grid.size(); ++c)
        {
            row.push_back(grid[c][r]);
        }
        count += counts[row];
    }
    return count;
}

int main()
{
    // parse input from stdin
    // number of test cases
    int K;
    cin >> K;
    for (int k = 0; k < K; ++k)
    {
        int N;
        cin >> N;
        vector<vector<int>> grid(N, vector<int>(N));
        for (int r = 0; r < N; ++r)
        {
            for (int q = 0; q < N; ++q)
            {
                cin >> grid[r][q];
            }
        }
        int ans;
        cin >> ans;
        int res = equalPairs(grid);
        if (res == ans)
        {
            cout << "Test case " << k + 1 << " passed." << endl;
        }
        else
        {
            cout << "Test case " << k + 1 << " failed." << endl;
            cout << "Expected " << ans << " but got " << res << endl;
        }
    }
}