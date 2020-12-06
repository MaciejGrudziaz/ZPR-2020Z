#ifndef ATOMIC_FORWARD_LIST_HPP
#define ATOMIC_FORWARD_LIST_HPP

#include "forward_list.h"

namespace atomic {

template <class T>
forward_list<T>::node::node(std::shared_ptr<node> next)
    : _next(std::move(next)) {}

template <class T>
forward_list<T>::node::node(T val, std::shared_ptr<node> next)
    : _val(val), _next(std::move(next)) {}

template <class T>
forward_list<T>::forward_list()
    : _begin(std::make_shared<node>(nullptr)), _end(_begin), _size(0) {}

template <class T>
void forward_list<T>::clear() {
    _begin = _end;
    _size = 0;
}

template <class T>
std::size_t forward_list<T>::size() const {
    return _size;
}

template <class T>
bool forward_list<T>::empty() const {
    return _size == 0;
}

template <class T>
void forward_list<T>::push_front(const T& val) {
    auto _node = std::make_shared<node>(val, _begin);
    _begin = _node;
    ++_size;
}

template <class T>
void forward_list<T>::push_front(T&& val) {
    auto _node = std::make_shared<node>(std::move(val), _begin);
    _begin = _node;
    ++_size;
}

template <class T>
typename forward_list<T>::iterator forward_list<T>::begin() const {
    return iterator(_begin);
}

template <class T>
typename forward_list<T>::iterator forward_list<T>::end() const {
    return iterator(_end);
}

template <class T>
forward_list<T>::iterator::iterator(std::shared_ptr<node> node) : _node(node) {}

template <class T>
typename forward_list<T>::iterator& forward_list<T>::iterator::operator++() {
    if (_node->_next) {
        _node = _node->_next;
    }
    return *this;
}

template <class T>
typename forward_list<T>::iterator forward_list<T>::iterator::operator++(int) {
    auto curr_it = iterator(_node);
    if (_node->_next) {
        _node = _node->next;
    }
    return curr_it;
}

template <class T>
bool forward_list<T>::iterator::operator==(iterator other) const {
    return _node.get() == other._node.get();
}

template <class T>
bool forward_list<T>::iterator::operator!=(iterator other) const {
    return !(*this == other);
}

template <class T>
T& forward_list<T>::iterator::operator*() const {
    return _node->_val;
}

template <class T>
forward_list<T>::const_iterator::const_iterator(std::shared_ptr<node> node)
    : _node(node) {}

template <class T>
typename forward_list<T>::const_iterator&
forward_list<T>::const_iterator::operator++() {
    if (_node->_next) {
        _node = _node->_next;
    }
    return *this;
}

template <class T>
typename forward_list<T>::const_iterator
forward_list<T>::const_iterator::operator++(int) {
    auto curr_it = const_iterator(_node);
    if (_node->_next) {
        _node = _node->next;
    }
    return curr_it;
}

template <class T>
bool forward_list<T>::const_iterator::operator==(const_iterator other) const {
    return _node.get() == other._node.get();
}

template <class T>
bool forward_list<T>::const_iterator::operator!=(const_iterator other) const {
    return !(*this == other);
}

template <class T>
const T& forward_list<T>::const_iterator::operator*() const {
    return _node->_val;
}

}  // namespace atomic

#endif  // ATOMIC_FORWARD_LIST_HPP
