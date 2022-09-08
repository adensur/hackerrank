#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

unordered_map<char, vector<char>> mapping = {
    {'2', {'a', 'b', 'c'}}, {'3', {'d', 'e', 'f'}},
    {'4', {'g', 'h', 'i'}}, {'5', {'j', 'k', 'l'}},
    {'6', {'m', 'n', 'o'}}, {'7', {'p', 'q', 'r', 's'}},
    {'8', {'t', 'u', 'v'}}, {'9', {'w', 'x', 'y', 'z'}}};

vector<string> letterCombinations(string digits) {
    vector<string> result;
    if (digits.size() == 0) {
        return result;
    }
    vector<int> indices(digits.size(), 0);
    string cur;
    auto process_result = [&] {
        cur.clear();
        for (int j = 0; j < indices.size(); ++j) {
            cur += mapping[digits[j]][indices[j]];
        }
        result.push_back(cur);
    };
    process_result();
    int i = 0;
    while (true) {
        // try to increment current index
        int index = indices[i];
        if (index + 1 >= mapping[digits[i]].size()) {
            indices[i] = 0;
            ++i;
            if (i >= digits.size()) {
                break;
            }
            continue;
        }
        ++indices[i];
        i = 0;
        process_result();
    }
    return result;
}

int main() {
    string number;
    cin >> number;
    vector<string> result = letterCombinations(number);
    for (string num : result) {
        cout << num << endl;
    }
}