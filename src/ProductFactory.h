#ifndef LABASHOP_PRODUCTFACTORY_H
#define LABASHOP_PRODUCTFACTORY_H

#include "Product.h"

#include <memory>
#include <string>

class ProductFactory {
public:
    virtual ~ProductFactory() = default;

    [[nodiscard]] std::unique_ptr<Product> createProduct(
        const std::string& name, int price, int quantity, double weight) const;
    [[nodiscard]] virtual std::unique_ptr<Product> createProduct(
        int id, const std::string& name, int price, int quantity, double weight) const = 0;

protected:
    ProductFactory() = default;
    ProductFactory(const ProductFactory& other) = default;
    ProductFactory& operator=(const ProductFactory& other) = default;
    ProductFactory(ProductFactory&& other) noexcept = default;
    ProductFactory& operator=(ProductFactory&& other) noexcept = default;
};

class SmartphoneFactory final : public ProductFactory {
public:
    using ProductFactory::createProduct;

    [[nodiscard]] std::unique_ptr<Product> createProduct(
        int id, const std::string& name, int price, int quantity, double weight) const override;
};

class LaptopFactory final : public ProductFactory {
public:
    using ProductFactory::createProduct;

    [[nodiscard]] std::unique_ptr<Product> createProduct(
        int id, const std::string& name, int price, int quantity, double weight) const override;
};

class TShirtFactory final : public ProductFactory {
public:
    using ProductFactory::createProduct;

    [[nodiscard]] std::unique_ptr<Product> createProduct(
        int id, const std::string& name, int price, int quantity, double weight) const override;
};

class JacketFactory final : public ProductFactory {
public:
    using ProductFactory::createProduct;

    [[nodiscard]] std::unique_ptr<Product> createProduct(
        int id, const std::string& name, int price, int quantity, double weight) const override;
};

class MilkFactory final : public ProductFactory {
public:
    using ProductFactory::createProduct;

    [[nodiscard]] std::unique_ptr<Product> createProduct(
        int id, const std::string& name, int price, int quantity, double weight) const override;
};

class BreadFactory final : public ProductFactory {
public:
    using ProductFactory::createProduct;

    [[nodiscard]] std::unique_ptr<Product> createProduct(
        int id, const std::string& name, int price, int quantity, double weight) const override;
};

#endif
