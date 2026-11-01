#ifndef LABASHOP_PRICE_H
#define LABASHOP_PRICE_H

class Price {
public:
    explicit Price(int value = 0);

    [[nodiscard]] int getValue() const noexcept;
    void setValue(int value);

private:
    int value_ = 0;
};

#endif
