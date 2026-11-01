#ifndef LABASHOP_PRODUCT_H
#define LABASHOP_PRODUCT_H

#include "Price.h"
#include "Quantity.h"
#include "Weight.h"

#include <memory>
#include <string>

class Warehouse;

class Product {
public:
    virtual ~Product() = default;

    Product(Product&& other) = delete;
    Product& operator=(const Product& other) = delete;
    Product& operator=(Product&& other) = delete;

    [[nodiscard]] int getId() const noexcept;
    [[nodiscard]] const std::string& getName() const noexcept;
    [[nodiscard]] const Price& getPrice() const noexcept;
    [[nodiscard]] const Quantity& getQuantity() const noexcept;
    [[nodiscard]] const Weight& getWeight() const noexcept;
    [[nodiscard]] const std::string& getCategory() const noexcept;

    void transferTo(Warehouse& other);
    void changeQuantity(int delta);
    void setPrice(const Price& price) noexcept;

    [[nodiscard]] virtual std::unique_ptr<Product> clone() const = 0;

    [[nodiscard]] static int generateId();

protected:
    Product(int id, const std::string& name, const Price& price,
            const Quantity& quantity, const Weight& weight, const std::string& category);
    Product(const Product& other);

private:
    friend class Warehouse;

    static long long nextId_;

    int id_;
    std::string name_;
    Price price_;
    Quantity quantity_;
    Weight weight_;
    std::string category_;
    Warehouse* warehouse_ = nullptr;
};

#endif
