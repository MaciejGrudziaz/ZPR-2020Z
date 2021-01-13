#include "forward_list.hpp"

#include <gtest/gtest.h>

#include <numeric>
#include <thread>

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

TEST(ForwardList, Sectors) {
    atomic::forward_list<int> l(5);

    for (int i = 0; i < 25; ++i) {
        l.push_front(i);
    }

    EXPECT_EQ(l.sector_count(), 5);

    auto a = (*l.sector_begin()).begin();
    EXPECT_EQ(*a, 24);
    EXPECT_EQ(*(++a), 23);
    EXPECT_EQ(*(++a), 22);
    EXPECT_EQ(*(++a), 21);
    EXPECT_EQ(*(++a), 20);
    EXPECT_EQ(++a, (*l.sector_begin()).end());

    EXPECT_EQ(std::accumulate(l.begin(), l.end(), 0), 300);
}

TEST(ForwardList, SectorsIterator) {
    atomic::forward_list<int> l(5);

    for (int i = 0; i < 20; ++i) {
        l.push_front(i);
    }

    EXPECT_EQ(l.sector_count(), 4);

    auto begin = l.sector_begin();
    auto it = atomic::forward_list<int>::iterator(begin);
    int count = 0;

    while (begin == it) {
        count += *it;
        ++it;
    }

    EXPECT_EQ(count, 85);
}

TEST(ForwardList, InitializerListInit) {
    atomic::forward_list<int> l({1, 2, 3, 4, 5});

    EXPECT_EQ(l.size(), 5);
    EXPECT_EQ(l.sector_count(), 1);

    auto it = l.begin();
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*(++it), 2);
    EXPECT_EQ(*(++it), 3);
    EXPECT_EQ(*(++it), 4);
    EXPECT_EQ(*(++it), 5);
}

TEST(ForwardList, PopFront) {
    atomic::forward_list<int> l({1, 2, 3, 4, 5}, 2);

    EXPECT_EQ(l.size(), 5);
    EXPECT_EQ(l.sector_count(), 3);

    l.pop_front();
    l.pop_front();

    EXPECT_EQ(l.size(), 3);
    EXPECT_EQ(l.sector_count(), 2);
    EXPECT_EQ(*l.begin(), 3);

    l.pop_front();
    l.pop_front();

    EXPECT_EQ(l.size(), 1);
    EXPECT_EQ(l.sector_count(), 1);
    EXPECT_EQ(*l.begin(), 5);

    l.pop_front();
    EXPECT_TRUE(l.empty());
}

TEST(ForwardList, Atomic) {
    atomic::forward_list<int> l({1, 2, 3, 4, 5, 6}, 1);

    {
        auto it = l.begin();
        auto it2 = it++;
        *it = 8;
        *it2 = 9;
    }

    EXPECT_EQ(*l.begin(), 9);
    EXPECT_EQ(*(++l.begin()), 8);
}
