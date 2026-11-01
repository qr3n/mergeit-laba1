#include "Price.h"

#include <stdexcept>

Price::Price(const int value) {
    setValue(value);
}

int Price::getValue() const noexcept {
    return value_;
}

void Price::setValue(const int value) {
    if (value < 0) {
        throw std::invalid_argument("Price cannot be negative");
    }

    value_ = value;
}
