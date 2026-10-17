cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

project (Raylib_Test)

set(CMAKE_BUILD_TYPE "DEBUG")

find_package(raylib CONFIG REQUIRED)


add_executable(GUI_Wizard main.cpp)


target_link_libraries(GUI_Wizard PRIVATE raylib)
