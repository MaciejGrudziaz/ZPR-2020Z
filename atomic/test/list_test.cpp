#include <gtest/gtest.h>

#include "./../src/bidir_list_interface.hpp"


TEST(Bidirectional_List, List_Init) {
atomic_list::Atomic_Blist<int> li;
}

TEST(Bidirectional_List, list_Push_and_POP_Back) {
atomic_list::Atomic_Blist<int> li;
li.push_back(1);
li.push_back(2);
li.push_back(3);
li.push_back(4);
    EXPECT_EQ(li.begin(),1);
    EXPECT_EQ(li.end(),4);
li.pop_back();
li.pop_back();
li.pop_back();
    EXPECT_EQ(li.begin(),1);
    EXPECT_EQ(li.end(),1);
li.clear();
}

TEST(Bidirectional_List, list_Push_and_POP_Front) {
atomic_list::Atomic_Blist<int> li;
li.push_front(1);
li.push_front(2);
li.push_front(3);
li.push_front(4);
    EXPECT_EQ(li.begin(),4);
    EXPECT_EQ(li.end(),1);
li.pop_front();
li.pop_front();
li.pop_front();
    EXPECT_EQ(li.begin(),1);
    EXPECT_EQ(li.end(),1);
li.clear();


}

TEST(Bidirectional_List, Manipulating_List_Test) {
        atomic_list::Atomic_Blist<int> li;


        
}
