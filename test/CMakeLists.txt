include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY "https://github.com/google/googletest.git"
  GIT_TAG        f8d7d77c06936315286eb55f8de22cd23c188571 # v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(BENCHMARK_ENABLE_TESTING OFF)
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG 344117638c8ff7e239044fd0fa7085839fc03021 # v1.8.3
)
FetchContent_MakeAvailable(googlebenchmark)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG e69e5f977d458f2650bb346dadf2ad30c5320281 # v10.2.1
)
FetchContent_MakeAvailable(fmt)

include(CTest)

target_compile_options(
  microfmt
  INTERFACE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Werror -Wshadow -Wundef -Wpedantic>
  $<$<CXX_COMPILER_ID:GNU>:-Wuseless-cast -Wnonnull-compare>
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

if(CMAKE_GENERATOR STREQUAL "Ninja")
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
  endif()
  if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
  elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics")
  endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

add_executable(example example.cpp)
target_link_libraries(example PRIVATE microfmt)
target_compile_features(example PRIVATE cxx_std_20)

if(NOT MSVC)
  add_executable(perf perf.cpp)
  target_link_libraries(perf PRIVATE microfmt)
  target_compile_features(perf PRIVATE cxx_std_20)
endif()

add_executable(unittests unittests.cpp)
target_link_libraries(unittests GTest::gtest_main microfmt)
target_compile_features(unittests PRIVATE cxx_std_20)
add_test(NAME unittests COMMAND unittests)

add_executable(unittests_cpp11 unittests.cpp)
target_link_libraries(unittests_cpp11 GTest::gtest_main microfmt)
set_target_properties(unittests_cpp11 PROPERTIES CXX_STANDARD 11)
add_test(NAME unittests_cpp11 COMMAND unittests_cpp11)

add_executable(bench bench.cpp)
target_link_libraries(bench PRIVATE microfmt benchmark::benchmark fmt::fmt)
target_compile_features(bench PRIVATE cxx_std_20)
