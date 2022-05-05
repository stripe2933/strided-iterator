cmake_minimum_required(VERSION 3.0.0)
project(strided-iterator VERSION 0.1.0)

add_subdirectory(test)

add_library(strided-iterator INTERFACE)
target_include_directories(strided-iterator INTERFACE ${CMAKE_SOURCE_DIR}/include)
target_compile_options(strided-iterator INTERFACE -Wall -Werror)
target_compile_features(strided-iterator INTERFACE cxx_std_20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

enable_testing()
add_test(strided-iterator-test ${CMAKE_BUILD_DIR}/test/strided-iterator-test)
add_test(const-strided-iterator-test ${CMAKE_BUILD_DIR}/test/const-strided-iterator-test)