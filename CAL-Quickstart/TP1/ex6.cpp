// By: Gonçalo Leão

#include "exercises.h"
#include <algorithm>

bool Activity::operator==(const Activity &a2) const {
    return start == a2.start && finish == a2.finish;
}

bool Activity::operator<(const Activity &a2) const {
    return finish < a2.finish;
}

std::vector<Activity> earliestFinishScheduling(std::vector<Activity> A) {
    std::vector<Activity> res;
    std::sort(A.begin(), A.end());

    while(!A.empty()){
        res.push_back(A[0]);
        for(int i = A.size() - 1; i >= 0; i--){
            if((A[i].start == res[res.size() - 1].start && A[i].finish == res[res.size() - 1].finish) || A[i].start < res[res.size() - 1].finish){
                for(int j = i; j < A.size() - 1; j++) A[j] = A[j + 1];
                A.resize(A.size() - 1);
            }
        }
    }
    return res;
}

/// TESTS ///
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(TP1_Ex6, activityScheduling) {
    std::vector<Activity> A = {{10,20}, {30, 35}, {5, 15}, {10, 40}, {40, 50}};
    std::vector<Activity> V = earliestFinishScheduling(A);
    EXPECT_EQ(V.size(), 3 );
    ASSERT_THAT(earliestFinishScheduling(A),  ::testing::ElementsAre(Activity(5, 15), Activity(30, 35), Activity(40, 50)));
}