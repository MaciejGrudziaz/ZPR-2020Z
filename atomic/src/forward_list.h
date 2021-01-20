#ifndef ATOMIC_FORWARD_LIST_H
#define ATOMIC_FORWARD_LIST_H

#include <memory>
#include <mutex>
#include <optional>

#include "thread_policy.h"

namespace atomic {

template <class T, typename lock_policy = exception_policy>
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
        class const_iterator;

        class iterator {
        public:
            explicit iterator(std::shared_ptr<node> node, sector& parent);
            ~iterator();
            iterator& operator=(iterator&& it);

            iterator& operator++();
            // iterator operator++(int);
            bool operator==(const iterator& other) const;
            bool operator==(const const_iterator& other) const;
            bool operator!=(const iterator& other) const;
            bool operator!=(const const_iterator& other) const;
            T& operator*() const;

            const std::shared_ptr<node> get() const;

        private:
            std::shared_ptr<node> _node;
            sector* _parent;
        };

        class const_iterator {
        public:
            explicit const_iterator(std::shared_ptr<node> node);
            const_iterator(const iterator& it);
            const_iterator& operator=(const const_iterator& it);

            const_iterator& operator++();
            const_iterator operator++(int);
            bool operator==(const const_iterator& other) const;
            bool operator==(const iterator& other) const;
            bool operator!=(const const_iterator& other) const;
            bool operator!=(const iterator& other) const;
            const T& operator*() const;

            const std::shared_ptr<node> get() const;

        private:
            std::shared_ptr<node> _node;
        };

        explicit sector(std::shared_ptr<sector> next);
        ~sector();

        void push_front(const T& val);
        void push_front(T&& val);

        void pop_front();

        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;

        iterator end();
        const_iterator end() const;
        const_iterator cend() const;

        void clear();
        std::size_t size() const;
        bool empty() const;

        std::shared_ptr<sector> next() const;

        //        void lock() const;
        //        void unlock() const;
        //        bool try_lock() const;

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
        iterator(iterator&& it);
        iterator(const sector_iterator& it);
        iterator& operator=(iterator&& it);
        iterator& operator=(const sector_iterator& it);
        ~iterator();
        iterator& operator++();
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
        T& operator*();

        bool is_sector_end() const;

    private:
        std::shared_ptr<sector> _sec;
        typename sector::iterator _sec_it;
    };

    class const_iterator : public std::iterator<std::forward_iterator_tag, T, T, const T*, const T&> {
    public:
        explicit const_iterator(std::shared_ptr<sector> sec);
        const_iterator(const const_iterator& it);
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
        typename sector::const_iterator _sec_it;
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
    ~forward_list();

    void clear();
    std::size_t size() const;
    bool empty() const;

    void push_front(const T& val);
    void push_front(T&& val);

    void pop_front();

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

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
