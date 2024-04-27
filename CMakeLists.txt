cmake_minimum_required(VERSION 3.9)

project(microfmt LANGUAGES CXX)

add_library(microfmt INTERFACE)
target_include_directories(microfmt INTERFACE include)
target_compile_features(microfmt INTERFACE cxx_std_11)

option(MICROFMT_BUILD_TESTING "" OFF)

if(MICROFMT_BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
