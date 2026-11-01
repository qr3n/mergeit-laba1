#ifndef LABASHOP_WAREHOUSE_H
#define LABASHOP_WAREHOUSE_H

#include "WarehouseIterator.h"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class Warehouse {
public:
    explicit Warehouse(std::size_t maxSize);
    Warehouse(const Warehouse& other);
    Warehouse& operator=(const Warehouse& other);
    Warehouse(Warehouse&& other) noexcept;
    Warehouse& operator=(Warehouse&& other) noexcept;
    ~Warehouse();

    [[nodiscard]] std::size_t getSize() const noexcept;
    [[nodiscard]] std::size_t getMaxSize() const noexcept;
    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] bool isFull() const noexcept;

    void addProduct(std::unique_ptr<Product> product);
    bool removeById(int id);
    void removeByIndex(std::size_t index);

    [[nodiscard]] Product* findById(int id) noexcept;
    [[nodiscard]] const Product* findById(int id) const noexcept;
    [[nodiscard]] std::vector<Product*> findByCategory(const std::string& category);
    [[nodiscard]] std::vector<const Product*> findByCategory(const std::string& category) const;
    [[nodiscard]] std::vector<Product*> findByPrice(const Price& price);
    [[nodiscard]] std::vector<const Product*> findByPrice(const Price& price) const;

    [[nodiscard]] WarehouseIterator begin() noexcept;
    [[nodiscard]] WarehouseIterator end() noexcept;
    [[nodiscard]] ConstWarehouseIterator begin() const noexcept;
    [[nodiscard]] ConstWarehouseIterator end() const noexcept;
    [[nodiscard]] ConstWarehouseIterator cbegin() const noexcept;
    [[nodiscard]] ConstWarehouseIterator cend() const noexcept;

private:
    friend class Product;

    void transferProduct(Product& product, Warehouse& other);
    void bindProducts() noexcept;
    void swap(Warehouse& other) noexcept;

    std::vector<std::unique_ptr<Product>> products_;
    std::size_t maxSize_;
};

#endif
