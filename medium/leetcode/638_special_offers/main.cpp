#include <functional>
#include <iostream>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

struct VecHasher {
    int operator()(const vector<int> &state) const {
        stringstream ss;
        for (int x : state) {
            ss << x;
        }
        return hash<string>{}(ss.str());
    }
};

bool positive(const vector<int> &vec) {
    for (int v : vec) {
        if (v < 0) {
            return false;
        }
    }
    return true;
}

void minus_in_place(vector<int> &left, const vector<int> &right) {
    for (size_t i = 0; i < left.size(); ++i) {
        left[i] -= right[i];
    }
}

void plus_in_place(vector<int> &left, const vector<int> &right) {
    for (size_t i = 0; i < left.size(); ++i) {
        left[i] += right[i];
    }
}

int recurse(vector<int> &cur,
            unordered_map<vector<int>, int, VecHasher> &states,
            const vector<int> &price, const vector<vector<int>> &special,
            const vector<int> &needs) {
    if (states.count(cur) > 0) {
        return states[cur];
    }
    // iterate over individual prices
    int cur_price = numeric_limits<int>::max();
    for (int i = 0; i < price.size(); ++i) {
        if (cur[i] <= 0) {
            continue;
        }
        --cur[i];
        int prev_price = recurse(cur, states, price, special, needs);
        cur_price = min(cur_price, price[i] + prev_price);
        ++cur[i];
    }
    for (int i = 0; i < special.size(); ++i) {
        minus_in_place(cur, special[i]);
        if (positive(cur)) {
            int offer_price = special[i][special[i].size() - 1];
            if (offer_price <= cur_price) {
                int prev_price = recurse(cur, states, price, special, needs);
                cur_price = min(cur_price, offer_price + prev_price);
            }
        }
        plus_in_place(cur, special[i]);
    }
    states[cur] = cur_price;
    return cur_price;
}

int shoppingOffers(vector<int> &price, vector<vector<int>> &special,
                   vector<int> &needs) {
    unordered_map<vector<int>, int, VecHasher> states;  // counts -> min price
    vector<int> cur(needs.size(), 0);
    states[cur] = 0;  // price for zero items is zero
    cur = needs;
    return recurse(cur, states, price, special, needs);
}

int main() {
    int N;
    cin >> N;
    vector<int> price(N, 0);
    for (int i = 0; i < N; ++i) {
        cin >> price[i];
    }
    vector<int> needs(N, 0);
    for (int i = 0; i < N; ++i) {
        cin >> needs[i];
    }
    int K;
    cin >> K;
    vector<vector<int>> offers;
    for (int i = 0; i < K; ++i) {
        vector<int> offer(N + 1, 0);
        for (int j = 0; j < N + 1; ++j) {
            cin >> offer[j];
        }
        offers.push_back(std::move(offer));
    }
    cout << shoppingOffers(price, offers, needs) << endl;
}