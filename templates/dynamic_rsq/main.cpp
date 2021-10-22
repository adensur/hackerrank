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

template <class T>
std::string ToString(const std::vector<T>& vec) {
    std::stringstream ss;
    for (const T& elem : vec) {
        ss << elem << ",";
    }
    return ss.str();
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
        for (i64 q = 0; q < 10'000; ++q) {
            i64 size = rand() % 1000 + 1;
            std::vector<i64> vec = GenerateVector(size, 10);
            i64 left = rand() % size;
            i64 right = rand() % (size - left) + left + 1;
            if (left >= right || right >= size + 1 || left >= size) {
                std::cout << "Wrong left or right, left: " << left << "; right: " << right << "; size: " << size << std::endl;
            }
            i64 expected = CalcSumNaive(vec, left, right);
            TSegmentTree tree(vec);
            i64 sum = tree.CalcSum(left, right);
            if (sum != expected) {
                std::cout << "Error on case# " << q << " vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << sum << std::endl;
                throw std::runtime_error("");
            }
        }
    }
    std::cout << "All success!" << std::endl;
}