#include "ProductFactory.h"
#include "Warehouse.h"

#include <exception>
#include <iostream>
#include <utility>

namespace {

[[nodiscard]] Warehouse createDemoWarehouse() {
    Warehouse warehouse(10);

    const SmartphoneFactory smartphoneFactory;
    const LaptopFactory laptopFactory;
    const TShirtFactory tshirtFactory;
    const JacketFactory jacketFactory;
    const MilkFactory milkFactory;
    const BreadFactory breadFactory;

    warehouse.addProduct(smartphoneFactory.createProduct(1, "Смартфон", 60000, 12, 0.19));
    warehouse.addProduct(laptopFactory.createProduct(2, "Ноутбук", 95000, 5, 1.6));
    warehouse.addProduct(tshirtFactory.createProduct(3, "Футболка", 1500, 30, 0.2));
    warehouse.addProduct(jacketFactory.createProduct(4, "Куртка", 8000, 8, 0.9));
    warehouse.addProduct(milkFactory.createProduct(5, "Молоко", 100, 40, 1.0));
    warehouse.addProduct(breadFactory.createProduct(6, "Хлеб", 100, 25, 0.5));

    return warehouse;
}

void printProduct(const Product& product) {
    std::cout << product.getId() << " | " << product.getName()
              << " | " << product.getCategory()
              << " | " << product.getPrice().getValue() << " руб."
              << " | " << product.getQuantity().getValue() << " шт."
              << " | " << product.getWeight().getValue() << " кг\n";
}

void printWarehouse(const Warehouse& warehouse) {
    std::cout << "Товаров: " << warehouse.getSize() << '/' << warehouse.getMaxSize() << '\n';

    for (const auto& product : warehouse) {
        printProduct(product);
    }
}

void demonstrateCreation() {
    std::cout << "1. Создание склада, фабрики и итерация\n";
    const Warehouse warehouse = createDemoWarehouse();
    printWarehouse(warehouse);
}

void demonstrateCopying() {
    std::cout << "\n2. Глубокое копирование\n";
    const Warehouse original = createDemoWarehouse();
    Warehouse copy(original);

    copy.findById(1)->setPrice(Price(55000));
    copy.findById(1)->changeQuantity(-2);

    std::cout << "Оригинал: ";
    printProduct(*original.findById(1));
    std::cout << "Копия:    ";
    printProduct(*copy.findById(1));

    Warehouse assigned(1);
    assigned = std::move(copy);
    std::cout << "После присваивания копированием: " << assigned.getSize() << " товаров\n";
}

void demonstrateMoving() {
    std::cout << "\n3. Перемещение склада\n";
    Warehouse original = createDemoWarehouse();
    const Product* const product = original.findById(1);
    Warehouse moved(std::move(original));

    std::cout << "Исходный склад пуст: " << original.isEmpty() << '\n';
    std::cout << "Адрес товара сохранён: " << (moved.findById(1) == product) << '\n';

    Warehouse assigned(1);
    assigned = std::move(moved);

    std::cout << "После присваивания перемещением: " << assigned.getSize() << " товаров\n";
    std::cout << "Адрес товара сохранён: " << (assigned.findById(1) == product) << '\n';
}

void demonstrateSearchingAndRemoving() {
    std::cout << "\n4. Поиск и удаление\n";
    Warehouse warehouse = createDemoWarehouse();

    std::cout << "Товар с ID 3:\n";
    printProduct(*warehouse.findById(3));

    std::cout << "Категория food:\n";
    for (const auto* const product : warehouse.findByCategory("food")) {
        printProduct(*product);
    }

    std::cout << "Цена 100 руб.:\n";
    for (const auto* const product : warehouse.findByPrice(Price(100))) {
        printProduct(*product);
    }

    warehouse.removeById(3);
    warehouse.removeByIndex(0);

    std::cout << "После удаления по ID и индексу:\n";
    printWarehouse(warehouse);
}

void demonstrateTransfer() {
    std::cout << "\n5. Перенос товара, изменение остатка и цены\n";
    Warehouse source = createDemoWarehouse();
    Warehouse destination(3);
    Product* const product = source.findById(1);

    product->transferTo(destination);
    product->changeQuantity(3);
    product->setPrice(Price(58000));

    std::cout << "На исходном складе: " << source.getSize() << " товаров\n";
    std::cout << "На новом складе:\n";
    printWarehouse(destination);
    std::cout << "Адрес товара сохранён: " << (destination.findById(1) == product) << '\n';
}

}

int main() {
    try {
        std::cout << std::boolalpha;
        demonstrateCreation();
        demonstrateCopying();
        demonstrateMoving();
        demonstrateSearchingAndRemoving();
        demonstrateTransfer();
    } catch (const std::exception& error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
