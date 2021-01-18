#include "forward_list.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>

TEST(ForwardListTest, ConstructorsTest) {
    atomic::forward_list<std::string> l1;
    atomic::forward_list<int> l2({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 3);
    atomic::forward_list<std::string> l3("test", 100, 10);

    EXPECT_TRUE(l1.empty());
    EXPECT_EQ(l1.size(), 0);
    EXPECT_EQ(l1.sector_count(), 0);

    EXPECT_FALSE(l2.empty());
    EXPECT_EQ(l2.size(), 10);
    EXPECT_EQ(l2.sector_count(), 4);

    EXPECT_FALSE(l3.empty());
    EXPECT_EQ(l3.size(), 100);
    EXPECT_EQ(l3.sector_count(), 10);
}

TEST(ForwardListTest, ClearTest) {
    atomic::forward_list<int> l({1, 2, 3, 4, 5});

    EXPECT_FALSE(l.empty());
    EXPECT_EQ(l.size(), 5);

    l.clear();

    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);

    l.push_front(1);
    l.push_front(99);

    EXPECT_FALSE(l.empty());
    EXPECT_EQ(l.size(), 2);
}

TEST(ForwardListTest, PushPopTest) {
    atomic::forward_list<int> l(2);

    int val = 9;
    l.push_front(1);
    l.push_front(2);
    l.push_front(val);

    EXPECT_FALSE(l.empty());
    EXPECT_EQ(l.size(), 3);
    EXPECT_EQ(l.sector_count(), 2);
    EXPECT_EQ(*l.begin(), val);

    l.pop_front();
    l.pop_front();

    EXPECT_EQ(l.size(), 1);
    EXPECT_EQ(l.sector_count(), 1);
    EXPECT_EQ(*l.begin(), 1);
}

TEST(ForwardListTest, Iterators) {
    atomic::forward_list<int> l({1, 2, 3, 4});

    auto it = l.begin();

    EXPECT_EQ(l.size(), 4);
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*(++it), 2);
    EXPECT_EQ(*(++it), 3);
    EXPECT_EQ(*(++it), 4);
    EXPECT_EQ(++it, l.end());
}

TEST(ForwardListTest, FetchMultipleIteratorsForSingleSector) {
    atomic::forward_list<int> l({1, 2, 3, 4, 5});

    auto it = l.begin();
    EXPECT_THROW(l.begin(), std::runtime_error);
    EXPECT_THROW(it++, std::runtime_error);

    auto sec = l.sector_begin();
    EXPECT_THROW(atomic::forward_list<int>::iterator it2(sec), std::runtime_error);
}

TEST(ForwardListTest, FetchMultipleIteratorsForMultipleSectors) {
    using List = atomic::forward_list<int>;
    List l({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 3);

    EXPECT_EQ(l.sector_count(), 4);

    auto secIt = l.sector_begin();
    List::iterator it1(secIt);
    List::iterator it2(++secIt);
    List::iterator it3(++secIt);
    List::iterator it4(++secIt);

    EXPECT_EQ(++secIt, l.sector_end());
    EXPECT_EQ(*it1, 1);
    EXPECT_EQ(*it2, 2);
    EXPECT_EQ(*it3, 5);
    EXPECT_EQ(*it4, 8);
}

TEST(ForwardListTest, ConstInteratorsTest) {
    using List = atomic::forward_list<int>;
    atomic::forward_list<int> l({1, 2, 3, 4, 5});

    [](const List& list) {
        auto it1 = list.begin();
        auto it2 = ++list.begin();
        auto it3 = it2++;

        EXPECT_EQ(*it1, 1);
        EXPECT_EQ(*it2, 3);
        EXPECT_EQ(*it3, 2);
    }(l);
}

TEST(ForwardListTest, NonBlockingEndAccessTest) {
    atomic::forward_list<int> l({1, 2, 3, 4, 5});
    auto it1 = l.end();
    auto it2 = l.end();
    auto it3 = l.begin();
    ++(++(++(++(++(it3)))));

    EXPECT_EQ(it1, it2);
    EXPECT_EQ(it3, l.end());
    EXPECT_EQ(it3, it1);
}

TEST(ForwardListTest, StdAlgorithmsTest) {
    atomic::forward_list<int> l({1, 2, 3, 4, 5});

    EXPECT_EQ(std::accumulate(l.begin(), l.end(), 0), 15);
    EXPECT_NE(std::find(l.begin(), l.end(), 3), l.end());
    EXPECT_EQ(std::find(l.begin(), l.end(), 8), l.end());
    EXPECT_EQ(std::count_if(l.begin(), l.end(), [](int val) -> bool { return val % 2; }), 3);

    atomic::forward_list<int> l2;
    std::transform(l.begin(), l.end(), std::front_inserter(l2), [](int val) { return val * 10; });

    EXPECT_EQ(l2.size(), 5);
    EXPECT_EQ(*l2.begin(), 50);
}
