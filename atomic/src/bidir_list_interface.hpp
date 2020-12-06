#ifndef BDIR_LIST_INTERFACE
#define BDIR_LIST_INTERFACE
#include <memory>

#include "bdir_list_node.hpp"
namespace atomic_list {

template <class T>

class Atomic_Blist {
public:
    void pop_back();
    void push_back(T new_class);
    void pop_front();
    void push_front(T new_class);
    void begin();
    void end();

    // operatory
    void operator++();
    void operator--();
    T operator=(const Atomic_Blist& list) {
        return pointed_elem->get_container();
    }

private:
    std::shared_ptr<atomic_list::list_node> first_elem;
    std::weak_ptr<atomic_list::list_node> last_elem;

    std::shared_ptr<atomic_list::list_node> pointed_elem;
    int size;
    int elem_iterator;
};

}  // namespace atomic_list

#endif