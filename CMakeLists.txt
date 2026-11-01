cmake_minimum_required(VERSION 3.16)
project(labashop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

add_library(labashop_core
    src/Price.cpp
    src/Quantity.cpp
    src/Weight.cpp
    src/Product.cpp
    src/Electronics.cpp
    src/Clothing.cpp
    src/Food.cpp
    src/ProductFactory.cpp
    src/Warehouse.cpp
)

target_include_directories(labashop_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(labashop_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(labashop src/main.cpp)
target_link_libraries(labashop PRIVATE labashop_core)

if(BUILD_TESTING)
    add_executable(labashop_tests tests/WarehouseTests.cpp)
    target_link_libraries(labashop_tests PRIVATE labashop_core)
    add_test(NAME labashop_tests COMMAND labashop_tests)
endif()
