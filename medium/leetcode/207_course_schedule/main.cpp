#include <deque>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Forest
{
    class Node
    {
    public:
        std::vector<int> children;
        bool isVisited = false;
        Node() {}
    };

public:
    std::unordered_map<int, Node> nodes; // nodeIdx -> childrenIdx
    Forest(const vector<vector<int>> &prerequisites)
    {
        for (const auto &prerequisite : prerequisites)
        {
            // idx1 depends on idx2
            // idx2 --> idx1
            int idx1 = prerequisite[0];
            int idx2 = prerequisite[1];
            if (!nodes.contains(idx1))
            {
                nodes[idx1] = Node();
            }
            if (!nodes.contains(idx2))
            {
                nodes[idx2] = Node();
            }
            nodes[idx2].children.push_back(idx1);
        }
    }

    bool hasCyclesDfs(int idx, std::unordered_set<int> &dfsStack)
    {
        auto &node = nodes[idx];
        if (dfsStack.count(idx) > 0)
        {
            // a cycle!
            return true;
        }
        if (node.isVisited)
        {
            // we've been here before - no point in descending further
            return false;
        }
        node.isVisited = true;
        dfsStack.insert(idx);
        for (int childIdx : node.children)
        {
            if (hasCyclesDfs(childIdx, dfsStack))
            {
                return true;
            }
        }
        dfsStack.erase(idx);
        return false;
    }

    bool canFinish()
    {
        for (const auto &[idx, node] : nodes)
        {
            // launch dfs to detect cycles
            std::unordered_set<int> dfsStack;
            auto hasCycles = hasCyclesDfs(idx, dfsStack);
            if (hasCycles)
            {
                return false;
            }
        }
        return true;
    }
};

bool canFinish(int numCourses, vector<vector<int>> &prerequisites)
{
    if (prerequisites.size() == 0)
    {
        return true;
    }
    auto forest = Forest(prerequisites);
    return forest.canFinish();
}

int main()
{
    int K; // test case count
    cin >> K;
    for (int k = 0; k < K; ++k)
    {
        int N; // prerequisites count
        cin >> N;
        vector<vector<int>> prereqs;
        for (int i = 0; i < N; ++i)
        {
            int a, b;
            cin >> a >> b;
            prereqs.push_back({a, b});
        }
        int ans;
        cin >> ans;
        int res = canFinish(prereqs.size(), prereqs);
        if (res != ans)
        {
            cout << "Test case # " << k << "; expected " << ans << " got " << res << endl;
        }
        else
        {
            cout << "Test case # " << k << " passed!" << endl;
        }
    }
}