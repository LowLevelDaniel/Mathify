cmake_minimum_required(VERSION 3.22.1) # my version
project(mathify VERSION 0.0.0)


include(GNUInstallDirs)

add_library(mathify SHARED
  src/mathify.c
  src/string.c
  src/io.c
)
add_library(mathify::mathify ALIAS mathify)

target_link_libraries(mathify PRIVATE ${LINK_LIBS})
target_compile_definitions(mathify PUBLIC ${COM_ARGS})

include_directories(.)

target_include_directories(
  mathify
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS mathify
  EXPORT mathifyTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Include the CMakePackageConfigHelpers module
include(CMakePackageConfigHelpers)

# Set some properties for your library target
set_property(TARGET mathify PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET mathify PROPERTY SOVERSION 1)
set_property(TARGET mathify PROPERTY
  mNTERFACE_Mathify_MAJOR_VERSION 1)
set_property(TARGET mathify APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING mathify_MAJOR_VERSION
)

# Generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/mathifyConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY AnyNewerVersion
)

# Create the config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/mathifyConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/mathifyConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mathify
)

# Install the config and version files
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/mathifyConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/mathifyConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mathify
)

# Export the targets for the build tree
export(EXPORT mathifyTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/mathifyTargets.cmake"
  NAMESPACE mathify::
)

# Install the export set for the install tree
install(EXPORT mathifyTargets
  FILE mathifyTargets.cmake
  NAMESPACE mathify::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mathify
)


