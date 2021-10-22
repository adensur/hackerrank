#include <cctype>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// problem: https://www.hackerrank.com/challenges/new-year-chaos/problem?isFullScreen=true&h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=arrays

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'minimumBribes' function below.
 *
 * The function accepts INTEGER_ARRAY q as parameter.
 */

using i64 = int64_t;

class TSegmentTree {
    struct TNode {
        TNode* LeftChild = nullptr;
        TNode* RightChild = nullptr;
        i64 Value;
        i64 LeftIndex;
        i64 RightIndex;
        TNode() = default;
    };
public:
    TSegmentTree(const std::vector<i64>& vec) {
        std::vector<TNode*> nodes;
        nodes.reserve(vec.size());
        for (i64 i = 0; i < vec.size(); ++i) {
            TNode* node = new TNode();
            node->Value = vec[i];
            node->LeftIndex = i;
            node->RightIndex = i + 1;
            nodes.push_back(node);
        }
        std::vector<TNode*> nodes2;
        nodes2.reserve(nodes.size() / 2 + 1);
        while (true) {
            for (i64 i = 0; i < nodes.size(); i += 2) {
                TNode* left = nodes[i];
                if (i + 1 >= nodes.size()) { // no right child here!
                    nodes2.push_back(left);
                    continue;
                }
                TNode* right = nodes[i + 1];
                TNode* node = new TNode();
                node->LeftChild = left;
                node->RightChild = right;
                node->Value = left->Value + right->Value;
                node->LeftIndex = left->LeftIndex;
                node->RightIndex = right->RightIndex;
                nodes2.push_back(node);
            }
            if (nodes2.size() == 1) {
                Root_ = nodes2[0];
                return;
            }
            std::swap(nodes, nodes2);
            nodes2.clear();
        }
    }
    void Update(i64 index, i64 newValue) {
        UpdateInner(index, newValue, Root_);
    }
    i64 CalcSum(i64 left, i64 right) { // [left, right)
        return CalcSumInner(left, right, Root_);
    }
private:
    TNode* Root_ = nullptr;
    void UpdateInner(i64 index, i64 newValue, TNode* node) {
        if (index < node->LeftIndex || index >= node->RightIndex) {
            return;
        }
        if (index == node->LeftIndex && index == node->RightIndex - 1) {
            node->Value = newValue;
            return;
        }   
        i64 newSum = 0;
        if (node->LeftChild) {
            UpdateInner(index, newValue, node->LeftChild);
            newSum += node->LeftChild->Value;
        }
        if (node->RightChild) {
            UpdateInner(index, newValue, node->RightChild);
            newSum += node->RightChild->Value;
        }
        node->Value = newSum;
    }
    i64 CalcSumInner(i64 left, i64 right, TNode* node) {
        if (node->RightIndex <= left || node->LeftIndex >= right) {
            return 0;
        }
        if (node->LeftIndex == left && node->RightIndex == right) {
            return node->Value;
        }
        i64 sum = 0;
        if (node->LeftChild) {
            sum += CalcSumInner(left, std::min(node->LeftChild->RightIndex, right), node->LeftChild);
        }
        if (node->RightChild) {
            sum += CalcSumInner(std::max(node->RightChild->LeftIndex, left), right, node->RightChild);
        }
        return sum;
    }
};

void minimumBribes(vector<int> q) {
    std::vector<i64> vec(q.size(), 0);
    TSegmentTree tree(vec);
    i64 result = 0;
    for (i64 i = 0; i < q.size(); ++i) {
        i64 offset = tree.CalcSum(q[i], q.size());
        i64 trueCurrentPos = i - offset;
        i64 expectedPos = q[i] - 1;
        i64 delta = expectedPos - trueCurrentPos;
        if (delta > 2) {
            std::cout << "Too chaotic" << std::endl;
            return;
        }
        tree.Update(expectedPos, 1);
        if (delta > 0) {
            result += delta;
        }
    }
    std::cout << result << std::endl;
}

int main()
{
    string t_temp;
    getline(cin, t_temp);

    int t = stoi(ltrim(rtrim(t_temp)));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string n_temp;
        getline(cin, n_temp);

        int n = stoi(ltrim(rtrim(n_temp)));

        string q_temp_temp;
        getline(cin, q_temp_temp);

        vector<string> q_temp = split(rtrim(q_temp_temp));

        vector<int> q(n);

        for (int i = 0; i < n; i++) {
            int q_item = stoi(q_temp[i]);

            q[i] = q_item;
        }

        minimumBribes(q);
    }

    return 0;
}

string ltrim(const string &str) {
    string s(str);
    return s;
}

string rtrim(const string &str) {
    string s(str);

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
