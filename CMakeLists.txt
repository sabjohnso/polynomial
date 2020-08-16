cmake_minimum_required(VERSION 3.12)

project(polynomial VERSION 0.1.0 LANGUAGES C CXX)

option(polynomial_BUILD_TESTING "Build the polynomial tests" ON)

include(GNUInstallDirs)
set(polynomial_INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR})
set(polynomial_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
set(polynomial_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})
set(polynomial_INSTALL_CONFDIR ${polynomial_INSTALL_LIBDIR}/cmake/polynomial-${PROJECT_VERSION})

list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)
include(polynomial_deps)

add_subdirectory(polynomial)

if(polynomial_BUILD_TESTING)
  include(CTest)
  if(BUILD_TESTING)
    add_subdirectory(polynomial_testing)
  endif()
endif()

install(EXPORT polynomial_EXPORTS
  NAMESPACE polynomial::
  FILE polynomial-exports.cmake
  DESTINATION ${polynomial_INSTALL_CONFDIR})

configure_file(polynomial-config.cmake.in polynomial-config.cmake)
install(FILES ${PROJECT_BINARY_DIR}/polynomial-config.cmake
  DESTINATION ${polynomial_INSTALL_CONFDIR})








