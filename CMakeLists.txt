cmake_minimum_required(VERSION 3.16)
project(
  WebpagePrinterTool
  VERSION 0.1
  LANGUAGES CXX)
if(NOT DEFINED ONLY_EXE)
  set(ONLY_EXE
      OFF
      CACHE BOOL "JUST BUILD EXE")
endif()
add_subdirectory(src)
if(NOT ONLY_EXE)
  add_subdirectory(docs)
  enable_testing()
  include(CTest)
  add_subdirectory(tests)
endif()

