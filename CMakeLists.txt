cmake_minimum_required(VERSION 3.10)
project(CommandEngine)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(CommandEngine
    main.cpp
)

target_compile_features(CommandEngine PRIVATE cxx_std_17)