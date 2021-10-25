#include <iostream>
#include <random>
#include <sstream>
#include <vector>

using i64 = int64_t;

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
    void GroupAdd(i64 left, i64 right, i64 addValue) {
        GroupAddInner(left, right, addValue, Root_);
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
            return node->Value + node->AddModification * (node->RightIndex - node->LeftIndex);
        }
        PropagateModification(node);
        i64 sum = 0;
        if (node->LeftChild) {
            sum += CalcSumInner(left, std::min(node->LeftChild->RightIndex, right), node->LeftChild);
        }
        if (node->RightChild) {
            sum += CalcSumInner(std::max(node->RightChild->LeftIndex, left), right, node->RightChild);
        }
        return sum;
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
        node->Value += addValue * (right - left);
        if (node->LeftChild) {
            GroupAddInner(left, std::min(node->LeftChild->RightIndex, right), addValue, node->LeftChild);
        }
        if (node->RightChild) {
            GroupAddInner(std::max(node->RightChild->LeftIndex, left), right, addValue, node->RightChild);
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
        node->Value += node->AddModification * (node->RightIndex - node->LeftIndex);
        node->AddModification = 0;
    }
};

template <class T>
std::string ToString(const std::vector<T>& vec) {
    std::stringstream ss;
    for (const T& elem : vec) {
        ss << elem << ",";
    }
    return ss.str();
}

void GroupUpdateNaive(i64 left, i64 right, i64 addValue, std::vector<i64>& vec) {
    for (i64 i = left; i < right; ++i) {
        vec[i] += addValue;
    }
}

i64 CalcSumNaive(const std::vector<i64>& vec, i64 left, i64 right) {
    i64 result = 0;
    for (i64 i = left; i < right; ++i) {
        result += vec[i];
    }
    return result;
}

std::vector<i64> GenerateVector(i64 size, i64 maxNumber) {
    std::vector<i64> result;
    result.reserve(size);
    for (i64 i = 0; i < size; ++i) {
        i64 num = rand() % maxNumber;
        result.push_back(num);
    }
    return result;
}

int main() {
    {
        std::cout << "testcase testing" << std::endl;
        i64 i = 0;
        {
            ++i;
            std::vector<i64> vec = {9,3,8,0,2,4,8,3};
            TSegmentTree tree(vec);
            {
                i64 leftUpdate = 5;
                i64 rightUpdate = 6;
                i64 updateAdd = 42;
                GroupUpdateNaive(leftUpdate, rightUpdate, updateAdd, vec);
                tree.GroupAdd(leftUpdate, rightUpdate, updateAdd);
            }
            {
                i64 leftUpdate = 7;
                i64 rightUpdate = 8;
                i64 updateAdd = 840;
                GroupUpdateNaive(leftUpdate, rightUpdate, updateAdd, vec);
                tree.GroupAdd(leftUpdate, rightUpdate, updateAdd);
            }
            {
                i64 leftUpdate = 1;
                i64 rightUpdate = 3;
                i64 updateAdd = 157;
                GroupUpdateNaive(leftUpdate, rightUpdate, updateAdd, vec);
                tree.GroupAdd(leftUpdate, rightUpdate, updateAdd);
            }
            {
                i64 leftUpdate = 3;
                i64 rightUpdate = 7;
                i64 updateAdd = 816;
                GroupUpdateNaive(leftUpdate, rightUpdate, updateAdd, vec);
                tree.GroupAdd(leftUpdate, rightUpdate, updateAdd);
            }
            i64 left = 7;
            i64 right = 8;
            i64 sum = tree.CalcSum(left, right);
            i64 expected = CalcSumNaive(vec, left, right);
            if (sum != expected) {
                std::cout << "Error on vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << sum << std::endl;
                throw std::runtime_error("");
            }
        }
        {
            ++i;
            std::vector<i64> vec = {9, 3, 8, 0, 2, 4, 8, 3};
            TSegmentTree tree(vec);
            {
                i64 leftUpdate = 5;
                i64 rightUpdate = 6;
                i64 updateAdd = 42;
                GroupUpdateNaive(leftUpdate, rightUpdate, updateAdd, vec);
                tree.GroupAdd(leftUpdate, rightUpdate, updateAdd);
            }
            i64 left = 3;
            i64 right = 7;
            i64 sum = tree.CalcSum(left, right);
            i64 expected = 56;
            if (sum != expected) {
                std::cout << "Error on vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << sum << std::endl;
                throw std::runtime_error("");
            }
        }
        {
            ++i;
            std::vector<i64> vec = {1, 7, 2, 3, 6};
            TSegmentTree tree(vec);
            i64 left = 0;
            i64 right = 1;
            i64 sum = tree.CalcSum(left, right);
            i64 expected = 1;
            if (sum != expected) {
                std::cout << "Error on vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << sum << std::endl;
                throw std::runtime_error("");
            }
        }
        {
            ++i;
            std::vector<i64> vec = {9, 3, 8};
            TSegmentTree tree(vec);
            i64 left = 1;
            i64 right = 2;
            i64 sum = tree.CalcSum(left, right);
            i64 expected = 3;
            if (sum != expected) {
                std::cout << "Error on vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << sum << std::endl;
                throw std::runtime_error("");
            }
        }
        {
            ++i;
            std::vector<i64> vec = {0, 1, 2, 3, 4, 5};
            TSegmentTree tree(vec);
            i64 left = 0;
            i64 right = 6;
            i64 sum = tree.CalcSum(left, right);
            i64 expected = 15;
            if (sum != expected) {
                std::cout << "Error on vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << sum << std::endl;
                throw std::runtime_error("");
            }
        }
    }
    {
        std::cout << "Starting brute force testing" << std::endl;
        for (i64 q = 0; q < 1'000; ++q) {
            i64 size = rand() % 1000 + 1;
            std::vector<i64> vec = GenerateVector(size, 10);
            TSegmentTree tree(vec);
            for (i64 w = 0; w < 1'000; ++w) {
                i64 left = rand() % size;
                i64 right = rand() % (size - left) + left + 1;
                if (left >= right || right >= size + 1 || left >= size) {
                    std::cout << "Wrong left or right, left: " << left << "; right: " << right << "; size: " << size << std::endl;
                }
                i64 expected = CalcSumNaive(vec, left, right);
                i64 sum = tree.CalcSum(left, right);
                if (sum != expected) {
                    std::cout << "Error on case# " << q << " iteration #" << w << " vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << sum << std::endl;
                    throw std::runtime_error("");
                }
                // do some group updates
                left = rand() % size;
                right = rand() % (size - left) + left + 1;
                i64 addValue = rand() % 1000;
                if (left >= right || right >= size + 1 || left >= size) {
                    std::cout << "Wrong left or right, left: " << left << "; right: " << right << "; size: " << size << std::endl;
                }
                // std::cout << "group add left " << left << " right " << right << " add value " << addValue << " vec " << ToString(vec) << std::endl;
                GroupUpdateNaive(left, right, addValue, vec);
                tree.GroupAdd(left, right, addValue);
            }
        }
    }
    std::cout << "All success!" << std::endl;
}