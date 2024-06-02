cmake_minimum_required(VERSION 3.9)

project(microfmt LANGUAGES CXX)

add_library(microfmt INTERFACE)
target_include_directories(microfmt INTERFACE include)
target_compile_features(microfmt INTERFACE cxx_std_11)

# target_compile_options(microfmt INTERFACE -fanalyzer)
# add_compile_options(-fanalyzer)

option(MICROFMT_BUILD_TESTING "" OFF)
option(MICROFMT_LLVM_COV "" OFF)
if(MICROFMT_LLVM_COV)
  add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
  add_link_options(-fprofile-instr-generate -fcoverage-mapping)
endif()

if(MICROFMT_BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
