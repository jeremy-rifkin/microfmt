include(CTest)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE microfmt)

add_executable(perf perf.cpp)
target_link_libraries(perf PRIVATE microfmt)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY "https://github.com/google/googletest.git"
  GIT_TAG        f8d7d77c06936315286eb55f8de22cd23c188571 # v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test test.cpp)
target_link_libraries(test GTest::gtest_main microfmt)
add_test(NAME test COMMAND test)

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

add_executable(bench bench.cpp)
target_link_libraries(bench PRIVATE microfmt benchmark::benchmark fmt::fmt)
target_compile_features(bench PRIVATE cxx_std_20)
