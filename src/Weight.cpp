#include "Weight.h"

#include <cmath>
#include <stdexcept>

Weight::Weight(const double value) {
    setValue(value);
}

double Weight::getValue() const noexcept {
    return value_;
}

void Weight::setValue(const double value) {
    if (!std::isfinite(value) || value < 0.0) {
        throw std::invalid_argument("Weight must be finite and nonnegative");
    }

    value_ = value;
}
