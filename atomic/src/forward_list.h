#ifndef ATOMIC_FORWARD_LIST_H
#define ATOMIC_FORWARD_LIST_H

namespace atomic {

template <class T>
class forward_list {
public:
    forward_list(const T &val);
    T get() const;

private:
    T val;
};

}  // namespace atomic

#endif  // ATOMIC_FORWARD_LIST_H
