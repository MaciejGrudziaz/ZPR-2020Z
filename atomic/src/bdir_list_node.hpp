#ifndef BDIR_LIST_NODE
#define BDIR_LIST_NODE
#include <memory>
namespace atomic_list {
template <class T>

class list_node {
    T get_container();
    void push_container(T);

private:
    std::shared_ptr<list_node> prev_elem;
    std::weak_ptr<list_node> next_elem;

    std::shared_ptr<T> container;
};
}  // namespace atomic_list
#endif