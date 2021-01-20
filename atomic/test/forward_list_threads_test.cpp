#include <gtest/gtest.h>

#include <memory>
#include <thread>

#include "forward_list.hpp"

TEST(ForwardListThreadTest, ProducerConsumerTest) {
    using List = atomic::forward_list<int, atomic::lock_policy>;

    const int SIZE = 100;
    const int COUNT = 10000;
    List l(0, 2 * SIZE, SIZE);

    int consumed = 0;

    auto nextSector = [](List::sector_iterator& secIt, const List& list) {
        ++secIt;
        if (secIt == list.sector_end()) {
            secIt = list.sector_begin();
        }
        return List::iterator(secIt);
    };

    std::thread producer([&]() {
        auto secIt = l.sector_begin();
        List::iterator it(secIt);

        while (consumed < COUNT) {
            int i = 0;

            while (!it.is_sector_end()) {
                *(++it) = i++;
            }

            it = nextSector(secIt, l);
        }
    });

    std::thread consumer([&]() {
        auto secIt = ++l.sector_begin();
        List::iterator it(secIt);

        while (consumed < COUNT) {
            while (!it.is_sector_end()) {
                ++consumed;
                ++it;
            }

            it = nextSector(secIt, l);
        }
    });

    consumer.join();
    producer.join();

    EXPECT_EQ(consumed, COUNT + 98);
}

TEST(ForwardListThreadTest, AtomicTest) {
    using List = atomic::forward_list<int>;

    const int COUNT = 10000;

    List l(1);
    l.push_front(0);

    auto it1 = l.begin();

    l.push_front(-100);

    auto it2 = l.begin();

    std::thread th1([&]() {
        while (*it1 < COUNT) ++(*it1);
    });

    std::thread th2([&]() {
        while (*it2 < COUNT) ++(*it2);
    });

    std::thread th3([&]() {
        for (int i = 0; i < COUNT; ++i) {
            l.push_front(i);
        }
    });

    th1.join();
    th2.join();
    th3.join();

    EXPECT_EQ(l.size(), COUNT + 2);
    EXPECT_EQ(*l.begin(), COUNT - 1);
    EXPECT_EQ(*it1, COUNT);
    EXPECT_EQ(*it2, COUNT);
}
