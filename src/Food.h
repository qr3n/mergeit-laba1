#ifndef LABASHOP_FOOD_H
#define LABASHOP_FOOD_H

#include "Product.h"

class Food : public Product {
protected:
    Food(int id, const std::string& name, const Price& price,
         const Quantity& quantity, const Weight& weight);
};

class Milk final : public Food {
public:
    Milk(int id, const std::string& name, const Price& price,
         const Quantity& quantity, const Weight& weight);

    [[nodiscard]] std::unique_ptr<Product> clone() const override;
};

class Bread final : public Food {
public:
    Bread(int id, const std::string& name, const Price& price,
          const Quantity& quantity, const Weight& weight);

    [[nodiscard]] std::unique_ptr<Product> clone() const override;
};

#endif
