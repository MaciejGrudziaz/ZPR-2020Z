#include <gtest/gtest.h>

#include "forward_list.hpp"

TEST(TestCase, TestName) {
    atomic::forward_list<int> l(5);
    EXPECT_EQ(l.get(), 5);
}
