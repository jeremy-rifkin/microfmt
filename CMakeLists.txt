cmake_minimum_required(VERSION 3.9)

project(microfmt)

add_library(microfmt INTERFACE)
target_include_directories(microfmt INTERFACE include)

option(MICROFMT_BUILD_TESTING "" OFF)

if(MICROFMT_BUILD_TESTING)
  add_compile_options(-Wall -Wextra -Wshadow)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
  endif()
  add_subdirectory(test)
endif()
