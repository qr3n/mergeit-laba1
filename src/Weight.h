#ifndef LABASHOP_WEIGHT_H
#define LABASHOP_WEIGHT_H

class Weight {
public:
    explicit Weight(double value = 0.0);

    [[nodiscard]] double getValue() const noexcept;
    void setValue(double value);

private:
    double value_ = 0.0;
};

#endif
