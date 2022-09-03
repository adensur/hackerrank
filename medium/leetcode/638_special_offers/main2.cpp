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

int shoppingOffers(vector<int> &price, vector<vector<int>> &special,
                   vector<int> &needs) {
    vector<int> cur(needs.size());
    unordered_map<vector<int>, int, VecHasher> states;  // state -> price
    states[cur] = 0;  // zero items cost zero price
    if (cur == needs) {
        return 0;
    }
    size_t i = 0;
    while (true) {
        if (cur[i] >= needs[i]) {
            if (i == needs.size()) {
                // we've iterated over all possible variants!
                break;
            }
            cur[i] = 0;
            ++i;
        } else {
            ++cur[i];
            i = 0;
            // actually do stuff
            int cur_price = numeric_limits<int>::max();
            // try to iterate over all individual items
            for (int j = 0; j < price.size(); ++j) {
                vector<int> cpy = cur;
                if (cpy[j] <= 0) {
                    continue;
                }
                cpy[j] -= 1;
                int next_price = states[cpy] + price[j];
                cur_price = min(cur_price, next_price);
            }
            // try to iterate over all offers
            for (int j = 0; j < special.size(); ++j) {
                vector<int> cpy = cur;
                minus_in_place(cpy, special[j]);
                if (!positive(cpy)) {
                    continue;
                }
                int next_price =
                    states[cpy] + special[j][special[j].size() - 1];
                cur_price = min(cur_price, next_price);
            }
            if (cur == needs) {
                return cur_price;
            }
            states[cur] = cur_price;
        }
    }
    return states[needs];
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