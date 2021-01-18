#ifndef ATOMIC_FORWARD_LIST_H
#define ATOMIC_FORWARD_LIST_H

#include <forward_list>
#include <iterator>
#include <memory>
#include <mutex>

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
    using value_type = T;

    class sector {
    public:
        class iterator : public std::iterator<std::forward_iterator_tag, T> {
        public:
            explicit iterator(std::shared_ptr<node> node);

            iterator& operator++();
            iterator operator++(int);
            bool operator==(const iterator& other) const;
            bool operator!=(const iterator& other) const;
            T& operator*() const;

        private:
            std::shared_ptr<node> _node;
        };

        explicit sector(std::shared_ptr<sector> next);

        void push_front(const T& val);
        void push_front(T&& val);

        void pop_front();

        iterator begin() const;
        iterator end() const;

        void clear();
        std::size_t size() const;
        bool empty() const;

        std::shared_ptr<sector> next() const;

        void lock() const;
        void unlock() const;
        bool try_lock() const;

    private:
        std::shared_ptr<node> _begin;
        std::shared_ptr<node> _end;
        std::size_t _size;

        std::shared_ptr<sector> _next;
        mutable std::mutex _m;
    };

    class sector_iterator;

    class iterator : public std::iterator<std::forward_iterator_tag, T> {
    public:
        explicit iterator(std::shared_ptr<sector> sec);
        iterator(const sector_iterator& it);
        iterator& operator=(const sector_iterator& it);
        ~iterator();
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
        T& operator*();

        const std::shared_ptr<sector>& get_sector() const;
        typename sector::iterator get_sector_it() const;
        typename sector::iterator sector_begin() const;
        typename sector::iterator sector_end() const;

    private:
        std::shared_ptr<sector> _sec;
        typename sector::iterator _sec_it;
    };

    class const_iterator {
    public:
        explicit const_iterator(std::shared_ptr<sector> sec);
        const_iterator(const sector_iterator& it);
        const const_iterator& operator=(const sector_iterator& it);
        const const_iterator& operator++();
        const_iterator operator++(int);
        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;
        const T& operator*() const;

        const std::shared_ptr<sector>& get() const;

    private:
        std::shared_ptr<sector> _sec;
        typename sector::iterator _sec_it;
    };

    class sector_iterator : public std::iterator<std::forward_iterator_tag, T> {
    public:
        explicit sector_iterator(std::shared_ptr<sector> sec);
        sector_iterator(const iterator& it);
        sector_iterator& operator=(const sector_iterator& it);
        sector_iterator& operator++();
        sector_iterator operator++(int);
        bool operator==(const sector_iterator& other) const;
        bool operator!=(const sector_iterator& other) const;
        sector& operator*() const;

        const std::shared_ptr<sector>& get() const;

    private:
        std::shared_ptr<sector> _sec;
    };

    forward_list(std::size_t sector_size = 10);
    forward_list(std::initializer_list<T> list, std::size_t sector_size = 10);
    forward_list(T val, std::size_t count, std::size_t sector_size = 10);

    void clear();
    std::size_t size() const;
    bool empty() const;

    void push_front(const T& val);
    void push_front(T&& val);

    void pop_front();

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    sector_iterator sector_begin() const;
    sector_iterator sector_end() const;

    std::size_t sector_count() const;

private:
    std::shared_ptr<sector> _begin;
    std::shared_ptr<sector> _end;

    const std::size_t _sector_size;
    std::size_t _sector_count;
};

}  // namespace atomic

#endif  // ATOMIC_FORWARD_LIST_H
