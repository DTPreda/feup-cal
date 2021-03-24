// By: Gonçalo Leão

#include "exercises.h"

unsigned int sumArray(unsigned int a[], unsigned int n) {
    unsigned int sum = 0;
    for(unsigned int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}

#include <iostream>

bool changeMakingGreedy(unsigned int C[], unsigned int n, unsigned int T) {
    for(int i = n - 1; i >= 0; i--){
        if(T >= C[i]){
            int quantity = T/C[i];
            T -= quantity*C[i];
            if(T == 0) return true;
        }
    }
    return false;
}

bool isCanonical(unsigned int C[], unsigned int n) {
    for(int i = C[2]; i < C[n - 1] + C[n - 2]; i++){
        if(!changeMakingGreedy(C, n, i)) return false;
    }
    return true;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex5, canonicalTrue) {
    unsigned int C[] = {1,2,5};
    EXPECT_EQ(isCanonical(C,3), true);

    unsigned int C2[] = {1,2,5,10};
    EXPECT_EQ(isCanonical(C2,4), true);
}

TEST(TP1_Ex5, canonicalFalse) {
    unsigned int C[] = {1,4,5};
    EXPECT_EQ(isCanonical(C,3), true);
}