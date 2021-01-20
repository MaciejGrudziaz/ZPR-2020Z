#ifndef BDIR_LIST_INTERFACE
#define BDIR_LIST_INTERFACE
#include <atomic>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <memory>

#include "Monitor.hpp"
#include "bdir_list_node.hpp"

namespace atomic_list {

template <class T>

class Atomic_Blist : public Monitor {
public:
    //////////////// iterator ////////////////////////////
    class iterator : public std::iterator<std::bidirectional_iterator_tag, T, T> {
    public:
        iterator(std::shared_ptr<atomic_list::list_node<T>> node, std::atomic<int>* reader_counter,
                 Atomic_Blist* parent)
            : _node(node) {
            parent_ = parent;
            _reader_counter = reader_counter;
            _reader_counter->fetch_add(1);

            //    std::cout<<"Ctor"<<std::endl;
            //    std::cout.flush();
            parent_->leave();
        }
        ~iterator() {
            parent_->enter();

            //   std::cout<<"Dtor"<<std::endl;
            //    std::cout.flush();

            _reader_counter->fetch_sub(1);
            if (parent_->no_of_active_readers.load() == 0 && parent_->no_of_waiting_writers.load() > 0) {
                parent_->signal(parent_->mozna_pisac);
            }
            if (parent_->no_of_active_readers.load() > 0) {
                parent_->signal(parent_->mozna_czytac);
            }

            parent_->leave();
        }
        iterator& operator++() {
            if (_node->prev_elem) {
                _node = _node->prev_elem;
            }
            return *this;
        }

        iterator& operator--() {
            if (_node->next_elem.lock()) {
                _node = _node->next_elem.lock();
            }
            return *this;
        }

        T& operator*() const { return _node->container; }
        bool operator==(const iterator& other) const { return (_node.get() == other._node.get()); }
        bool operator!=(const iterator& other) const { return !(_node.get() == other._node.get()); }

    private:
        std::shared_ptr<atomic_list::list_node<T>> _node;
        std::atomic<int>* _reader_counter;
        Atomic_Blist* parent_;
    };

    //////////////// reverse iterator ////////////////////////////

    class riterator : public std::iterator<std::bidirectional_iterator_tag, T, T> {
    public:
        riterator(std::shared_ptr<atomic_list::list_node<T>> node, std::atomic<int>* reader_counter,
                  Atomic_Blist* parent)
            : _node(node) {
            parent_ = parent;
            _reader_counter = reader_counter;
            _reader_counter->fetch_add(1);

            //    std::cout<<"Ctor"<<std::endl;
            // std::cout.flush();
            parent_->leave();
        }
        ~riterator() {
            parent_->enter();

            //    std::cout<<"Dtor"<<std::endl;
            //   std::cout.flush();

            _reader_counter->fetch_sub(1);
            if (parent_->no_of_active_readers.load() == 0 && parent_->no_of_waiting_writers.load() > 0) {
                parent_->signal(parent_->mozna_pisac);
            }
            if (parent_->no_of_active_readers.load() > 0) {
                parent_->signal(parent_->mozna_czytac);
            }

            parent_->leave();
        }
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

        T& operator*() const { return _node->container; }
        bool operator==(riterator other) const { return (_node.get() == other._node.get()); }
        bool operator!=(riterator other) const { return !(*this == other); }

    private:
        std::shared_ptr<atomic_list::list_node<T>> _node;
        std::atomic<int>* _reader_counter;
        Atomic_Blist* parent_;
    };
    //////////////// konstruktor i destruktor ////////////////////////////
    Atomic_Blist() {
        size = 0;
        no_of_active_readers.store(0);
        past_last.reset();
        past_first.reset();
        past_last = std::make_shared<list_node<T>>();
        past_first = std::make_shared<list_node<T>>();
        someone_is_writing = 0;
    }
    ~Atomic_Blist() {
        last_elem.reset();
        first_elem.reset();
        past_last.reset();
        past_first.reset();
    }

    void wait_for_iterators_to_finnish() {
        int tmp;
        reading_mutex.try_lock();
        do {
            tmp = no_of_active_readers.load();
            writing_mutex.unlock();
            writing_mutex.lock();
            //            std::cout<<"no of active readers= "<<tmp<<std::endl;
        } while (tmp > 0);
    }

    //////////////////// POP BACK////////////////////////////

    T pop_back() {
        enter();
        if (no_of_active_readers.load() > 0) {
            no_of_waiting_writers.fetch_add(1);
            wait(mozna_pisac);
            no_of_waiting_writers.fetch_add(-1);
        }
        someone_is_writing = 1;

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
        someone_is_writing = 0;
        if (no_of_waiting_readers.load() > 0) signal(mozna_czytac);
        if (no_of_waiting_writers.load() > 0) signal(mozna_pisac);
        leave();
        return tmp;
    }

    /////////////////// POP FRONT ////////////////////////////

    T pop_front() {
        enter();
        if (no_of_active_readers.load() > 0) {
            no_of_waiting_writers.fetch_add(1);
            wait(mozna_pisac);
            no_of_waiting_writers.fetch_add(-1);
        }
        someone_is_writing = 1;
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
        someone_is_writing = 0;
        if (no_of_waiting_readers.load() > 0) signal(mozna_czytac);
        if (no_of_waiting_writers.load() > 0) signal(mozna_pisac);
        leave();
        return tmp;
    }
    //////////////////// PUSH  BACK////////////////////////////
    void push_back(T new_class) {
        enter();
        if (no_of_active_readers.load() > 0) {
            no_of_waiting_writers.fetch_add(1);
            wait(mozna_pisac);
            no_of_waiting_writers.fetch_add(-1);
        }
        someone_is_writing = 1;
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

        someone_is_writing = 0;
        if (no_of_waiting_readers.load() > 0) signal(mozna_czytac);
        if (no_of_waiting_writers.load() > 0) signal(mozna_pisac);
        leave();
    }
    //////////////////////// PUSH FRONT////////////////////////////
    void push_front(T new_class) {
        enter();
        if (no_of_active_readers.load() > 0) {
            no_of_waiting_writers.fetch_add(1);
            wait(mozna_pisac);
            no_of_waiting_writers.fetch_add(-1);
        }
        someone_is_writing = 1;
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
        if (no_of_waiting_readers.load() > 0) signal(mozna_czytac);
        if (no_of_waiting_writers.load() > 0) signal(mozna_pisac);
        someone_is_writing = 0;
        leave();
    }

    // ITERATORY

    iterator begin() {
        enter();
        if (someone_is_writing) {
            no_of_waiting_readers.fetch_add(1);
            wait(mozna_czytac);
            no_of_waiting_readers.fetch_sub(1);
        }

        return iterator(first_elem, &no_of_active_readers, this);
    }

    riterator rbegin() {
        enter();
        if (someone_is_writing) {
            no_of_waiting_readers.fetch_add(1);
            wait(mozna_czytac);
            no_of_waiting_readers.fetch_sub(1);
        }

        return riterator(last_elem.lock(), &no_of_active_readers, this);
    }

    iterator end() {
        enter();
        if (someone_is_writing) {
            no_of_waiting_readers.fetch_add(1);
            wait(mozna_czytac);
            no_of_waiting_readers.fetch_sub(1);
        }

        return iterator(past_last, &no_of_active_readers, this);
    }

    riterator rend() {
        enter();
        if (someone_is_writing) {
            no_of_waiting_readers.fetch_add(1);
            wait(mozna_czytac);
            no_of_waiting_readers.fetch_sub(1);
        }

        return riterator(past_first, &no_of_active_readers, this);
    }

    void clear() {
        enter();
        if (no_of_active_readers.load() > 0) {
            no_of_waiting_writers.fetch_add(1);
            wait(mozna_pisac);
            no_of_waiting_writers.fetch_add(-1);
        }
        someone_is_writing = 1;

        last_elem.reset();
        first_elem.reset();

        if (no_of_waiting_readers.load() > 0) signal(mozna_czytac);
        if (no_of_waiting_writers.load() > 0) signal(mozna_pisac);
        someone_is_writing = 0;

        leave();
    }
    int gsize() {
        enter();
        return size;
        leave();
    }

private:
    std::shared_ptr<atomic_list::list_node<T>> first_elem;
    std::weak_ptr<atomic_list::list_node<T>> last_elem;

    std::shared_ptr<atomic_list::list_node<T>> past_last;
    std::shared_ptr<atomic_list::list_node<T>> past_first;
    std::atomic<int> no_of_active_readers;
    std::atomic<int> no_of_waiting_readers;

    std::atomic<int> no_of_waiting_writers;
    bool someone_is_writing;

    boost::interprocess::interprocess_mutex writing_mutex;
    boost::interprocess::interprocess_mutex reading_mutex;

    Condition mozna_czytac;
    Condition mozna_pisac;

    int size;
    int elem_iterator;
};

}  // namespace atomic_list

#endif