#include <deque>
#include <iostream>
#include <vector>

using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

int maxLevelSum(TreeNode *root)
{
    std::deque<std::pair<TreeNode *, int>> queue; // node, lvl
    queue.emplace_back(root, 1);
    int max = root->val;
    int maxLvl = 1;
    int currentSum = root->val;
    int lastLevel = 0;
    while (!queue.empty())
    {
        const auto [node, lvl] = queue.front();
        queue.pop_front();
        if (lvl != lastLevel)
        {
            if (currentSum > max)
            {
                max = currentSum;
                maxLvl = lastLevel;
                cout << "new max: " << max << " maxLvl: " << maxLvl << endl;
            }
            currentSum = 0;
        }
        lastLevel = lvl;
        currentSum += node->val;
        if (node->left != nullptr)
        {
            queue.emplace_back(node->left, lvl + 1);
        }
        if (node->right != nullptr)
        {
            queue.emplace_back(node->right, lvl + 1);
        }
    }
    if (currentSum > max)
    {
        max = currentSum;
        maxLvl = lastLevel;
    }
    return maxLvl;
}

int main()
{
    // build a tree for [989,null,10250,98693,-89388,null,null,null,-32127]
    // TreeNode *root = new TreeNode(989);
    // root->right = new TreeNode(10250);
    // root->right->left = new TreeNode(98693);
    // root->right->right = new TreeNode(-89388);
    // root->right->right->right = new TreeNode(-32127);

    // build a tree for [-100,-200,-300,-20,-5,-10,null]
    auto *root = new TreeNode(-100);
    auto
            cout
        << maxLevelSum(root) << endl;
}