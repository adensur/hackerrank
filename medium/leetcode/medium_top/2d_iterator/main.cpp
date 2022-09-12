// https://leetcode.com/problems/flatten-2d-vector

class Vector2D {
   private:
    size_t index1 = 0;
    size_t index2 = -1;
    vector<vector<int>>& data;
    void try_advance() {
        if (index2 + 1 < data[index1].size()) {
            ++index2;
            return;
        }
        index2 = 0;
        ++index1;
        while (index1 < data.size() && data[index1].size() == 0) {
            ++index1;
        }
    }

   public:
    Vector2D(vector<vector<int>>& vec) : data(vec) {
        if (vec.size() > 0) {
            try_advance();
        }
    }

    int next() {
        int res = data[index1][index2];
        try_advance();
        return res;
    }

    bool hasNext() {
        return index1 < data.size() && index2 < data[index1].size();
    }
};

/**
 * Your Vector2D object will be instantiated and called as such:
 * Vector2D* obj = new Vector2D(vec);
 * int param_1 = obj->next();
 * bool param_2 = obj->hasNext();
 */