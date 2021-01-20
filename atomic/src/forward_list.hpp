#ifndef ATOMIC_FORWARD_LIST_HPP
#define ATOMIC_FORWARD_LIST_HPP

#include "forward_list.h"

namespace atomic {

template <class T, typename lock_policy>
forward_list<T, lock_policy>::node::node(std::shared_ptr<node> next) : _next(std::move(next)) {}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::node::node(T val, std::shared_ptr<node> next) : _val(val), _next(std::move(next)) {}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::sector::sector(std::shared_ptr<sector> next)
    : _begin(std::make_shared<node>(nullptr)), _end(_begin), _size(0), _next(next) {}

template <class T, typename lock_policy>
void forward_list<T, lock_policy>::sector::push_front(const T& val) {
    auto _node = std::make_shared<node>(val, _begin);
    _m.lock();
    _begin = _node;
    ++_size;
    _m.unlock();
}

template <class T, typename lock_policy>
void forward_list<T, lock_policy>::sector::push_front(T&& val) {
    auto _node = std::__make_shared<node>(std::move(val), _begin);
    _m.lock();
    _begin = _node;
    ++_size;
    _m.unlock();
}

template <class T, typename lock_policy>
void forward_list<T, lock_policy>::sector::pop_front() {
    if (_begin == _end) {
        return;
    }
    _m.lock();
    _begin = _begin->_next;
    --_size;
    _m.unlock();
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector::iterator forward_list<T, lock_policy>::sector::begin() {
    return iterator(_begin, *this);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector::const_iterator forward_list<T, lock_policy>::sector::begin() const {
    return const_iterator(_begin);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector::const_iterator forward_list<T, lock_policy>::sector::cbegin() const {
    return const_iterator(_begin);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector::iterator forward_list<T, lock_policy>::sector::end() {
    return iterator(_end, *this);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector::const_iterator forward_list<T, lock_policy>::sector::end() const {
    return const_iterator(_end);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector::const_iterator forward_list<T, lock_policy>::sector::cend() const {
    return const_iterator(_end);
}

template <class T, typename lock_policy>
void forward_list<T, lock_policy>::sector::clear() {
    _m.lock();
    _begin = _end;
    _size = 0;
    _m.unlock();
}

template <class T, typename lock_policy>
std::size_t forward_list<T, lock_policy>::sector::size() const {
    return _size;
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::sector::empty() const {
    return _size == 0;
}

template <class T, typename lock_policy>
std::shared_ptr<typename forward_list<T, lock_policy>::sector> forward_list<T, lock_policy>::sector::next() const {
    return _next;
}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::sector::iterator::iterator(std::shared_ptr<node> node, sector& parent)
    : _node(node), _parent(&parent) {
    if (_parent->_next) {
        //_parent->_m.lock();
        lock_policy::execute(_parent->_m);
    }
}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::sector::iterator::iterator(iterator&& it)
    : _node(std::move(it._node)), _parent(it._parent) {}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::sector::iterator::~iterator() {
    _parent->_m.unlock();
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector::iterator& forward_list<T, lock_policy>::sector::iterator::operator=(
    iterator&& it) {
    _parent->_m.unlock();

    _node = std::move(it._node);
    _parent = it._parent;

    return *this;
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector::iterator& forward_list<T, lock_policy>::sector::iterator::operator++() {
    if (_node->_next) {
        _node = _node->_next;
    }
    return *this;
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::sector::iterator::operator==(
    const forward_list<T, lock_policy>::sector::iterator& other) const {
    return _node.get() == other._node.get();
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::sector::iterator::operator==(
    const forward_list<T, lock_policy>::sector::const_iterator& other) const {
    return _node.get() == other.get().get();
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::sector::iterator::operator!=(
    const forward_list<T, lock_policy>::sector::iterator& other) const {
    return !(*this == other);
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::sector::iterator::operator!=(
    const forward_list<T, lock_policy>::sector::const_iterator& other) const {
    return !(*this == other);
}

template <class T, typename lock_policy>
T& forward_list<T, lock_policy>::sector::iterator::operator*() const {
    return _node->_val;
}

template <class T, typename lock_policy>
const std::shared_ptr<typename forward_list<T, lock_policy>::node> forward_list<T, lock_policy>::sector::iterator::get()
    const {
    return _node;
}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::sector::const_iterator::const_iterator(std::shared_ptr<node> node) : _node(node) {}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::sector::const_iterator::const_iterator(const iterator& it) : _node(it.get()) {}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector::const_iterator&
forward_list<T, lock_policy>::sector::const_iterator::operator=(const const_iterator& it) {
    _node = it._node;
    return *this;
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector::const_iterator&
forward_list<T, lock_policy>::sector::const_iterator::operator++() {
    if (_node->_next) {
        _node = _node->_next;
    }
    return *this;
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector::const_iterator
forward_list<T, lock_policy>::sector::const_iterator::operator++(int) {
    auto curr_it = const_iterator(_node);
    ++(*this);
    return curr_it;
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::sector::const_iterator::operator==(const const_iterator& other) const {
    return _node.get() == other._node.get();
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::sector::const_iterator::operator==(const iterator& other) const {
    return _node.get() == other.get().get();
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::sector::const_iterator::operator!=(const const_iterator& other) const {
    return !(*this == other);
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::sector::const_iterator::operator!=(const iterator& other) const {
    return !(*this == other);
}

template <class T, typename lock_policy>
const T& forward_list<T, lock_policy>::sector::const_iterator::operator*() const {
    return _node->_val;
}

template <class T, typename lock_policy>
const std::shared_ptr<typename forward_list<T, lock_policy>::node>
forward_list<T, lock_policy>::sector::const_iterator::get() const {
    return _node;
}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::forward_list(std::size_t sector_size)
    : _begin(std::make_shared<sector>(nullptr)), _end(_begin), _sector_size(sector_size), _sector_count(0) {}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::forward_list(std::initializer_list<T> list, std::size_t sector_size)
    : _begin(std::make_shared<sector>(nullptr)), _end(_begin), _sector_size(sector_size), _sector_count(0) {
    for (auto it = list.end() - 1; it != list.begin(); --it) {
        push_front(*it);
    }
    push_front(*list.begin());
}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::forward_list(T val, std::size_t count, std::size_t sector_size)
    : _begin(std::make_shared<sector>(nullptr)), _end(_begin), _sector_size(sector_size), _sector_count(0) {
    for (std::size_t i = 0; i < count; ++i) {
        push_front(val);
    }
}

template <class T, typename lock_policy>
void forward_list<T, lock_policy>::clear() {
    for (auto sec = sector_begin(); sec != sector_end(); ++sec) {
        (*sec).clear();
    }
    _begin = _end;
    _sector_count = 0;
}

template <class T, typename lock_policy>
std::size_t forward_list<T, lock_policy>::size() const {
    std::size_t _size = 0;
    for (auto _sec = _begin; _sec.get() != _end.get(); _sec = _sec->next()) {
        _size += _sec->size();
    }

    return _size;
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::empty() const {
    return size() == 0;
}

template <class T, typename lock_policy>
void forward_list<T, lock_policy>::push_front(const T& val) {
    if (_begin->size() >= _sector_size || _begin == _end) {
        auto _sec = std::make_shared<sector>(_begin);
        _begin = _sec;
        ++_sector_count;
    }
    _begin->push_front(val);
}

template <class T, typename lock_policy>
void forward_list<T, lock_policy>::push_front(T&& val) {
    if (_begin->size() >= _sector_size || _begin == _end) {
        auto _sec = std::make_shared<sector>(_begin);
        _begin = _sec;
        ++_sector_count;
    }
    _begin->push_front(val);
}

template <class T, typename lock_policy>
void forward_list<T, lock_policy>::pop_front() {
    if (_begin == _end) {
        return;
    }
    _begin->pop_front();
    if (_begin->empty()) {
        _begin = _begin->next();
        --_sector_count;
    }
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::iterator forward_list<T, lock_policy>::begin() {
    return iterator(_begin);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::const_iterator forward_list<T, lock_policy>::begin() const {
    return const_iterator(_begin);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::const_iterator forward_list<T, lock_policy>::cbegin() const {
    return const_iterator(_begin);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::iterator forward_list<T, lock_policy>::end() {
    return iterator(_end);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::const_iterator forward_list<T, lock_policy>::end() const {
    return const_iterator(_end);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::const_iterator forward_list<T, lock_policy>::cend() const {
    return const_iterator(_end);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector_iterator forward_list<T, lock_policy>::sector_begin() const {
    return sector_iterator(_begin);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector_iterator forward_list<T, lock_policy>::sector_end() const {
    return sector_iterator(_end);
}

template <class T, typename lock_policy>
std::size_t forward_list<T, lock_policy>::sector_count() const {
    return _sector_count;
}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::iterator::iterator(std::shared_ptr<sector> sec) : _sec(sec), _sec_it(_sec->begin()) {}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::iterator::iterator(iterator&& it)
    : _sec(std::move(it._sec)), _sec_it(std::forward<typename sector::iterator>(it._sec_it)) {}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::iterator::iterator(const sector_iterator& it) : _sec(it.get()), _sec_it(_sec->begin()) {}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::iterator::~iterator() {}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::iterator& forward_list<T, lock_policy>::iterator::operator=(iterator&& it) {
    _sec = std::move(it._sec);
    _sec_it = std::move(it._sec_it);
    return *this;
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::iterator& forward_list<T, lock_policy>::iterator::operator=(
    const sector_iterator& it) {
    _sec = it.get();
    _sec_it = std::move(_sec->begin());
    return *this;
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::iterator& forward_list<T, lock_policy>::iterator::operator++() {
    ++_sec_it;
    if (_sec_it == _sec->cend() && _sec->next()) {
        _sec = _sec->next();
        _sec_it = std::move(_sec->begin());
    }
    return *this;
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::iterator::operator==(const iterator& other) const {
    return _sec_it == other._sec_it;
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::iterator::operator!=(const iterator& other) const {
    return !(*this == other);
}

template <class T, typename lock_policy>
T& forward_list<T, lock_policy>::iterator::operator*() {
    return *_sec_it;
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::iterator::is_sector_end() const {
    typename forward_list<T, lock_policy>::sector::const_iterator c_it(_sec_it);
    ++c_it;
    return c_it == _sec->cend();
}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::const_iterator::const_iterator(std::shared_ptr<sector> sec)
    : _sec(sec), _sec_it(_sec->cbegin()) {}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::const_iterator::const_iterator(const const_iterator& it)
    : _sec(it._sec), _sec_it(it._sec_it) {}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::const_iterator::const_iterator(const sector_iterator& it)
    : _sec(it.get()), _sec_it(_sec->cbegin()) {}

template <class T, typename lock_policy>
const typename forward_list<T, lock_policy>::const_iterator& forward_list<T, lock_policy>::const_iterator::operator=(
    const sector_iterator& it) {
    _sec = it.get();
    _sec_it = _sec->begin();
    return *this;
}

template <class T, typename lock_policy>
const typename forward_list<T, lock_policy>::const_iterator&
forward_list<T, lock_policy>::const_iterator::operator++() {
    ++_sec_it;
    if (_sec_it == _sec->cend() && _sec->next()) {
        _sec = _sec->next();
        _sec_it = _sec->cbegin();
    }
    return *this;
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::const_iterator forward_list<T, lock_policy>::const_iterator::operator++(int) {
    auto _curr_sec = _sec;
    auto _curr_sec_it = _sec_it;

    operator++();

    const_iterator _prev_it(_curr_sec);
    _prev_it._sec_it = _curr_sec_it;

    return _prev_it;
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::const_iterator::operator==(const const_iterator& other) const {
    return _sec_it == other._sec_it;
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::const_iterator::operator!=(const const_iterator& other) const {
    return !(*this == other);
}

template <class T, typename lock_policy>
const T& forward_list<T, lock_policy>::const_iterator::operator*() const {
    return *_sec_it;
}

template <class T, typename lock_policy>
const std::shared_ptr<typename forward_list<T, lock_policy>::sector>&
forward_list<T, lock_policy>::const_iterator::get() const {
    return _sec;
}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::sector_iterator::sector_iterator(std::shared_ptr<sector> sec) : _sec(sec) {}

template <class T, typename lock_policy>
forward_list<T, lock_policy>::sector_iterator::sector_iterator(const iterator& it) {
    _sec = it.get_sector();
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector_iterator& forward_list<T, lock_policy>::sector_iterator::operator=(
    const sector_iterator& it) {
    _sec = it.get();
    return *this;
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector_iterator& forward_list<T, lock_policy>::sector_iterator::operator++() {
    if (_sec->next()) {
        _sec = _sec->next();
    }
    return *this;
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector_iterator forward_list<T, lock_policy>::sector_iterator::operator++(int) {
    auto _curr_sec = sector_iterator(_sec);
    ++(*this);
    return _curr_sec;
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::sector_iterator::operator==(const sector_iterator& other) const {
    return _sec.get() == other._sec.get();
}

template <class T, typename lock_policy>
bool forward_list<T, lock_policy>::sector_iterator::operator!=(const sector_iterator& other) const {
    return !(*this == other);
}

template <class T, typename lock_policy>
typename forward_list<T, lock_policy>::sector& forward_list<T, lock_policy>::sector_iterator::operator*() const {
    return *_sec;
}

template <class T, typename lock_policy>
const std::shared_ptr<typename forward_list<T, lock_policy>::sector>&
forward_list<T, lock_policy>::sector_iterator::get() const {
    return _sec;
}

}  // namespace atomic

#endif  // ATOMIC_FORWARD_LIST_HPP
