#include "SkipList.hpp"
#include <iostream>
#include <string>

int main() {
    SkipList<int> sl;
    std::string op;
    while (std::cin >> op) {
        if (op == "insert") {
            int val;
            std::cin >> val;
            sl.insert(val);
        } else if (op == "search") {
            int val;
            std::cin >> val;
            std::cout << (sl.search(val) ? "true" : "false") << std::endl;
        } else if (op == "delete") {
            int val;
            std::cin >> val;
            sl.deleteItem(val);
        }
    }
    return 0;
}
