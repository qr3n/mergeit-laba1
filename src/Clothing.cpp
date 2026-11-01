#include "Clothing.h"

Clothing::Clothing(const int id, const std::string& name, const Price& price,
                   const Quantity& quantity, const Weight& weight)
    : Product(id, name, price, quantity, weight, "clothing") {
}

TShirt::TShirt(const int id, const std::string& name, const Price& price,
               const Quantity& quantity, const Weight& weight)
    : Clothing(id, name, price, quantity, weight) {
}

std::unique_ptr<Product> TShirt::clone() const {
    return std::make_unique<TShirt>(*this);
}

Jacket::Jacket(const int id, const std::string& name, const Price& price,
               const Quantity& quantity, const Weight& weight)
    : Clothing(id, name, price, quantity, weight) {
}

std::unique_ptr<Product> Jacket::clone() const {
    return std::make_unique<Jacket>(*this);
}
