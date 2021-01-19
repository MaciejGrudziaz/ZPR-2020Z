#ifndef ATOMIC_THREAD_POLICY_H
#define ATOMIC_THREAD_POLICY_H

#include <mutex>

namespace atomic {

struct ignore_policy {
    static void execute(std::mutex& l) {}
};

struct lock_policy {
    static void execute(std::mutex& l) { l.lock(); }
};

struct exception_policy {
    static void execute(std::mutex& l) {
        if (!l.try_lock()) {
            throw std::runtime_error("cannot create two iterators on the same sector at one time (thread lock error)");
        }
    }
};

}  // namespace atomic

#endif  // ATOMIC_THREAD_POLICY_H
