#include <vector>
#include <iostream>

using namespace std;

int max(const vector<int> &nums)
{
    int res = nums[0];
    for (int i = 1; i < nums.size(); ++i)
    {
        if (nums[i] > res)
        {
            res = nums[i];
        }
    }
    return res;
}

int rob(vector<int> &nums)
{
    int N = nums.size();
    if (N < 4)
    {
        return max(nums);
    }
    vector<int> dyn;  // dyn[i] - max value from robbing all houses on [0, i)
    vector<int> dyn2; // dyn2[i] - max value from robbing all houses on [1, i]
    dyn.push_back(nums[0]);
    dyn.push_back(std::max(nums[1], nums[0]));
    dyn2.push_back(nums[1]);
    dyn2.push_back(std::max(nums[1], nums[2]));
    for (int i = 2; i < nums.size() - 1; ++i)
    {
        dyn.push_back(std::max(dyn[i - 1], nums[i] + dyn[i - 2]));
    }
    for (int i = 3; i < nums.size(); ++i)
    {
        dyn2.push_back(std::max(dyn2[i - 2], nums[i] + dyn2[i - 3]));
    }
    return std::max(dyn[dyn.size() - 1], dyn2[dyn2.size() - 1]);
}

int main()
{
    int N;
    cin >> N;
    for (int i = 0; i < N; ++i)
    {
        vector<int> nums;
        int n;
        while (cin >> n)
        {
            nums.push_back(n);
            if (cin.get() == '\n')
            {
                break;
            }
        }
        int ans;
        cin >> ans;
        int res = rob(nums);
        cout << res << endl;
        if (res != ans)
        {
            cout << "Wrong answer; expected " << ans << " got " << res << endl;
        }
    }
}