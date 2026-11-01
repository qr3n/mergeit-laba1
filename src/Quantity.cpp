#include "Quantity.h"

#include <limits>
#include <stdexcept>

Quantity::Quantity(const int value) {
    setValue(value);
}

int Quantity::getValue() const noexcept {
    return value_;
}

void Quantity::setValue(const int value) {
    if (value < 0) {
        throw std::invalid_argument("Quantity cannot be negative");
    }

    value_ = value;
}

void Quantity::change(const int delta) {
    const long long value = static_cast<long long>(value_) + delta;

    if (value < 0) {
        throw std::invalid_argument("Quantity cannot be negative");
    }

    if (value > std::numeric_limits<int>::max()) {
        throw std::overflow_error("Quantity is too large");
    }

    value_ = static_cast<int>(value);
}
