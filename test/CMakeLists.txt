include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(googletest)

add_executable(strided-iterator-test strided_iterator_test.cpp)
target_link_libraries(strided-iterator-test PRIVATE strided-iterator gtest gmock gtest_main)

add_executable(const-strided-iterator-test const_strided_iterator_test.cpp)
target_link_libraries(const-strided-iterator-test PRIVATE strided-iterator gtest gmock gtest_main)