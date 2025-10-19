#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

struct Item {
    int id;
    std::string name;
    double price;
    int stock;

    Item(int id, const std::string& name, double price, int stock);
};

#endif
