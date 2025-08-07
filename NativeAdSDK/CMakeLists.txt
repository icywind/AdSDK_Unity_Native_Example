cmake_minimum_required(VERSION 3.10)
project(AdSDK)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(AdSDK SHARED
    AdSDK.cpp
)

target_include_directories(AdSDK PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})