#ifndef LABASHOP_ELECTRONICS_H
#define LABASHOP_ELECTRONICS_H

#include "Product.h"

class Electronics : public Product {
protected:
    Electronics(int id, const std::string& name, const Price& price,
                const Quantity& quantity, const Weight& weight);
};

class Smartphone final : public Electronics {
public:
    Smartphone(int id, const std::string& name, const Price& price,
               const Quantity& quantity, const Weight& weight);

    [[nodiscard]] std::unique_ptr<Product> clone() const override;
};

class Laptop final : public Electronics {
public:
    Laptop(int id, const std::string& name, const Price& price,
           const Quantity& quantity, const Weight& weight);

    [[nodiscard]] std::unique_ptr<Product> clone() const override;
};

#endif
