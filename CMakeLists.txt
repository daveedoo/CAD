cmake_minimum_required(VERSION 3.8)

project("CAD")

configure_file(Config.h.in Config.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory("src")
