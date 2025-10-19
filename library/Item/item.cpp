#include "item.h"

Item::Item(int id, const std::string& name, double price, int stock) {
    this->id = id;
    this->name = name;
    this->price = price;
    this->stock = stock;
}
