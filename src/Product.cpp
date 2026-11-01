#include "Product.h"
#include "Warehouse.h"

#include <algorithm>
#include <limits>
#include <stdexcept>

long long Product::nextId_ = 1;

Product::Product(const int id, const std::string& name, const Price& price,
                 const Quantity& quantity, const Weight& weight, const std::string& category)
    : id_(id), name_(name), price_(price), quantity_(quantity), weight_(weight),
      category_(category) {
    if (id <= 0) {
        throw std::invalid_argument("Product ID must be positive");
    }

    nextId_ = std::max(nextId_, static_cast<long long>(id) + 1);
}

Product::Product(const Product& other)
    : id_(other.id_), name_(other.name_), price_(other.price_), quantity_(other.quantity_),
      weight_(other.weight_), category_(other.category_) {
}

int Product::getId() const noexcept {
    return id_;
}

const std::string& Product::getName() const noexcept {
    return name_;
}

const Price& Product::getPrice() const noexcept {
    return price_;
}

const Quantity& Product::getQuantity() const noexcept {
    return quantity_;
}

const Weight& Product::getWeight() const noexcept {
    return weight_;
}

const std::string& Product::getCategory() const noexcept {
    return category_;
}

void Product::transferTo(Warehouse& other) {
    if (warehouse_ == nullptr) {
        throw std::logic_error("Product does not belong to a warehouse");
    }

    warehouse_->transferProduct(*this, other);
}

void Product::changeQuantity(const int delta) {
    quantity_.change(delta);
}

void Product::setPrice(const Price& price) noexcept {
    price_ = price;
}

int Product::generateId() {
    if (nextId_ > std::numeric_limits<int>::max()) {
        throw std::overflow_error("No product IDs available");
    }

    return static_cast<int>(nextId_++);
}
