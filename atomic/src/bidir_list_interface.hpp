#ifndef BDIR_LIST_INTERFACE
#define BDIR_LIST_INTERFACE
#include <../../boost/interprocess/sync/interprocess_mutex.hpp>
#include <memory>

#include "bdir_list_node.hpp"

namespace atomic_list {

template <class T>

class Atomic_Blist : public boost::interprocess::interprocess_mutex {
public:
    //////////////// iterator ////////////////////////////
    class iterator : public std::iterator<std::bidirectional_iterator_tag, T, T> {
    public:
        explicit iterator(std::shared_ptr<atomic_list::list_node<T>> node) : _node(node) {}
        iterator& operator++() {
            if (_node->prev_elem) {
                _node = _node->prev_elem;
            }
            return *this;
        }
        iterator operator++(int) {
            auto curr_it = iterator(_node);
            if (_node->prev_elem) {
                _node = _node->prev_elem;
            }
            return curr_it;
        }
        iterator& operator--() {
            if (_node->next_elem.lock()) {
                _node = _node->next_elem.lock();
            }
            return *this;
        }
        iterator operator--(int);

        T& operator*() const { return _node->container; }
        bool operator==(iterator other) const { return (_node.get() == other._node.get()); }
        bool operator!=(iterator other) const { return !(*this == other); }

    private:
        std::shared_ptr<atomic_list::list_node<T>> _node;
    };

    //////////////// reverse iterator ////////////////////////////

    class riterator : public std::iterator<std::bidirectional_iterator_tag, T, T> {
    public:
        explicit riterator(std::shared_ptr<atomic_list::list_node<T>> node) : _node(node) {}
        riterator& operator--() {
            if (_node->prev_elem) {
                _node = _node->prev_elem;
            }
            return *this;
        }
        riterator& operator++() {
            if (_node->next_elem.lock()) {
                _node = _node->next_elem.lock();
            }
            return *this;
        }
        riterator operator--(int) {
            auto curr_it = riterator(_node);
            if (_node->prev_elem) {
                _node = _node->prev_elem;
            }
            return curr_it;
        }
        riterator operator++(int) {
            auto curr_it = riterator(_node);
            if (_node->next_elem.lock()) {
                _node = _node->next_elem.lock();
            }
            return curr_it;
        }

        T& operator*() const { return _node->container; }
        bool operator==(riterator other) const { return (_node.get() == other._node.get()); }
        bool operator!=(riterator other) const { return !(*this == other); }

    private:
        std::shared_ptr<atomic_list::list_node<T>> _node;
    };
    //////////////// konstruktor i destruktor ////////////////////////////
    Atomic_Blist() {
        size = 0;
        past_last.reset();
        past_first.reset();
        past_last = std::make_shared<list_node<T>>();
        past_first = std::make_shared<list_node<T>>();
    }
    ~Atomic_Blist() {
        last_elem.reset();
        first_elem.reset();
        past_last.reset();
        past_first.reset();
    }

    //////////////////// POP BACK////////////////////////////

    T pop_back() {
        this->lock();
        T tmp = last_elem.lock()->get();

        if (size > 1) {
            last_elem = last_elem.lock()->next_elem.lock();
            last_elem.lock()->prev_elem = past_last;
            size--;
        } else if (size <= 1) {
            last_elem.reset();
            first_elem.reset();
            size = 0;
        }
        this->unlock();
        return tmp;
    }

    /////////////////// POP FRONT ////////////////////////////

    T pop_front() {
        this->lock();
        T tmp = first_elem->get();
        if (size > 1) {
            first_elem = first_elem->prev_elem;
            first_elem->next_elem = past_first;
            size--;
        } else if (size <= 1) {
            last_elem.reset();
            first_elem.reset();
            size = 0;
        }

        this->unlock();
        return tmp;
    }
    //////////////////// PUSH  BACK////////////////////////////
    void push_back(T new_class) {
        this->lock();
        if (last_elem.lock()) {
            std::shared_ptr<atomic_list::list_node<T>> tmp = std::make_shared<list_node<T>>(new_class);
            last_elem.lock()->prev_elem = tmp;
            tmp->next_elem = last_elem;
            last_elem = tmp;

        } else {
            std::shared_ptr<atomic_list::list_node<T>> tmp = std::make_shared<list_node<T>>(new_class);
            last_elem = tmp;
            first_elem = tmp;
        }
        first_elem->next_elem = past_first;
        last_elem.lock()->prev_elem = past_last;

        size++;
        this->unlock();
    }
    //////////////////////// PUSH FRONT////////////////////////////
    void push_front(T new_class) {
        this->lock();
        if (first_elem) {
            std::shared_ptr<atomic_list::list_node<T>> tmp = std::make_shared<list_node<T>>(new_class);
            tmp->prev_elem = first_elem;
            first_elem->next_elem = tmp;
            first_elem = tmp;
        } else {
            std::shared_ptr<atomic_list::list_node<T>> tmp = std::make_shared<list_node<T>>(new_class);
            last_elem = tmp;
            first_elem = tmp;
        }
        first_elem->next_elem = past_first;
        last_elem.lock()->prev_elem = past_last;

        size++;
        this->unlock();
    }

    // ITERATORY

    iterator begin() { return iterator(first_elem); }
    riterator rbegin() { return riterator(last_elem.lock()); }
    iterator end() { return iterator(past_last); }
    riterator rend() { return riterator(past_first); }

    void clear() {
        this->lock();
        last_elem.reset();
        first_elem.reset();
        this->unlock();
    }

private:
    std::shared_ptr<atomic_list::list_node<T>> first_elem;
    std::weak_ptr<atomic_list::list_node<T>> last_elem;

    std::shared_ptr<atomic_list::list_node<T>> past_last;
    std::shared_ptr<atomic_list::list_node<T>> past_first;

    int size;
    int elem_iterator;
};

}  // namespace atomic_list

#endif