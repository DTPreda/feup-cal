// By: Gonçalo Leão

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <class T>
bool in(vector<T>& v, vector<vector<T>>& cont){
    bool equal;
    for(int i = 0; i < cont.size(); i++){
        equal = true;
        for(int j = 0; j < cont[i].size() && j < v.size(); j++){
            if(v[j] != cont[i][j]){
                equal = false;
                break;
            }
        }
        if(equal) return true;
    }
    return false;
}

unsigned int sum(vector<unsigned int>& v){
    unsigned int ret = 0;
    for(auto elem: v) ret += elem;
    return ret;
}

void perms(vector<unsigned int> Stock, vector<unsigned int> current, unsigned int pos, vector<vector<unsigned int>>& ret){
    if(pos == current.size() - 1){
        for(int i = 0; i < Stock[pos] + 1; i++){
            current[pos] = i;
            if(!in(current, ret)) ret.push_back(current);
        }
        return;
    }
    for(int i = 0; i < Stock[pos] + 1; i++){
        current[pos] = i;
        if(!in(current, ret)) ret.push_back(current);
        for(int j = pos + 1; j < current.size(); j++){
            perms(Stock, current, j, ret);
        }
    }
}
bool changeMakingBF(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    vector<vector<unsigned int>> ret;
    vector<unsigned int> current;
    vector<unsigned int> stock;
    for(int i = 0; i < n; i++){
        current.push_back(0);
        stock.push_back(Stock[i]);
    }

    perms(stock, current, 0, ret);

    std::sort(ret.begin(), ret.end(), [](vector<unsigned int> v1, vector<unsigned int> v2){
        return sum(v1) < sum(v2);
    });

    unsigned int sum;
    for(int i = 0; i < ret.size(); i++){
        sum = 0;
        for(int j = 0; j < ret[i].size(); j++) sum += ret[i][j]*C[j];
        if(sum == T){
            for(int j = 0; j < ret[i].size(); j++) usedCoins[j] = ret[i][j];
            return true;
        }
    }

    return false;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex3, hasBFChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {1,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBF(C,Stock,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    unsigned int Stock2[] = {1,2,4,2};
    EXPECT_EQ(changeMakingBF(C,Stock2,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 3);
    EXPECT_EQ(usedCoins[3], 2);
}

TEST(TP1_Ex3, hasBFChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int Stock[] = {2,2,1};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingBF(C,Stock,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingBF(C,Stock,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);
}

TEST(TP1_Ex3, hasNoBFChange) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {0,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBF(C,Stock,n,18,usedCoins), false);
    EXPECT_EQ(changeMakingBF(C,Stock,n,1,usedCoins), false);
}