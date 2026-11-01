#include "ProductFactory.h"
#include "Clothing.h"
#include "Electronics.h"
#include "Food.h"

std::unique_ptr<Product> ProductFactory::createProduct(const std::string& name, const int price,
                                                      const int quantity, const double weight) const {
    return createProduct(Product::generateId(), name, price, quantity, weight);
}

std::unique_ptr<Product> SmartphoneFactory::createProduct(const int id, const std::string& name,
                                                         const int price, const int quantity,
                                                         const double weight) const {
    return std::make_unique<Smartphone>(id, name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> LaptopFactory::createProduct(const int id, const std::string& name,
                                                     const int price, const int quantity,
                                                     const double weight) const {
    return std::make_unique<Laptop>(id, name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> TShirtFactory::createProduct(const int id, const std::string& name,
                                                     const int price, const int quantity,
                                                     const double weight) const {
    return std::make_unique<TShirt>(id, name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> JacketFactory::createProduct(const int id, const std::string& name,
                                                     const int price, const int quantity,
                                                     const double weight) const {
    return std::make_unique<Jacket>(id, name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> MilkFactory::createProduct(const int id, const std::string& name,
                                                   const int price, const int quantity,
                                                   const double weight) const {
    return std::make_unique<Milk>(id, name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> BreadFactory::createProduct(const int id, const std::string& name,
                                                    const int price, const int quantity,
                                                    const double weight) const {
    return std::make_unique<Bread>(id, name, Price(price), Quantity(quantity), Weight(weight));
}
