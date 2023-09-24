cmake_minimum_required(VERSION 3.20)

project(TypeListTests)

add_executable(tests tests.cpp)

target_compile_features(tests PRIVATE cxx_std_20)
set_target_properties(tests PROPERTIES CXX_EXTENSIONS OFF)