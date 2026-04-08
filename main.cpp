#include "SkipList.hpp"
#include <iostream>

int main() {
    SkipList<int> sl;
    sl.insert(1);
    sl.insert(2);
    sl.insert(3);
    if (sl.search(2)) {
        std::cout << "Found 2" << std::endl;
    }
    sl.deleteItem(2);
    if (!sl.search(2)) {
        std::cout << "Deleted 2" << std::endl;
    }
    return 0;
}
