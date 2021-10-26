#include <iostream>
#include <random>
#include <sstream>
#include <vector>

using i64 = int64_t;
constexpr i64 Inf = std::numeric_limits<i64>::max();

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
                node->Value = std::min(left->Value, right->Value);
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
    i64 CalcMin(i64 left, i64 right) { // [left, right)
        return CalcMinInner(left, right, Root_);
    }
    void GroupAdd(i64 left, i64 right, i64 addValue) {
        GroupAddInner(left, right, addValue, Root_);
    }
private:
    TNode* Root_ = nullptr;
    i64 CalcMinInner(i64 left, i64 right, TNode* node) {
        if (node->RightIndex <= left || node->LeftIndex >= right) {
            return Inf;
        }
        if (node->LeftIndex == left && node->RightIndex == right) {
            return node->Value + node->AddModification;
        }
        PropagateModification(node);
        i64 min = CalcMinInner(left, std::min(node->LeftChild->RightIndex, right), node->LeftChild);
        if (node->RightChild) {
            min = std::min(min, CalcMinInner(std::max(node->RightChild->LeftIndex, left), right, node->RightChild));
        }
        return min;
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
            node->Value = std::min(node->Value, node->RightChild->Value + node->RightChild->AddModification);
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
        node->Value += node->AddModification;
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

i64 CalcMinNaive(const std::vector<i64>& vec, i64 left, i64 right) {
    i64 result = vec[left];
    for (i64 i = left + 1; i < right; ++i) {
        result = std::min(result, vec[i]);
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
            i64 left = 4;
            i64 right = 8;
            i64 min = tree.CalcMin(left, right);
            i64 expected = CalcMinNaive(vec, left, right);
            if (min != expected) {
                std::cout << "Error on vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
                throw std::runtime_error("");
            }
        }
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
            i64 min = tree.CalcMin(left, right);
            i64 expected = CalcMinNaive(vec, left, right);
            if (min != expected) {
                std::cout << "Error on vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
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
            i64 min = tree.CalcMin(left, right);
            i64 expected = CalcMinNaive(vec, left, right);
            if (min != expected) {
                std::cout << "Error on vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
                throw std::runtime_error("");
            }
        }
        {
            ++i;
            std::vector<i64> vec = {1, 7, 2, 3, 6};
            TSegmentTree tree(vec);
            i64 left = 0;
            i64 right = 1;
            i64 min = tree.CalcMin(left, right);
            i64 expected = CalcMinNaive(vec, left, right);
            if (min != expected) {
                std::cout << "Error on vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
                throw std::runtime_error("");
            }
        }
        {
            ++i;
            std::vector<i64> vec = {9, 3, 8};
            TSegmentTree tree(vec);
            i64 left = 1;
            i64 right = 2;
            i64 min = tree.CalcMin(left, right);
            i64 expected = CalcMinNaive(vec, left, right);
            if (min != expected) {
                std::cout << "Error on vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
                throw std::runtime_error("");
            }
        }
        {
            ++i;
            std::vector<i64> vec = {0, 1, 2, 3, 4, 5};
            TSegmentTree tree(vec);
            i64 left = 0;
            i64 right = 6;
            i64 min = tree.CalcMin(left, right);
            i64 expected = CalcMinNaive(vec, left, right);
            if (min != expected) {
                std::cout << "Error on vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
                throw std::runtime_error("");
            }
        }
    }
    {
        std::cout << "Starting brute force testing" << std::endl;
        for (i64 q = 0; q < 1'000; ++q) {
            i64 size = rand() % 20 + 1;
            std::vector<i64> vec = GenerateVector(size, 10);
            TSegmentTree tree(vec);
            for (i64 w = 0; w < 1'000; ++w) {
                i64 left = rand() % size;
                i64 right = rand() % (size - left) + left + 1;
                if (left >= right || right >= size + 1 || left >= size) {
                    std::cout << "Wrong left or right, left: " << left << "; right: " << right << "; size: " << size << std::endl;
                }
                i64 expected = CalcMinNaive(vec, left, right);
                i64 min = tree.CalcMin(left, right);
                if (min != expected) {
                    std::cout << "Error on case# " << q << " iteration #" << w << " vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
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