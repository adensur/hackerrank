// https://leetcode.com/problems/n-ary-tree-level-order-traversal/

/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> children;

    Node() {}

    Node(int _val) {
        val = _val;
    }

    Node(int _val, vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};
*/

#include <deque>
#include <vector>
using namespace std;
class Node {
   public:
    int val;
    vector<Node*> children;

    Node() {}

    Node(int _val) { val = _val; }

    Node(int _val, vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};

class Solution {
   public:
    vector<vector<int>> levelOrder(Node* root) {
        // bfs
        if (root == nullptr) {
            return {};
        }
        vector<vector<int>> result;
        deque<pair<Node*, int>> queue;
        queue.emplace_back(root, 0);
        int prev_level = 0;
        vector<int> current_row{};
        while (queue.size() > 0) {
            auto [cur_node, level] = queue.front();
            queue.pop_front();
            if (level != prev_level) {
                result.push_back(std::move(current_row));
                current_row = vector<int>{};
                ++prev_level;
            }
            current_row.push_back(cur_node->val);
            for (Node* child : cur_node->children) {
                queue.emplace_back(child, prev_level + 1);
            }
        }
        result.push_back(std::move(current_row));
        return result;
    }
};
