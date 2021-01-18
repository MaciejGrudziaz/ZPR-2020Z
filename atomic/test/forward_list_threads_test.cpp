#include <gtest/gtest.h>

#include <thread>

#include "forward_list.hpp"

TEST(ForwardListThreadTest, ProducerConsumerTest) {
    using List = atomic::forward_list<int>;

    const int COUNT = 100;
    List l(0, 3 * COUNT, COUNT);

    int consumed = 0;

    std::thread producer([&]() {
        auto secIt = l.sector_begin();
        List::iterator it(secIt);

        while (consumed < 1000000) {
            int i = 0;
            while (it.get_sector_it() != it.sector_end()) {
                std::cout << "produce: " << i << std::endl;
                *(++it) = i++;
            }

            ++secIt;
            if (secIt != l.sector_end()) {
                it = List::iterator(secIt);
            } else {
                secIt = l.sector_begin();
                it = List::iterator(secIt);
            }
        }
    });

    std::thread consumer([&]() {
        auto secIt = ++l.sector_begin();
        List::iterator it(secIt);

        while (consumed < 1000000) {
            while (it.get_sector_it() != it.sector_end()) {
                std::cout << "consume: " << *(++it) << std::endl;
                ++consumed;
            }

            ++secIt;
            if (secIt != l.sector_end()) {
                it = List::iterator(secIt);
            } else {
                secIt = l.sector_begin();
                it = List::iterator(secIt);
            }
        }
    });

    consumer.join();
    producer.join();

    EXPECT_EQ(consumed, 1000200);
}
