#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
    [ ] Proper recursive modification propagation - all the way to the root!
*/

using i64 = int64_t;
constexpr i64 Inf = std::numeric_limits<i64>::min();

class TSegmentTree {
    struct TNode {
        TNode* LeftChild = nullptr;
        TNode* RightChild = nullptr;
        i64 Value;
        i64 LeftIndex;
        i64 RightIndex;
        TNode() = default;
        i64 AddModification = 0;
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
                node->Value = std::max(left->Value, right->Value);
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
    i64 CalcMax(i64 left, i64 right) { // [left, right)
        return CalcMaxInner(left, right, Root_);
    }
    void GroupAdd(i64 left, i64 right, i64 addValue) {
        GroupAddInner(left, right, addValue, Root_);
    }
private:
    TNode* Root_ = nullptr;
    i64 CalcMaxInner(i64 left, i64 right, TNode* node) {
        if (node->RightIndex <= left || node->LeftIndex >= right) {
            return Inf;
        }
        if (node->LeftIndex == left && node->RightIndex == right) {
            return node->Value + node->AddModification;
        }
        PropagateModification(node);
        i64 max = CalcMaxInner(left, std::min(node->LeftChild->RightIndex, right), node->LeftChild);
        if (node->RightChild) {
            max = std::max(max, CalcMaxInner(std::max(node->RightChild->LeftIndex, left), right, node->RightChild));
        }
        return max;
    }
    void GroupAddInner(i64 left, i64 right, i64 addValue, TNode* node) {
        if (node->RightIndex <= left || node->LeftIndex >= right) {
            return;
        }
        if (node->LeftIndex == left && node->RightIndex == right) {
            node->AddModification += addValue;
            return;
        }
        PropagateModification(node);
        GroupAddInner(left, std::min(node->LeftChild->RightIndex, right), addValue, node->LeftChild);
        node->Value = node->LeftChild->Value + node->LeftChild->AddModification;
        if (node->RightChild) {
            GroupAddInner(std::max(node->RightChild->LeftIndex, left), right, addValue, node->RightChild);
            node->Value = std::max(node->Value, node->RightChild->Value + node->RightChild->AddModification);
        }
    }
    void PropagateModification(TNode* node) {
        // propagate modification one step - from node to its children, applies to current node
        if (node->LeftChild) {
            node->LeftChild->AddModification += node->AddModification;
        }
        if (node->RightChild) {
            node->RightChild->AddModification += node->AddModification;
        }
        node->AddModification = 0;
    }
};

/*
 * Complete the 'arrayManipulation' function below.
 *
 * The function is expected to return a LONG_INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. 2D_INTEGER_ARRAY queries
 */
long arrayManipulation(int n, vector<vector<int>> queries) {
    std::vector<i64> vec(n, 0);
    TSegmentTree tree(vec);
    for (const vector<int>& query : queries) {
        int a = query[0];
        int b = query[1];
        int k = query[2];
        tree.GroupAdd(a - 1, b, k);
    }
    return tree.CalcMax(0, n);
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    int m = stoi(first_multiple_input[1]);

    vector<vector<int>> queries(m);

    for (int i = 0; i < m; i++) {
        queries[i].resize(3);

        string queries_row_temp_temp;
        getline(cin, queries_row_temp_temp);

        vector<string> queries_row_temp = split(rtrim(queries_row_temp_temp));

        for (int j = 0; j < 3; j++) {
            int queries_row_item = stoi(queries_row_temp[j]);

            queries[i][j] = queries_row_item;
        }
    }

    long result = arrayManipulation(n, queries);

    std::cout << result << "\n";

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
