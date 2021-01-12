#include <gtest/gtest.h>
#include <numeric>
#include "./../src/bidir_list_interface.hpp"
#include <thread>
#include <unistd.h>




TEST(Bidirectional_List, List_Init) {
atomic_list::Atomic_Blist<int> li;
}

TEST(Bidirectional_List, list_Push_and_POP_Back) {
atomic_list::Atomic_Blist<int> li;
li.push_back(1);
li.push_back(2);
li.push_back(3);
li.push_back(4);
    EXPECT_EQ(*li.begin(),1);
    EXPECT_EQ(*li.end(),4);
li.pop_back();
li.pop_back();
li.pop_back();
    EXPECT_EQ(*li.begin(),1);
    EXPECT_EQ(*li.end(),1);
li.clear();
}

TEST(Bidirectional_List, list_Push_and_POP_Front) {
atomic_list::Atomic_Blist<int> li;
li.push_front(1);
li.push_front(2);
li.push_front(3);
li.push_front(4);
    EXPECT_EQ(*li.begin(),4);
    EXPECT_EQ(*li.end(),1);
li.pop_front();
li.pop_front();
li.pop_front();
    EXPECT_EQ(*li.begin(),1);
    EXPECT_EQ(*li.end(),1);
li.clear();


}

TEST(Bidirectional_List, Manipulating_List_Test) {
        atomic_list::Atomic_Blist<int> li;
    li.push_back(1);
    li.push_back(2);
    li.push_back(3);
    li.push_back(4);
    li.push_back(5);
    li.push_back(6);

    EXPECT_EQ(li.end()==li.end(),1);    //equal operator test
    EXPECT_EQ(li.begin()==li.end(),0);    //equal operator test
    EXPECT_EQ(li.end()!=li.end(),0);    //not equal operator test
    EXPECT_EQ(li.begin()!=li.end(),1);    //not equal operator test
    int i=1;
     for (auto ptr = li.begin(); ptr != li.end(); ++ptr) //incrementtion
     {
         EXPECT_EQ(*ptr,i);
         ++i;
     }
    i=1;
     for (auto ptr = li.begin(); ptr != li.end(); ptr++) //incrementtion
     {
         EXPECT_EQ(*ptr,i);
         i++;
     }




     EXPECT_EQ(std::accumulate(li.begin(), li.end(), 0), 15);

        
}

atomic_list::Atomic_Blist<int> li;
atomic_list::Atomic_Blist<int> out;
void thread_1()
{
   for(int i=0;i<100;++i)
   {
        li.push_back(i);
        //li.push_front(i);
   }
}
void thread_2()
{
   for(int i=0;i<100;++i)
   {
        li.push_back(i);
        //li.push_front(i);
   }
}
void thread_3()
{
   for(int i=0;i<100;++i)
   {
        li.push_back(i);
        //li.push_front(i);
   }
}
void thread_4()
{
   for(int i=0;i<100;++i)
   {
        li.push_back(i);
        //li.push_front(i);
   }
}

void r_thread_1()
{
   for(int i=0;i<100;++i)
   {
        out.push_back(li.pop_back());
      //  out.push_front(li.pop_front());
   }
}
void r_thread_2()
{
   for(int i=0;i<100;++i)
   {
       out.push_back(li.pop_back());
      // out.push_front(li.pop_front());
   }
}
void r_thread_3()
{
   for(int i=0;i<100;++i)
   {
       out.push_back(li.pop_back());
      // out.push_front(li.pop_front());
   }
}
void r_thread_4()
{
   for(int i=0;i<100;++i)
   {
        out.push_back(li.pop_back());
        //out.push_front(li.pop_front());
   }
}




TEST(Bidirectional_List, MUTEX_Test) {
for(int iterator_i=0; iterator_i<1; ++iterator_i)
{

    std::thread thr1(thread_1);
    std::thread thr2(thread_2);
    std::thread thr3(thread_3);
    std::thread thr4(thread_4);


    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    int expected=0;

    for(int i=0;i<100;++i)
    {
        expected=expected+(i);
    }
    expected=expected*4;
    li.push_back(0);
    EXPECT_EQ(std::accumulate(li.begin(), li.end(), 0), expected);


    std::thread rthr1(r_thread_1);
    std::thread rthr2(r_thread_2);
    std::thread rthr3(r_thread_3);
    std::thread rthr4(r_thread_4);


    rthr1.join();
    rthr2.join();
    rthr3.join();
    rthr4.join();

    out.push_back(0);
    EXPECT_EQ(std::accumulate(out.begin(), out.end(), 0), expected);


out.clear();
li.clear();

}
}
