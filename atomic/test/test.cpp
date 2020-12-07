#include <gtest/gtest.h>

//#include "forward_list.hpp"
#include "bidir_list_interface.hpp"

TEST(TestCase, TestName) {
  //  atomic::forward_list<int> l(5);
   // EXPECT_EQ(l.get(), 5);



    atomic_list::Atomic_Blist<int> li;

    /*
    EXPECT_EQ(li.end().get(),5); li.pop_back();
EXPECT_EQ(li.end()->(),3);li.pop_back();
EXPECT_EQ(li.end().get(),2);li.pop_back();

*/
}
