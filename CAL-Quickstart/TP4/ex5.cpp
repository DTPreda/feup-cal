#include "exercises.h"
#include <vector>

unsigned long s_recursive(unsigned int n, unsigned int k) {
    if(k == n || k == 1) return 1;
    if(k > 1 && k < n) return s_recursive(n-1, k-1) + k * s_recursive(n-1, k);
    return 0;
}

unsigned long b_recursive(unsigned int n) {
    unsigned long ret = 0;
    for(int i = 1; i <= n; i++) ret += s_recursive(n, i);
    return ret;
}

std::vector<std::vector<unsigned long>> s(20, std::vector<unsigned long>(20, -1));
unsigned long s_dynamic(unsigned int n, unsigned int k) {
    if(k == n || k == 1){
        s[n - 1][k - 1] = 1;
        return 1;
    }
    if(s[n - 1][k - 1] != -1) return s[n - 1][k - 1];
    else {
        s[n - 1][k - 1] = s_dynamic(n-1, k-1) + k * s_dynamic(n-1, k);
        return s[n - 1][k - 1];
    }
}

unsigned long b_dynamic(unsigned int n) {
    unsigned long ret = 0;
    for(int i = 1; i <= n; i++) ret += s_dynamic(n, i);
    return ret;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex5, testPartitioningRecursive) {
    EXPECT_EQ(1,s_recursive(3,3));
    EXPECT_EQ(3025,s_recursive(9,3));
    EXPECT_EQ(22827,s_recursive(10,6));

    EXPECT_EQ(5,b_recursive(3));
    EXPECT_EQ(203,b_recursive(6));
    EXPECT_EQ(1382958545,b_recursive(15));
}

TEST(TP4_Ex5, testPartitioningDynamic) {
    EXPECT_EQ(1,s_dynamic(3,3));
    EXPECT_EQ(3025,s_dynamic(9,3));
    EXPECT_EQ(22827,s_dynamic(10,6));

    EXPECT_EQ(5,b_dynamic(3));
    EXPECT_EQ(203,b_dynamic(6));
    EXPECT_EQ(1382958545,b_dynamic(15));
}