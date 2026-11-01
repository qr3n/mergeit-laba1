#include "Warehouse.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

Warehouse::Warehouse(const std::size_t maxSize)
    : maxSize_(maxSize) {
    if (maxSize == 0) {
        throw std::invalid_argument("Warehouse capacity must be positive");
    }
}

Warehouse::Warehouse(const Warehouse& other)
    : maxSize_(other.maxSize_) {
    products_.reserve(other.products_.size());

    for (const auto& product : other.products_) {
        auto copy = product->clone();
        copy->warehouse_ = this;
        products_.push_back(std::move(copy));
    }
}

Warehouse& Warehouse::operator=(const Warehouse& other) {
    if (this != &other) {
        Warehouse copy(other);
        swap(copy);
    }

    return *this;
}

Warehouse::Warehouse(Warehouse&& other) noexcept
    : products_(std::move(other.products_)), maxSize_(other.maxSize_) {
    bindProducts();
    other.products_.clear();
}

Warehouse& Warehouse::operator=(Warehouse&& other) noexcept {
    if (this != &other) {
        products_ = std::move(other.products_);
        maxSize_ = other.maxSize_;
        bindProducts();
        other.products_.clear();
    }

    return *this;
}

Warehouse::~Warehouse() = default;

std::size_t Warehouse::getSize() const noexcept {
    return products_.size();
}

std::size_t Warehouse::getMaxSize() const noexcept {
    return maxSize_;
}

bool Warehouse::isEmpty() const noexcept {
    return products_.empty();
}

bool Warehouse::isFull() const noexcept {
    return products_.size() >= maxSize_;
}

void Warehouse::addProduct(std::unique_ptr<Product> product) {
    if (product == nullptr) {
        throw std::invalid_argument("Cannot add a null product");
    }

    if (isFull()) {
        throw std::length_error("Warehouse is full");
    }

    if (findById(product->getId()) != nullptr) {
        throw std::invalid_argument("Product ID already exists in warehouse");
    }

    products_.push_back(std::move(product));
    products_.back()->warehouse_ = this;
}

bool Warehouse::removeById(const int id) {
    const auto position = std::find_if(products_.begin(), products_.end(),
                                       [id](const auto& product) {
                                           return product->getId() == id;
                                       });

    if (position == products_.end()) {
        return false;
    }

    products_.erase(position);
    return true;
}

void Warehouse::removeByIndex(const std::size_t index) {
    if (index >= products_.size()) {
        throw std::out_of_range("Product index is out of range");
    }

    products_.erase(products_.begin() + static_cast<std::ptrdiff_t>(index));
}

Product* Warehouse::findById(const int id) noexcept {
    for (const auto& product : products_) {
        if (product->getId() == id) {
            return product.get();
        }
    }

    return nullptr;
}

const Product* Warehouse::findById(const int id) const noexcept {
    for (const auto& product : products_) {
        if (product->getId() == id) {
            return product.get();
        }
    }

    return nullptr;
}

std::vector<Product*> Warehouse::findByCategory(const std::string& category) {
    std::vector<Product*> result;

    for (const auto& product : products_) {
        if (product->getCategory() == category) {
            result.push_back(product.get());
        }
    }

    return result;
}

std::vector<const Product*> Warehouse::findByCategory(const std::string& category) const {
    std::vector<const Product*> result;

    for (const auto& product : products_) {
        if (product->getCategory() == category) {
            result.push_back(product.get());
        }
    }

    return result;
}

std::vector<Product*> Warehouse::findByPrice(const Price& price) {
    std::vector<Product*> result;

    for (const auto& product : products_) {
        if (product->getPrice().getValue() == price.getValue()) {
            result.push_back(product.get());
        }
    }

    return result;
}

std::vector<const Product*> Warehouse::findByPrice(const Price& price) const {
    std::vector<const Product*> result;

    for (const auto& product : products_) {
        if (product->getPrice().getValue() == price.getValue()) {
            result.push_back(product.get());
        }
    }

    return result;
}

WarehouseIterator Warehouse::begin() noexcept {
    return WarehouseIterator(products_.begin());
}

WarehouseIterator Warehouse::end() noexcept {
    return WarehouseIterator(products_.end());
}

ConstWarehouseIterator Warehouse::begin() const noexcept {
    return ConstWarehouseIterator(products_.begin());
}

ConstWarehouseIterator Warehouse::end() const noexcept {
    return ConstWarehouseIterator(products_.end());
}

ConstWarehouseIterator Warehouse::cbegin() const noexcept {
    return begin();
}

ConstWarehouseIterator Warehouse::cend() const noexcept {
    return end();
}

void Warehouse::transferProduct(Product& product, Warehouse& other) {
    if (this == &other) {
        return;
    }

    if (other.isFull()) {
        throw std::length_error("Destination warehouse is full");
    }

    if (other.findById(product.getId()) != nullptr) {
        throw std::invalid_argument("Product ID already exists in destination warehouse");
    }

    const auto position = std::find_if(products_.begin(), products_.end(),
                                       [&product](const auto& stored) {
                                           return stored.get() == &product;
                                       });

    if (position == products_.end()) {
        throw std::logic_error("Product does not belong to this warehouse");
    }

    other.products_.push_back(std::move(*position));
    product.warehouse_ = &other;
    products_.erase(position);
}

void Warehouse::bindProducts() noexcept {
    for (const auto& product : products_) {
        product->warehouse_ = this;
    }
}

void Warehouse::swap(Warehouse& other) noexcept {
    products_.swap(other.products_);
    std::swap(maxSize_, other.maxSize_);
    bindProducts();
    other.bindProducts();
}
