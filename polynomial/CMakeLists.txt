configure_file(config.hpp.in config.hpp)

add_library(polynomial_header INTERFACE)
target_include_directories(polynomial_header
  INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${polynomial_INSTALL_INCLUDEDIR}>)

add_library(polynomial::header ALIAS polynomial_header)
  
install(TARGETS polynomial_header
  EXPORT polynomial_EXPORTS
  RUNTIME DESTINATION ${polynomial_INSTALL_BINDIR}
  LIBRARY DESTINATION ${polynomial_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${polynomial_INSTALL_LIBDIR})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
  polynomial.hpp
  DESTINATION ${polynomial_INSTALL_INCLUDEDIR}/polynomial)

install(DIRECTORY details
  DESTINATION ${polynomial_INSTALL_INCLUDEDIR}/polynomial
  PATTERN "*.hpp")

