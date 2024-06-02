cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(banking_lib)

add_library(account_lib STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Account.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(transaction_lib STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Transaction.cpp)
target_link_libraries(transaction_lib account_lib)