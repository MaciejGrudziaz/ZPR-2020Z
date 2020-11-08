#include <iostream>
#include <boost/algorithm/string/trim_all.hpp>

int main() {
    std::cout<< boost::trim_all_copy(std::string("    Hello    world    ")) << "\n";
    return 0;
}
