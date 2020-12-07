#ifndef BDIR_LIST_NODE
#define BDIR_LIST_NODE
#include <memory>
namespace atomic_list {
template <class T>

class list_node{
    public:
    list_node(T new_container)
    {
    container=new_container;
    }
    T get(){
        return  container;
    }
    std::shared_ptr<list_node<T>> prev_elem;
    std::weak_ptr<list_node<T>> next_elem;
    T container;
    };
}
#endif