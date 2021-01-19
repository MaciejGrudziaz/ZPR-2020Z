#ifndef BDIR_LIST_INTERFACE
#define BDIR_LIST_INTERFACE
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <memory>
#include <atomic>

#include "bdir_list_node.hpp"

namespace atomic_list {

template <class T>

class Atomic_Blist : public boost::interprocess::interprocess_mutex {
public:
    //////////////// iterator ////////////////////////////
    class iterator : public std::iterator<std::bidirectional_iterator_tag, T, T> {
    public:
         iterator(std::shared_ptr<atomic_list::list_node<T>> node,std::atomic<int> *reader_counter,boost::interprocess::interprocess_mutex  *writing_mutex) : _node(node) {
            _writing_mutex=writing_mutex;
            _reader_counter=reader_counter;
            _reader_counter->fetch_add(1);
            std::cout<<"iteratoC"<<std::endl;
            std::cout.flush();
             _writing_mutex->unlock();
            }
        ~iterator(){
            _reader_counter->fetch_sub(1);
            std::cout<<"iteratod"<<std::endl;
            std::cout.flush();
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
        bool operator==(iterator other) const { return (_node.get() == other._node.get()); }
        bool operator!=(iterator other) const { return !(_node.get() == other._node.get()); }

    private:
        std::shared_ptr<atomic_list::list_node<T>> _node;
        std::atomic<int> * _reader_counter;
        boost::interprocess::interprocess_mutex  *_writing_mutex;
    };

    //////////////// reverse iterator ////////////////////////////

    class riterator : public std::iterator<std::bidirectional_iterator_tag, T, T> {
    public:
         riterator(std::shared_ptr<atomic_list::list_node<T>> node,std::atomic<int> *reader_counter,boost::interprocess::interprocess_mutex  *writing_mutex) : _node(node) {
            _writing_mutex=writing_mutex;
            _reader_counter=reader_counter;
            _reader_counter->fetch_add(1);
            _writing_mutex->unlock();

        }
        ~riterator(){
            _reader_counter->fetch_sub(1);

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
        std::atomic<int>  *_reader_counter;
        boost::interprocess::interprocess_mutex  *_writing_mutex;
    };
//////////////// konstruktor i destruktor ////////////////////////////
    Atomic_Blist() { size = 0; 
        no_of_active_readers.store(0);
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
        no_of_active_readers=0;
    }

	void wait_for_iterators_to_finnish()
	{
        int tmp;

        do
        {
            tmp=no_of_active_readers.load();
           // std::cout<<"no of active readers= "<<tmp<<std::endl;
        }
	while(tmp>0);


	}



    //////////////////// POP BACK////////////////////////////

    T pop_back() {

        writing_mutex.lock();
        wait_for_iterators_to_finnish();
        

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

        writing_mutex.unlock();

        return tmp;
    }

    /////////////////// POP FRONT ////////////////////////////

    T pop_front() {
        writing_mutex.lock();
        wait_for_iterators_to_finnish();

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

        writing_mutex.unlock();
        return tmp;
    }
    //////////////////// PUSH  BACK////////////////////////////
    void push_back(T new_class) {
        writing_mutex.lock();
        wait_for_iterators_to_finnish();

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
        writing_mutex.unlock();
    }
    //////////////////////// PUSH FRONT////////////////////////////
    void push_front(T new_class) {
        writing_mutex.lock();
        wait_for_iterators_to_finnish();

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

        writing_mutex.unlock();
    }

    // ITERATORY

    iterator begin() {    	writing_mutex.lock();  
    return iterator(first_elem,&no_of_active_readers,&writing_mutex); }
    riterator rbegin() {    writing_mutex.lock();  
    return riterator(last_elem.lock(),&no_of_active_readers,&writing_mutex); }
    iterator end() {     	writing_mutex.lock();  	
    return iterator(past_last,&no_of_active_readers,&writing_mutex); }
    riterator rend() {   	writing_mutex.lock();  	 
    return riterator(past_first,&no_of_active_readers,&writing_mutex); }

    void clear() {
        writing_mutex.lock();
        wait_for_iterators_to_finnish();

        last_elem.reset();
        first_elem.reset();
        writing_mutex.unlock();
    }

private:
    std::shared_ptr<atomic_list::list_node<T>> first_elem;
    std::weak_ptr<atomic_list::list_node<T>> last_elem;

    std::shared_ptr<atomic_list::list_node<T>> past_last;
    std::shared_ptr<atomic_list::list_node<T>> past_first;
    std::atomic<int> no_of_active_readers;
    std::atomic< int> no_of_waiting_writers;


    boost::interprocess::interprocess_mutex  writing_mutex;

    int size;
    int elem_iterator;
};

}  // namespace atomic_list

#endif