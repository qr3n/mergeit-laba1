#ifndef LABASHOP_WAREHOUSEITERATOR_H
#define LABASHOP_WAREHOUSEITERATOR_H

#include "Product.h"

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <vector>

class Warehouse;

template<typename Value>
class BasicWarehouseIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Product;
    using difference_type = std::ptrdiff_t;
    using pointer = Value*;
    using reference = Value&;

    BasicWarehouseIterator() = default;

    template<typename OtherValue,
             typename = std::enable_if_t<std::is_convertible_v<OtherValue*, Value*>>>
    BasicWarehouseIterator(const BasicWarehouseIterator<OtherValue>& other) noexcept
        : current_(other.current_) {
    }

    [[nodiscard]] reference operator*() const noexcept {
        return **current_;
    }

    [[nodiscard]] pointer operator->() const noexcept {
        return current_->get();
    }

    BasicWarehouseIterator& operator++() noexcept {
        ++current_;
        return *this;
    }

    BasicWarehouseIterator operator++(int) noexcept {
        const BasicWarehouseIterator previous(*this);
        ++(*this);
        return previous;
    }

    template<typename OtherValue>
    [[nodiscard]] bool operator==(const BasicWarehouseIterator<OtherValue>& other) const noexcept {
        return current_ == other.current_;
    }

    template<typename OtherValue>
    [[nodiscard]] bool operator!=(const BasicWarehouseIterator<OtherValue>& other) const noexcept {
        return !(*this == other);
    }

private:
    friend class Warehouse;
    template<typename> friend class BasicWarehouseIterator;

    using Storage = std::vector<std::unique_ptr<Product>>;
    using StorageIterator = std::conditional_t<std::is_const_v<Value>,
                                               Storage::const_iterator, Storage::iterator>;

    explicit BasicWarehouseIterator(const StorageIterator current) noexcept
        : current_(current) {
    }

    StorageIterator current_{};
};

using WarehouseIterator = BasicWarehouseIterator<Product>;
using ConstWarehouseIterator = BasicWarehouseIterator<const Product>;

#endif
