// By: Gonçalo Leão

#include "exercises.h"
#include <iostream>

int maxSubsequence(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    int maxSum = INT32_MIN;
    int currentSum = maxSum;

    for(int k = 0; k < n; k++){
        currentSum = A[k];
        for(int l = k + 1; l < n; l++){
            currentSum += A[l];
            if(currentSum > maxSum){
                i = k;
                j = l;
                maxSum = currentSum;
            }
        }
    }

    for(int k = 0; k < n; k++){
        A[k] = 0;
        if(k >= i && k <= j) A[k] = 1;
    }

    return maxSum;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex2, maxSubsequence) {
    int A[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n = 9;
    unsigned int i, j;
    EXPECT_EQ(maxSubsequence(A,n,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
}