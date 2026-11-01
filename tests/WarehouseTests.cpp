#include "Clothing.h"
#include "Electronics.h"
#include "Food.h"
#include "ProductFactory.h"
#include "Warehouse.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

static_assert(std::is_abstract_v<Product>);
static_assert(std::is_abstract_v<Electronics>);
static_assert(std::is_abstract_v<Clothing>);
static_assert(std::is_abstract_v<Food>);
static_assert(std::is_abstract_v<ProductFactory>);
static_assert(std::is_nothrow_move_constructible_v<Warehouse>);
static_assert(std::is_nothrow_move_assignable_v<Warehouse>);
static_assert(std::is_same_v<decltype(*std::declval<Warehouse&>().begin()), Product&>);
static_assert(std::is_same_v<decltype(*std::declval<const Warehouse&>().begin()), const Product&>);
static_assert(std::is_convertible_v<WarehouseIterator, ConstWarehouseIterator>);
static_assert(!std::is_convertible_v<ConstWarehouseIterator, WarehouseIterator>);
static_assert(std::is_same_v<decltype(std::declval<const Warehouse&>().findById(1)), const Product*>);

namespace {

void expect(const bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

template<typename Exception, typename Action>
void expectException(const Action& action, const std::string& message) {
    try {
        action();
    } catch (const Exception&) {
        return;
    }

    throw std::runtime_error(message);
}

void testAttributes() {
    expect(Price().getValue() == 0, "Default price");
    expect(Quantity().getValue() == 0, "Default quantity");
    expect(Weight().getValue() == 0.0, "Default weight");

    Price price(100);
    price.setValue(200);
    expect(price.getValue() == 200, "Price update");
    expectException<std::invalid_argument>([] { const Price value(-1); }, "Negative price accepted");
    expectException<std::invalid_argument>([&] { price.setValue(-1); }, "Negative price update accepted");
    expect(price.getValue() == 200, "Failed price update changed value");

    Quantity quantity(10);
    quantity.setValue(5);
    quantity.change(-5);
    expect(quantity.getValue() == 0, "Quantity decrement to zero");
    expectException<std::invalid_argument>([] { const Quantity value(-1); }, "Negative quantity accepted");
    expectException<std::invalid_argument>([&] { quantity.setValue(-1); }, "Negative quantity update accepted");
    expectException<std::invalid_argument>([&] { quantity.change(-1); }, "Negative stock accepted");
    expectException<std::invalid_argument>([&] {
        quantity.change(std::numeric_limits<int>::min());
    }, "Minimum integer delta accepted");
    expect(quantity.getValue() == 0, "Failed quantity change changed value");

    quantity.setValue(std::numeric_limits<int>::max());
    expectException<std::overflow_error>([&] { quantity.change(1); }, "Quantity overflow accepted");
    expect(quantity.getValue() == std::numeric_limits<int>::max(), "Overflow changed quantity");

    Weight weight(0.5);
    weight.setValue(1.5);
    expect(weight.getValue() == 1.5, "Weight update");
    expectException<std::invalid_argument>([] { const Weight value(-0.1); }, "Negative weight accepted");
    expectException<std::invalid_argument>([&] { weight.setValue(-1.0); }, "Negative weight update accepted");
    expectException<std::invalid_argument>([&] {
        weight.setValue(std::numeric_limits<double>::infinity());
    }, "Infinite weight accepted");
    expectException<std::invalid_argument>([&] {
        weight.setValue(std::numeric_limits<double>::quiet_NaN());
    }, "NaN weight accepted");
    expect(weight.getValue() == 1.5, "Failed weight update changed value");
}

template<typename ExpectedProduct>
void testFactory(const ProductFactory& factory, const std::string& category) {
    const auto product = factory.createProduct("Product", 150, 10, 0.5);
    expect(dynamic_cast<ExpectedProduct*>(product.get()) != nullptr, "Incorrect factory product type");
    expect(product->getName() == "Product", "Product name");
    expect(product->getPrice().getValue() == 150, "Product price");
    expect(product->getQuantity().getValue() == 10, "Product quantity");
    expect(product->getWeight().getValue() == 0.5, "Product weight");
    expect(product->getCategory() == category, "Product category");

    const auto copy = product->clone();
    expect(copy.get() != product.get(), "Clone reused original object");
    expect(dynamic_cast<ExpectedProduct*>(copy.get()) != nullptr, "Clone lost concrete type");
    expect(copy->getId() == product->getId(), "Clone changed ID");
    copy->setPrice(Price(200));
    copy->changeQuantity(-1);
    expect(product->getPrice().getValue() == 150, "Clone changed original price");
    expect(product->getQuantity().getValue() == 10, "Clone changed original quantity");
}

void testFactories() {
    testFactory<Smartphone>(SmartphoneFactory(), "electronics");
    testFactory<Laptop>(LaptopFactory(), "electronics");
    testFactory<TShirt>(TShirtFactory(), "clothing");
    testFactory<Jacket>(JacketFactory(), "clothing");
    testFactory<Milk>(MilkFactory(), "food");
    testFactory<Bread>(BreadFactory(), "food");

    const SmartphoneFactory factory;
    const auto first = factory.createProduct("First", 1, 1, 0.1);
    const auto second = factory.createProduct("Second", 1, 1, 0.1);
    expect(first->getId() != second->getId(), "Generated duplicate IDs");

    const auto manual = factory.createProduct(1000, "Manual", 1, 1, 0.1);
    const auto automatic = factory.createProduct("Automatic", 1, 1, 0.1);
    expect(automatic->getId() > manual->getId(), "Explicit ID did not advance counter");

    expectException<std::invalid_argument>([&] {
        static_cast<void>(factory.createProduct(0, "Invalid", 1, 1, 0.1));
    }, "Zero ID accepted");
    expectException<std::invalid_argument>([&] {
        static_cast<void>(factory.createProduct(-1, "Invalid", 1, 1, 0.1));
    }, "Negative ID accepted");
    expectException<std::invalid_argument>([&] {
        static_cast<void>(factory.createProduct("Invalid", -1, 1, 0.1));
    }, "Factory accepted invalid attributes");
}

[[nodiscard]] Warehouse createWarehouse() {
    Warehouse warehouse(4);
    warehouse.addProduct(SmartphoneFactory().createProduct(1, "Phone", 100, 10, 0.2));
    warehouse.addProduct(MilkFactory().createProduct(2, "Milk", 50, 20, 1.0));
    warehouse.addProduct(BreadFactory().createProduct(3, "Bread", 50, 30, 0.5));
    return warehouse;
}

void testStorageAndSearch() {
    expectException<std::invalid_argument>([] { const Warehouse warehouse(0); }, "Zero capacity accepted");

    Warehouse warehouse(1);
    expect(warehouse.isEmpty() && !warehouse.isFull(), "Initial warehouse state");
    expect(warehouse.begin() == warehouse.end(), "Empty warehouse iteration");
    expectException<std::invalid_argument>([&] { warehouse.addProduct(nullptr); }, "Null product accepted");

    warehouse.addProduct(MilkFactory().createProduct(1, "Milk", 50, 20, 1.0));
    expect(warehouse.getSize() == 1 && warehouse.isFull(), "Warehouse capacity");
    expectException<std::length_error>([&] {
        warehouse.addProduct(BreadFactory().createProduct(2, "Bread", 50, 10, 0.5));
    }, "Warehouse limit exceeded");
    expect(warehouse.getSize() == 1, "Failed addition changed warehouse");

    expect(!warehouse.removeById(99), "Missing product removed");
    expectException<std::out_of_range>([&] { warehouse.removeByIndex(1); }, "Invalid index accepted");
    expectException<std::out_of_range>([&] {
        warehouse.removeByIndex(std::numeric_limits<std::size_t>::max());
    }, "Maximum index accepted");
    expect(warehouse.removeById(1) && warehouse.isEmpty(), "Removal by ID");
    expectException<std::out_of_range>([&] { warehouse.removeByIndex(0); }, "Removal from empty warehouse");

    warehouse = createWarehouse();
    expectException<std::invalid_argument>([&] {
        warehouse.addProduct(MilkFactory().createProduct(1, "Duplicate", 50, 1, 1.0));
    }, "Duplicate ID accepted");

    expect(warehouse.findById(1)->getName() == "Phone", "Search by ID");
    expect(warehouse.findById(99) == nullptr, "Missing ID search");
    expect(warehouse.findByCategory("food").size() == 2, "Search by category");
    expect(warehouse.findByCategory("unknown").empty(), "Missing category search");
    expect(warehouse.findByPrice(Price(50)).size() == 2, "Search by price");
    expect(warehouse.findByPrice(Price(99)).empty(), "Missing price search");

    const Warehouse& constant = warehouse;
    expect(constant.findById(1) == warehouse.findById(1), "Const search by ID");
    expect(constant.findById(99) == nullptr, "Const missing ID search");
    expect(constant.findByCategory("food").size() == 2, "Const search by category");
    expect(constant.findByCategory("unknown").empty(), "Const missing category search");
    expect(constant.findByPrice(Price(50)).size() == 2, "Const search by price");
    expect(constant.findByPrice(Price(99)).empty(), "Const missing price search");

    warehouse.removeByIndex(1);
    expect(warehouse.getSize() == 2 && warehouse.findById(2) == nullptr, "Removal by index");
    expect(warehouse.findById(1) != nullptr && warehouse.findById(3) != nullptr, "Removal changed other products");
}

void testCopying() {
    const Warehouse original = createWarehouse();
    Warehouse copy(original);
    expect(copy.getSize() == original.getSize(), "Copy size");
    expect(copy.getMaxSize() == original.getMaxSize(), "Copy capacity");

    for (const auto& product : original) {
        const Product* const copied = copy.findById(product.getId());
        expect(copied != &product, "Shallow warehouse copy");
        expect(copied->getName() == product.getName(), "Copy lost name");
        expect(copied->getWeight().getValue() == product.getWeight().getValue(), "Copy lost weight");
        expect(copied->getCategory() == product.getCategory(), "Copy lost category");
    }

    expect(dynamic_cast<Smartphone*>(copy.findById(1)) != nullptr, "Copy lost product type");
    copy.findById(1)->setPrice(Price(200));
    copy.findById(1)->changeQuantity(-5);
    expect(original.findById(1)->getPrice().getValue() == 100, "Copy changed original price");
    expect(original.findById(1)->getQuantity().getValue() == 10, "Copy changed original quantity");

    Warehouse assigned(1);
    assigned.addProduct(JacketFactory().createProduct(99, "Old", 1, 1, 0.5));
    assigned = original;
    expect(assigned.getSize() == 3 && assigned.getMaxSize() == 4, "Copy assignment state");
    expect(assigned.findById(99) == nullptr, "Copy assignment retained old products");
    expect(assigned.findById(1) != original.findById(1), "Copy assignment shared product");

    const Warehouse& same = assigned;
    const Product* const product = assigned.findById(1);
    assigned = same;
    expect(assigned.findById(1) == product, "Self copy assignment changed product");

    Warehouse destination(2);
    copy.findById(1)->transferTo(destination);
    assigned.findById(2)->transferTo(destination);
    expect(original.getSize() == 3, "Copied product transfer changed original warehouse");
    expect(copy.getSize() == 2 && assigned.getSize() == 2, "Copied product owner not updated");

    Warehouse detached(1);
    detached.addProduct(original.findById(3)->clone());
    copy.removeById(3);
    detached.findById(3)->transferTo(copy);
    expect(detached.isEmpty() && original.getSize() == 3, "Clone retained original owner");

    const Warehouse empty(2);
    Warehouse emptyCopy(empty);
    expect(emptyCopy.isEmpty() && emptyCopy.getMaxSize() == 2, "Empty warehouse copy");
    emptyCopy.addProduct(MilkFactory().createProduct(4, "Milk", 50, 1, 1.0));
    expect(empty.isEmpty(), "Empty warehouse copy shares storage");
    assigned = empty;
    expect(assigned.isEmpty() && assigned.getMaxSize() == 2, "Assignment from empty warehouse");
}

void testMoving() {
    Warehouse original = createWarehouse();
    Product* const product = original.findById(1);
    Warehouse moved(std::move(original));
    expect(original.isEmpty(), "Move constructor did not empty source");
    expect(moved.findById(1) == product, "Move constructor copied product");
    expect(moved.getMaxSize() == 4, "Move constructor lost capacity");

    original.addProduct(MilkFactory().createProduct(4, "New", 1, 1, 1.0));
    product->transferTo(original);
    expect(moved.findById(1) == nullptr && original.findById(1) == product, "Move constructor lost product owner");

    Warehouse assigned(1);
    assigned.addProduct(MilkFactory().createProduct(99, "Old", 1, 1, 1.0));
    Product* const second = moved.findById(2);
    assigned = std::move(moved);
    expect(moved.isEmpty(), "Move assignment did not empty source");
    expect(assigned.findById(2) == second, "Move assignment copied product");
    expect(assigned.getMaxSize() == 4 && assigned.findById(99) == nullptr, "Move assignment state");

    Warehouse& same = assigned;
    assigned = std::move(same);
    expect(assigned.findById(2) == second, "Self move assignment changed product");

    second->transferTo(moved);
    expect(assigned.findById(2) == nullptr && moved.findById(2) == second, "Move assignment lost product owner");

    Warehouse empty(2);
    Warehouse movedEmpty(std::move(empty));
    expect(movedEmpty.isEmpty() && movedEmpty.getMaxSize() == 2, "Empty warehouse move");
    assigned = std::move(movedEmpty);
    expect(assigned.isEmpty() && assigned.getMaxSize() == 2, "Move assignment from empty warehouse");
}

void testIterators() {
    Warehouse warehouse = createWarehouse();
    expect(std::distance(warehouse.begin(), warehouse.end()) == 3, "Iterator distance");

    for (auto& product : warehouse) {
        product.changeQuantity(-1);
    }

    expect(warehouse.findById(1)->getQuantity().getValue() == 9, "Mutable iterator");
    auto current = warehouse.begin();
    const auto previous = current++;
    expect(previous->getId() == 1 && current->getId() == 2, "Postfix increment");
    ++current;
    expect((*current).getId() == 3, "Prefix increment");

    const ConstWarehouseIterator constant = warehouse.begin();
    expect(constant == warehouse.begin(), "Mutable and const iterator equality");
    expect(warehouse.begin() == constant, "Const and mutable iterator equality");
    expect(constant != warehouse.end(), "Mutable and const iterator inequality");

    const Warehouse& view = warehouse;
    expect(std::distance(view.begin(), view.end()) == 3, "Const iterator distance");
    expect(view.cbegin() == view.begin() && view.cend() == view.end(), "Const iterator accessors");

    const auto found = std::find_if(view.begin(), view.end(), [](const Product& product) {
        return product.getCategory() == "food";
    });
    expect(found != view.end() && found->getId() == 2, "STL algorithm compatibility");
}

void testTransfers() {
    Warehouse source = createWarehouse();
    Warehouse destination(1);
    Product* const product = source.findById(1);

    product->transferTo(destination);
    expect(source.findById(1) == nullptr && source.getSize() == 2, "Transfer retained source product");
    expect(destination.findById(1) == product && destination.isFull(), "Transfer copied product");
    product->transferTo(destination);
    expect(destination.getSize() == 1, "Self transfer changed warehouse");

    Product* const milk = source.findById(2);
    expectException<std::length_error>([&] { milk->transferTo(destination); }, "Transfer exceeded limit");
    expect(source.findById(2) == milk && destination.getSize() == 1, "Failed transfer changed warehouses");

    product->transferTo(source);
    expect(destination.isEmpty() && source.findById(1) == product, "Repeated transfer lost owner");

    Warehouse duplicate(source);
    expectException<std::invalid_argument>([&] { product->transferTo(duplicate); }, "Transfer accepted duplicate ID");
    expect(source.getSize() == 3 && duplicate.getSize() == 3, "Duplicate transfer changed warehouses");
    expect(source.findById(1) == product, "Duplicate transfer lost original product");

    const auto detached = product->clone();
    expectException<std::logic_error>([&] { detached->transferTo(destination); }, "Detached clone transferred");

    const auto standalone = BreadFactory().createProduct("Standalone", 1, 1, 0.5);
    expectException<std::logic_error>([&] { standalone->transferTo(destination); }, "Unstored product transferred");

    for (const int id : {2, 3, 1}) {
        Product* const stored = source.findById(id);
        stored->transferTo(destination);
        expect(destination.findById(id) == stored, "Transfer of another product type failed");
        destination.removeById(id);
    }

    expect(source.isEmpty() && destination.isEmpty(), "Transfer and removal left products");
}

void testIdExhaustion() {
    const BreadFactory factory;
    const int lastId = std::numeric_limits<int>::max();
    const auto product = factory.createProduct(lastId, "Last", 1, 1, 0.5);
    expect(product->getId() == lastId, "Maximum explicit ID");
    expectException<std::overflow_error>([&] {
        static_cast<void>(factory.createProduct("Overflow", 1, 1, 0.5));
    }, "Product ID overflow accepted");
    expect(product->clone()->getId() == lastId, "Clone failed after ID exhaustion");
}

}

int main() {
    const std::array<std::pair<const char*, void (*)()>, 8> tests{{
        {"Attributes", testAttributes},
        {"Factories", testFactories},
        {"Storage and search", testStorageAndSearch},
        {"Copying", testCopying},
        {"Moving", testMoving},
        {"Iterators", testIterators},
        {"Transfers", testTransfers},
        {"ID exhaustion", testIdExhaustion}
    }};

    int failures = 0;

    for (const auto& test : tests) {
        try {
            test.second();
            std::cout << "PASS: " << test.first << '\n';
        } catch (const std::exception& error) {
            ++failures;
            std::cerr << "FAIL: " << test.first << ": " << error.what() << '\n';
        }
    }

    return failures == 0 ? 0 : 1;
}
