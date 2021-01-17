#ifndef ATOMIC_FORWARD_LIST_HPP
#define ATOMIC_FORWARD_LIST_HPP

#include "forward_list.h"

namespace atomic {

template <class T>
forward_list<T>::node::node(std::shared_ptr<node> next) : _next(std::move(next)) {}

template <class T>
forward_list<T>::node::node(T val, std::shared_ptr<node> next) : _val(val), _next(std::move(next)) {}

template <class T>
forward_list<T>::sector::sector(std::shared_ptr<sector> next)
    : _begin(std::make_shared<node>(nullptr)), _end(_begin), _size(0), _next(next) {}

template <class T>
void forward_list<T>::sector::push_front(const T& val) {
    auto _node = std::make_shared<node>(val, _begin);
    _m.lock();
    _begin = _node;
    ++_size;
    _m.unlock();
}

template <class T>
void forward_list<T>::sector::push_front(T&& val) {
    auto _node = std::__make_shared<node>(std::move(val), _begin);
    _m.lock();
    _begin = _node;
    ++_size;
    _m.unlock();
}

template <class T>
void forward_list<T>::sector::pop_front() {
    if (_begin == _end) {
        return;
    }
    _m.lock();
    _begin = _begin->_next;
    --_size;
    _m.unlock();
}

template <class T>
typename forward_list<T>::sector::iterator forward_list<T>::sector::begin() const {
    return iterator(_begin);
}

template <class T>
typename forward_list<T>::sector::iterator forward_list<T>::sector::end() const {
    return iterator(_end);
}

template <class T>
std::size_t forward_list<T>::sector::size() const {
    return _size;
}

template <class T>
bool forward_list<T>::sector::empty() const {
    return _size == 0;
}

template <class T>
std::shared_ptr<typename forward_list<T>::sector> forward_list<T>::sector::next() const {
    return _next;
}

template <class T>
void forward_list<T>::sector::lock() const {
    _m.lock();
}

template <class T>
void forward_list<T>::sector::unlock() const {
    _m.unlock();
}

template <class T>
forward_list<T>::sector::iterator::iterator(std::shared_ptr<node> node) : _node(node) {}

template <class T>
typename forward_list<T>::sector::iterator& forward_list<T>::sector::iterator::operator++() {
    if (_node->_next) {
        _node = _node->_next;
    }
    return *this;
}

template <class T>
typename forward_list<T>::sector::iterator forward_list<T>::sector::iterator::operator++(int) {
    auto curr_it = iterator(_node);
    ++(*this);
    return curr_it;
}

template <class T>
bool forward_list<T>::sector::iterator::operator==(const forward_list<T>::sector::iterator& other) const {
    return _node.get() == other._node.get();
}

template <class T>
bool forward_list<T>::sector::iterator::operator!=(const forward_list<T>::sector::iterator& other) const {
    return !(*this == other);
}

template <class T>
T& forward_list<T>::sector::iterator::operator*() const {
    return _node->_val;
}

template <class T>
forward_list<T>::forward_list(std::size_t sector_size)
    : _begin(std::make_shared<sector>(nullptr)), _end(_begin), _sector_size(sector_size), _sector_count(0) {}

template <class T>
forward_list<T>::forward_list(std::initializer_list<T> list, std::size_t sector_size)
    : _begin(std::make_shared<sector>(nullptr)), _end(_begin), _sector_size(sector_size), _sector_count(0) {
    for (auto it = list.end() - 1; it != list.begin(); --it) {
        push_front(*it);
    }
    push_front(*list.begin());
}

template <class T>
void forward_list<T>::clear() {
    for (auto sec = sector_begin(); sec != sector_end(); ++sec) {
        (*sec).lock();
    }
    _begin = _end;
    _sector_count = 0;
}

template <class T>
std::size_t forward_list<T>::size() const {
    std::size_t _size = 0;
    for (auto _sec = _begin; _sec.get() != _end.get(); _sec = _sec->next()) {
        _size += _sec->size();
    }

    return _size;
}

template <class T>
bool forward_list<T>::empty() const {
    return size() == 0;
}

template <class T>
void forward_list<T>::push_front(const T& val) {
    if (_begin->size() >= _sector_size || _begin == _end) {
        auto _sec = std::make_shared<sector>(_begin);
        _begin = _sec;
        ++_sector_count;
    }
    _begin->push_front(val);
}

template <class T>
void forward_list<T>::push_front(T&& val) {
    if (_begin->size() >= _sector_size || _begin == _end) {
        auto _sec = std::make_shared<sector>(_begin);
        _begin = _sec;
        ++_sector_count;
    }
    _begin->push_front(val);
}

template <class T>
void forward_list<T>::pop_front() {
    if (_begin == _end) {
        return;
    }
    _begin->pop_front();
    if (_begin->empty()) {
        _begin = _begin->next();
        --_sector_count;
    }
}

template <class T>
typename forward_list<T>::iterator forward_list<T>::begin() {
    return iterator(_begin);
}

template <class T>
typename forward_list<T>::const_iterator forward_list<T>::begin() const {
    return const_iterator(_begin);
}

template <class T>
typename forward_list<T>::iterator forward_list<T>::end() {
    return iterator(_end);
}

template <class T>
typename forward_list<T>::const_iterator forward_list<T>::end() const {
    return const_iterator(_end);
}

template <class T>
typename forward_list<T>::sector_iterator forward_list<T>::sector_begin() const {
    return sector_iterator(_begin);
}

template <class T>
typename forward_list<T>::sector_iterator forward_list<T>::sector_end() const {
    return sector_iterator(_end);
}

template <class T>
std::size_t forward_list<T>::sector_count() const {
    return _sector_count;
}

template <class T>
forward_list<T>::iterator::iterator(std::shared_ptr<sector> sec) : _sec(sec), _sec_it(_sec->begin()) {
    _sec->lock();
}

template <class T>
forward_list<T>::iterator::~iterator() {
    _sec->unlock();
}

template <class T>
forward_list<T>::iterator::iterator(const sector_iterator& it) : _sec(it.get()), _sec_it(_sec->begin()) {
    _sec->lock();
}

template <class T>
typename forward_list<T>::iterator& forward_list<T>::iterator::operator=(const sector_iterator& it) {
    _sec->unlock();
    _sec = it.get();
    _sec->lock();
    _sec_it = _sec->begin();
    return *this;
}

template <class T>
typename forward_list<T>::iterator& forward_list<T>::iterator::operator++() {
    _sec_it++;
    if (_sec_it == _sec->end() && _sec->next()) {
        _sec->unlock();
        _sec = _sec->next();
        _sec->lock();
        _sec_it = _sec->begin();
    }
    return *this;
}

template <class T>
typename forward_list<T>::iterator forward_list<T>::iterator::operator++(int) {
    auto _curr_sec = _sec;
    auto _curr_sec_it = _sec_it;

    operator++();

    if (_curr_sec == _sec) {
        throw std::runtime_error("cannot create two iterators on the same sector in single thread (thread lock error)");
    }

    auto _prev_it = iterator(_curr_sec);
    _prev_it._sec_it = _curr_sec_it;

    return _prev_it;
}

template <class T>
bool forward_list<T>::iterator::operator==(const iterator& other) const {
    return _sec_it == other._sec_it;
}

template <class T>
bool forward_list<T>::iterator::operator!=(const iterator& other) const {
    return !(*this == other);
}

template <class T>
T& forward_list<T>::iterator::operator*() {
    return *_sec_it;
}

template <class T>
const std::shared_ptr<typename forward_list<T>::sector>& forward_list<T>::iterator::get() const {
    return _sec;
}

template <class T>
forward_list<T>::const_iterator::const_iterator(std::shared_ptr<sector> sec) : _sec(sec), _sec_it(_sec->begin()) {}

template <class T>
forward_list<T>::const_iterator::const_iterator(const sector_iterator& it) : _sec(it.get()), _sec_it(_sec->begin()) {}

template <class T>
const typename forward_list<T>::const_iterator& forward_list<T>::const_iterator::operator=(const sector_iterator& it) {
    _sec = it.get();
    _sec_it = _sec->begin();
    return *this;
}

template <class T>
const typename forward_list<T>::const_iterator& forward_list<T>::const_iterator::operator++() {
    ++_sec_it;
    if (_sec_it == _sec->end() && _sec->next()) {
        _sec = _sec->next();
        _sec_it = _sec->begin();
    }
    return *this;
}

template <class T>
typename forward_list<T>::const_iterator forward_list<T>::const_iterator::operator++(int) {
    auto _curr_sec = _sec;
    auto _curr_sec_it = _sec_it;

    operator++();

    const_iterator _prev_it(_curr_sec);
    _prev_it._sec_it = _curr_sec_it;

    return _prev_it;
}

template <class T>
bool forward_list<T>::const_iterator::operator==(const const_iterator& other) const {
    return _sec_it == other._sec_it;
}

template <class T>
bool forward_list<T>::const_iterator::operator!=(const const_iterator& other) const {
    return !(*this == other);
}

template <class T>
const T& forward_list<T>::const_iterator::operator*() const {
    return *_sec_it;
}

template <class T>
const std::shared_ptr<typename forward_list<T>::sector>& forward_list<T>::const_iterator::get() const {
    return _sec;
}

template <class T>
forward_list<T>::sector_iterator::sector_iterator(std::shared_ptr<sector> sec) : _sec(sec) {}

template <class T>
forward_list<T>::sector_iterator::sector_iterator(const iterator& it) {
    _sec = it.get();
}

template <class T>
typename forward_list<T>::sector_iterator& forward_list<T>::sector_iterator::operator=(const sector_iterator& it) {
    _sec = it.get();
}

template <class T>
typename forward_list<T>::sector_iterator& forward_list<T>::sector_iterator::operator++() {
    if (_sec->next()) {
        _sec = _sec->next();
    }
    return *this;
}

template <class T>
typename forward_list<T>::sector_iterator forward_list<T>::sector_iterator::operator++(int) {
    auto _curr_sec = sector_iterator(_sec);
    ++(*this);
    return _curr_sec;
}

template <class T>
bool forward_list<T>::sector_iterator::operator==(const sector_iterator& other) const {
    return _sec.get() == other._sec.get();
}

template <class T>
bool forward_list<T>::sector_iterator::operator!=(const sector_iterator& other) const {
    return !(*this == other);
}

template <class T>
typename forward_list<T>::sector& forward_list<T>::sector_iterator::operator*() const {
    return *_sec;
}

template <class T>
const std::shared_ptr<typename forward_list<T>::sector>& forward_list<T>::sector_iterator::get() const {
    return _sec;
}

}  // namespace atomic

#endif  // ATOMIC_FORWARD_LIST_HPP
