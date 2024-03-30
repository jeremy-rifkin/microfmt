include(CTest)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE microfmt)

add_executable(bench bench.cpp)
target_link_libraries(bench PRIVATE microfmt)

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
