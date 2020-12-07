#ifndef BDIR_LIST_INTERFACE
#define BDIR_LIST_INTERFACE
#include "bdir_list_node.hpp"
#include <memory>

namespace atomic_list {

template <class T>

class Atomic_Blist{
public:
void pop_back();               
void push_back(T new_class){
    if(!last_elem)
    {
        std::shared_ptr< atomic_list::list_node<T> >tmp=new list_node<T>(new_class);
        last_elem->prev_elem=tmp;
        tmp->next_elem=last_elem;
        tmp->prev_elem=first_elem;
        last_elem=tmp;
    }
    else
    {
        std::shared_ptr< atomic_list::list_node<T> >tmp=new list_node<T>(new_class);
        tmp->last_elem=tmp;
        tmp->prev_elem=tmp;
        last_elem=tmp;
        first_elem=tmp;
    }
}    
void pop_front();
void push_front(T new_class);
void begin();                   
void end();

//operatory
void operator ++();
void operator --();
T operator=(const Atomic_Blist & list)
{
return pointed_elem->get_container();
}

private:

std::shared_ptr< atomic_list::list_node<T> > first_elem;
std::weak_ptr <atomic_list::list_node<T> > last_elem;

std::shared_ptr<atomic_list::list_node<T>> pointed_elem;
int size;
int elem_iterator;

};

}

#endif 