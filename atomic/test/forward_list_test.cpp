#include "forward_list.hpp"

#include <gtest/gtest.h>

#include <numeric>

TEST(ForwardList, InitTest) {
    atomic::forward_list<int> l;

    EXPECT_EQ(l.size(), 0);
    EXPECT_TRUE(l.empty());

    l.push_front(1);
    l.push_front(2);
    EXPECT_EQ(l.size(), 2);
    EXPECT_FALSE(l.empty());

    l.clear();
    EXPECT_EQ(l.size(), 0);
    EXPECT_TRUE(l.empty());
}

TEST(ForwardList, IteratorsTest) {
    atomic::forward_list<int> l;
    l.push_front(1);
    l.push_front(2);
    l.push_front(3);
    l.push_front(4);
    l.push_front(5);

    EXPECT_EQ(std::accumulate(l.begin(), l.end(), 0), 15);
}
