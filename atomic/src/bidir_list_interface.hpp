#ifndef BDIR_LIST_INTERFACE
#define BDIR_LIST_INTERFACE
#include <memory>
#include <../../boost/interprocess/sync/interprocess_mutex.hpp>

#include "bdir_list_node.hpp"

namespace atomic_list {

template <class T>

class Atomic_Blist : public boost::interprocess::interprocess_mutex {

public:

   class iterator : public std::iterator<std::bidirectional_iterator_tag,T,T> {
    public:
        explicit iterator(std::shared_ptr<atomic_list::list_node<T>> node): _node(node){}       
        iterator& operator++(){
            if(_node->prev_elem)
            {
                _node=_node->prev_elem;
            }
            return *this;
        }
        iterator operator++(int){
        auto curr_it = iterator(_node);
        if (_node->prev_elem) {
            _node = _node->prev_elem;
            }
            return curr_it;
        }
        iterator& operator--();
        iterator operator--(int);

        T& operator*() const {return _node->container;}
        bool operator==(iterator other) const {return (_node.get() == other._node.get());}
        bool operator!=(iterator other) const{ return  !(*this == other);}
       

    private:
        std::shared_ptr<atomic_list::list_node<T>> _node;
    };

   








Atomic_Blist()
{
    size=0;
}



void pop_back()
{

if(size>1){

    if(last_elem.lock()->get()==pointed_elem->get())
    {
               last_elem=last_elem.lock()->next_elem.lock();
        pointed_elem=last_elem.lock();
        pointed_elem->prev_elem=first_elem;

    } 
    else
    {
        last_elem=last_elem.lock()->next_elem;
        last_elem.lock()->prev_elem=first_elem;
    }
    
size--;
}
else if(size <=1)
{
  
    last_elem.reset();
    first_elem.reset();
    pointed_elem.reset();
}  

}  
void pop_front(){
if(size>1){
    if(first_elem==pointed_elem)
    {
        first_elem=first_elem->prev_elem;
        last_elem.lock()->next_elem=first_elem;
        first_elem->next_elem=last_elem;
        pointed_elem=first_elem;

    } 
    else
    {
        first_elem=first_elem->prev_elem;
        last_elem.lock()->next_elem=first_elem;
        first_elem->next_elem=last_elem;

    }    
size--;
}
else if(size <=1)
{
    last_elem.reset();
    first_elem.reset();
    pointed_elem.reset();
} 
}   

void push_back(T new_class){
    this->lock();
    if(last_elem.lock())
    {
        std::shared_ptr< atomic_list::list_node<T> >tmp=std::make_shared<list_node<T>>(new_class);
        last_elem.lock()->prev_elem=tmp;
        tmp->next_elem=last_elem;
        tmp->prev_elem=first_elem;
        last_elem=tmp;
        first_elem->next_elem.reset();
        
    }
    else
    {
        std::shared_ptr< atomic_list::list_node<T> >tmp=std::make_shared<list_node<T>>(new_class);
        tmp->next_elem=tmp;
        tmp->prev_elem=tmp;
        last_elem=tmp;
        first_elem=tmp;
    }
size++;
    this->unlock();
}    
void push_front(T new_class)
{
    if(first_elem)
    {
        std::shared_ptr< atomic_list::list_node<T> >tmp=std::make_shared<list_node<T>>(new_class);
        tmp->prev_elem=first_elem;
        first_elem->next_elem=tmp;
        tmp->next_elem=last_elem.lock();
        first_elem=tmp;
    }
    else
    {
        std::shared_ptr< atomic_list::list_node<T> >tmp=std::make_shared<list_node<T>>(new_class);
        tmp->next_elem=tmp;
        tmp->prev_elem=tmp;
        last_elem=tmp;
        first_elem=tmp;

    }
    size++;
}

iterator begin(){
    pointed_elem=first_elem;
    return iterator(pointed_elem);
}                   
iterator end(){

    pointed_elem=last_elem.lock();
    return iterator(pointed_elem);
}

void clear()
{
    last_elem.reset();
    first_elem.reset();
    pointed_elem.reset();
}






private:

std::shared_ptr< atomic_list::list_node<T> > first_elem;
std::weak_ptr <atomic_list::list_node<T> > last_elem;

std::shared_ptr<atomic_list::list_node<T>> pointed_elem;
int size;
int elem_iterator;







};

}  // namespace atomic_list

#endif