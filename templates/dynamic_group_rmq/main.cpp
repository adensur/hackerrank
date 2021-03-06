#include <iostream>
#include <random>
#include <sstream>
#include <vector>

using i64 = int64_t;
constexpr i64 Inf = std::numeric_limits<i64>::max();

class TSegmentTree {
private:
    struct TNode {
        i64 Value = Inf;
        i64 AddModification = 0;
    };
    i64 GetInnerIndex(i64 index) {
        // maps external index to final layer of the tree
        return index + Size_ / 2 - 1;
    }
    i64 GetLeftChild(i64 index) {
        return 2 * index + 1;
    }
    i64 GetRightChild(i64 index) {
        return 2 * index + 2;
    }
public:
    TSegmentTree(const std::vector<i64>& vec) {
        Size_ = pow(2, 1 + ceil(log2(vec.size())));
        Nodes_.resize(Size_, TNode());
        for (i64 i = 0; i < vec.size(); ++i) {
            Nodes_[GetInnerIndex(i)].Value = vec[i];
        }
        for (i64 i = Size_ / 2 - 2; i >= 0; --i) {
            Nodes_[i].Value = std::min(Nodes_[GetLeftChild(i)].Value, Nodes_[GetRightChild(i)].Value);
        }
    }
    i64 CalcMin(i64 queryLeft, i64 queryRight) { // [left, right)
        return CalcMinInner(queryLeft, queryRight, /* node = */ 0, /* left = */ 0, /* right = */ Size_ / 2);
    }
    void GroupAdd(i64 queryLeft, i64 queryRight, i64 addValue) {
        GroupAddInner(queryLeft, queryRight, addValue, /* node = */ 0, /* left = */ 0, /* right = */ Size_ / 2);
    }
private:
    i64 Size_ = 0;
    std::vector<TNode> Nodes_;
    i64 CalcMinInner(i64 queryLeft, i64 queryRight, i64 node, i64 left, i64 right) {
        if (right <= queryLeft || left >= queryRight) {
            return Inf;
        }
        if (left == queryLeft && right == queryRight) {
            return Nodes_[node].Value + Nodes_[node].AddModification;
        }
        PropagateModification(node);
        i64 middle = (left + right) / 2;
        i64 min = std::min(CalcMinInner(queryLeft, std::min(middle, queryRight), GetLeftChild(node), left, middle),
            CalcMinInner(std::max(middle, queryLeft), queryRight, GetRightChild(node), middle, right));
        return min;
    }
    void GroupAddInner(i64 queryLeft, i64 queryRight, i64 addValue, i64 node, i64 left, i64 right) {
        if (right <= queryLeft || left >= queryRight) {
            return;
        }
        if (left == queryLeft && right == queryRight) {
            Nodes_[node].AddModification += addValue;
            return;
        }
        PropagateModification(node);
        i64 middle = (left + right) / 2;
        GroupAddInner(queryLeft, std::min(middle, queryRight), addValue, GetLeftChild(node), left, middle);
        GroupAddInner(std::max(middle, queryLeft), queryRight, addValue, GetRightChild(node), middle, right);
        Nodes_[node].Value = std::min(Nodes_[GetLeftChild(node)].Value + Nodes_[GetLeftChild(node)].AddModification,
            Nodes_[GetRightChild(node)].Value + Nodes_[GetRightChild(node)].AddModification);
    }
    void PropagateModification(i64 node) {
        // propagate modification one step - from node to its children, applies to current node
        Nodes_[GetLeftChild(node)].AddModification += Nodes_[node].AddModification;
        Nodes_[GetRightChild(node)].AddModification += Nodes_[node].AddModification;
        Nodes_[node].Value += Nodes_[node].AddModification;
        Nodes_[node].AddModification = 0;
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
            // 4
            std::vector<i64> vec = {9,3,8,0,2,4,8,3};
            TSegmentTree tree(vec);
            i64 left = 7;
            i64 right = 8;
            i64 min = tree.CalcMin(left, right);
            i64 expected = CalcMinNaive(vec, left, right);
            if (min != expected) {
                std::cout << "Error on case #" << i << " vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
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
                std::cout << "Error on case #" << i << " vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
                throw std::runtime_error("");
            }
        }
        {
            ++i;
            // 1
            std::vector<i64> vec = {9,3,8,0,2,4,8,3};
            TSegmentTree tree(vec);
            i64 left = 4;
            i64 right = 8;
            i64 min = tree.CalcMin(left, right);
            i64 expected = CalcMinNaive(vec, left, right);
            if (min != expected) {
                std::cout << "Error on case #" << i << " vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
                throw std::runtime_error("");
            }
        }
        {
            ++i;
            // 2
            std::vector<i64> vec = {9,3,8,0,2,4,8,3};
            TSegmentTree tree(vec);
            {
                i64 leftUpdate = 5;
                i64 rightUpdate = 6;
                i64 updateAdd = 42;
                GroupUpdateNaive(leftUpdate, rightUpdate, updateAdd, vec);
                tree.GroupAdd(leftUpdate, rightUpdate, updateAdd);
            }
            i64 left = 4;
            i64 right = 8;
            i64 min = tree.CalcMin(left, right);
            i64 expected = CalcMinNaive(vec, left, right);
            if (min != expected) {
                std::cout << "Error on case #" << i << " vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
                throw std::runtime_error("");
            }
        }
        {
            ++i;
            // 3
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
                std::cout << "Error on case #" << i << " vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
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
                std::cout << "Error on case #" << i << " vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
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
                std::cout << "Error on case #" << i << " vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
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
                std::cout << "Error on case #" << i << " vec " << ToString(vec) << "; left: " << left << "; right: " << right << "; expected: " << expected << " got: " << min << std::endl;
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