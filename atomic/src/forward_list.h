#ifndef ATOMIC_FORWARD_LIST_H
#define ATOMIC_FORWARD_LIST_H

#include <forward_list>
#include <iterator>
#include <memory>

namespace atomic {

template <class T>
class forward_list {
    struct node {
    public:
        explicit node(std::shared_ptr<node> next);
        explicit node(T val, std::shared_ptr<node> next);

        T _val;
        std::shared_ptr<node> _next;
    };

public:
    class iterator : public std::iterator<std::forward_iterator_tag, T> {
    public:
        explicit iterator(std::shared_ptr<node> node);
        iterator& operator++();
        iterator operator++(int);
        bool operator==(iterator other) const;
        bool operator!=(iterator other) const;
        T& operator*() const;

    private:
        std::shared_ptr<node> _node;
    };

    class const_iterator : public std::iterator<std::forward_iterator_tag, T, T,
                                                const T*, const T&> {
    public:
        explicit const_iterator(std::shared_ptr<node> node);
        const_iterator& operator++();
        const_iterator operator++(int);
        bool operator==(const_iterator other) const;
        bool operator!=(const_iterator other) const;
        const T& operator*() const;

    private:
        std::shared_ptr<node> _node;
    };

    forward_list();
    // forward_list(std::initializer_list<T> list);

    void clear();
    std::size_t size() const;
    bool empty() const;

    void push_front(const T& val);
    void push_front(T&& val);

    iterator begin() const;
    iterator end() const;

private:
    std::shared_ptr<node> _begin;
    std::shared_ptr<node> _end;

    std::size_t _size;
};

}  // namespace atomic

#endif  // ATOMIC_FORWARD_LIST_H
