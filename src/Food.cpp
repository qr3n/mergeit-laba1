#include "Food.h"

Food::Food(const int id, const std::string& name, const Price& price,
           const Quantity& quantity, const Weight& weight)
    : Product(id, name, price, quantity, weight, "food") {
}

Milk::Milk(const int id, const std::string& name, const Price& price,
           const Quantity& quantity, const Weight& weight)
    : Food(id, name, price, quantity, weight) {
}

std::unique_ptr<Product> Milk::clone() const {
    return std::make_unique<Milk>(*this);
}

Bread::Bread(const int id, const std::string& name, const Price& price,
             const Quantity& quantity, const Weight& weight)
    : Food(id, name, price, quantity, weight) {
}

std::unique_ptr<Product> Bread::clone() const {
    return std::make_unique<Bread>(*this);
}
