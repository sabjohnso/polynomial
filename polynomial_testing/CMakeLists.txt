find_package(GTest)

macro(polynomial_add_test test_name exe_name)
  add_executable(${exe_name} ${ARGN})
  target_link_libraries(${exe_name} PRIVATE
    polynomial::header
    GTest::gtest_main)
  add_test(${test_name} ${exe_name})
endmacro()

polynomial_add_test(polynomial polynomial_test polynomial_test.cpp)
