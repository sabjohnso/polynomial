configure_file(config.hpp.in config.hpp)

add_library(polynomial INTERFACE)
target_include_directories(polynomial
  INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${polynomial_INSTALL_INCLUDEDIR}>)

add_library(polynomial::polynomial ALIAS polynomial)
  
install(TARGETS polynomial
  EXPORT polynomial_EXPORTS
  RUNTIME DESTINATION ${polynomial_INSTALL_BINDIR}
  LIBRARY DESTINATION ${polynomial_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${polynomial_INSTALL_LIBDIR})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
  DESTINATION ${polynomial_INSTALL_INCLUDEDIR})
