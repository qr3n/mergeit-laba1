#ifndef LABASHOP_QUANTITY_H
#define LABASHOP_QUANTITY_H

class Quantity {
public:
    explicit Quantity(int value = 0);

    [[nodiscard]] int getValue() const noexcept;
    void setValue(int value);
    void change(int delta);

private:
    int value_ = 0;
};

#endif
