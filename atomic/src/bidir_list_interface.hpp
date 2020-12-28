#ifndef BDIR_LIST_INTERFACE
#define BDIR_LIST_INTERFACE
#include <memory>
#include <../../boost/interprocess/sync/interprocess_mutex.hpp>

#include "bdir_list_node.hpp"

namespace atomic_list {

template <class T>

class Atomic_Blist {

public:

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
    if(last_elem.lock())
    {
        std::shared_ptr< atomic_list::list_node<T> >tmp=std::make_shared<list_node<T>>(new_class);
        last_elem.lock()->prev_elem=tmp;
        tmp->next_elem=last_elem;
        tmp->prev_elem=first_elem;
        last_elem=tmp;
        
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

T begin(){
    pointed_elem=first_elem;
    return pointed_elem->get();
}                   
T end(){

    pointed_elem=last_elem.lock();
    return pointed_elem->get();
}

void clear()
{
    last_elem.reset();
    first_elem.reset();
    pointed_elem.reset();
}

T getVal()
{
    return pointed_elem->get();
}

//operatory
void operator ++()
{
pointed_elem=pointed_elem->next_elem.lock();
};
void operator --();
T operator=(const Atomic_Blist & list)
{
return pointed_elem->get();
}


private:

std::shared_ptr< atomic_list::list_node<T> > first_elem;
std::weak_ptr <atomic_list::list_node<T> > last_elem;

std::shared_ptr<atomic_list::list_node<T>> pointed_elem;
int size;
int elem_iterator;


class iterator : public std::iterator<std::forward_iterator_tag, T, T, const T*,const T&>
{


};




};

}  // namespace atomic_list

#endif