find_program(DOXYGEN_PATH doxygen REQUIRED)

if(DOXYGEN_PATH_NOTFOUND)
  message(FATAL_ERROR "Doxygen is needed to build the
documentation. Please install it on your system")
else()
  message(STATUS "Doxygen found.")
  set(PLANTUML_JAR_PATH $ENV{PLANTUML_JAR_PATH})
  cmake_policy(SET CMP0135 NEW)
  include(FetchContent)
  FetchContent_Declare(
    doxygen-awesome-css
    URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip
  )
  FetchContent_MakeAvailable(doxygen-awesome-css)

  # Save the location the files were cloned into This allows us to get the path
  # to doxygen-awesome.css
  FetchContent_GetProperties(doxygen-awesome-css SOURCE_DIR AWESOME_CSS_DIR)

  # Generate the Doxyfile
  set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

  add_custom_target(
    docs
    COMMAND ${DOXYGEN_PATH} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMKAE_CURRENT_BINARY_DIR})
endif()
