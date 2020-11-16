#ifndef ATOMIC_FORWARD_LIST_HPP
#define ATOMIC_FORWARD_LIST_HPP

#include "forward_list.h"

namespace atomic {

template <class T>
forward_list<T>::forward_list(const T &val) : val(val) {}

template <class T>
T forward_list<T>::get() const {
    return val;
}

}  // namespace atomic

#endif  // ATOMIC_FORWARD_LIST_HPP
