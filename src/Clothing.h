#ifndef LABASHOP_CLOTHING_H
#define LABASHOP_CLOTHING_H

#include "Product.h"

class Clothing : public Product {
protected:
    Clothing(int id, const std::string& name, const Price& price,
             const Quantity& quantity, const Weight& weight);
};

class TShirt final : public Clothing {
public:
    TShirt(int id, const std::string& name, const Price& price,
           const Quantity& quantity, const Weight& weight);

    [[nodiscard]] std::unique_ptr<Product> clone() const override;
};

class Jacket final : public Clothing {
public:
    Jacket(int id, const std::string& name, const Price& price,
           const Quantity& quantity, const Weight& weight);

    [[nodiscard]] std::unique_ptr<Product> clone() const override;
};

#endif
