#include "Electronics.h"

Electronics::Electronics(const int id, const std::string& name, const Price& price,
                         const Quantity& quantity, const Weight& weight)
    : Product(id, name, price, quantity, weight, "electronics") {
}

Smartphone::Smartphone(const int id, const std::string& name, const Price& price,
                       const Quantity& quantity, const Weight& weight)
    : Electronics(id, name, price, quantity, weight) {
}

std::unique_ptr<Product> Smartphone::clone() const {
    return std::make_unique<Smartphone>(*this);
}

Laptop::Laptop(const int id, const std::string& name, const Price& price,
               const Quantity& quantity, const Weight& weight)
    : Electronics(id, name, price, quantity, weight) {
}

std::unique_ptr<Product> Laptop::clone() const {
    return std::make_unique<Laptop>(*this);
}
