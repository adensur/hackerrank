#include <iostream>
#include <stringstream>
#include <unordered_map>
#include <vector>
using namespace std;

int hash(vector<int> &state) {
    sstream ss;
    for (int x : state) {
        ss << x;
    }
    return hash(ss.str());
}

int shoppingOffers(vector<int> &price, vector<vector<int>> &special,
                   vector<int> &needs) {
    vector<int> cur(needs.size());
    unordered_map<vector<int>, int> states;  // state -> price
    states[cur] = 0;                         // zero items cost zero price
    if (cur == needs) {
        return 0;
    }
    size_t i = 0;
    while (true) {
        if (cur[i] >= target[i]) {
            if (i == target.size()) {
                // we've iterated over all possible variants!
                break;
            }
            cur[i] = 0;
            ++i;
        } else {
            ++cur[i];
            i = 0;
            // actually do stuff
            int price = INT_MAX;
            // try to iterate over all individual items
            for (int j = 0; j < price.size(); ++j) {
                vector<int> cpy = cur;
                if (cpy[j] <= 0) {
                    continue;
                }
                cpy[j] -= 1;
                int next_price = states[cpy] + prices[j];
                price = min(price, next_price);
            }
            // try to iterate over all offers
            for (int j = 0; j < special.size(); ++j) {
                vector<int> cpy = cur;
                cpy = minus(cpy, special[j]);
                if (!positive(cpy)) {
                    continue;
                }
                int next_price = states[cpy] + special[j].last();
                price = min(price, next_price);
            }
        }
    }

    if (cur == needs) {
        return price;
    }
    states[cur] = price;
    --cur[i];
    for (int i = 0; i < needs.size(); ++i) {
        if (cur[i] >= needs[i]) {
            continue;
        }
        ++cur[i];
    }
}
}

int main() { cout << "Hello world!" << endl; }