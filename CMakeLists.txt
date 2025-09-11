cmake_minimum_required(VERSION 3.16)
project(
  WebpagePrinterTool
  VERSION 0.1
  LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
if(NOT DEFINED EXE)
  set(EXE
      OFF
      CACHE BOOL)
endif()
if(NOT DEFINED DOCS)
  set(DOCS
      OFF
      CACHE BOOL)
endif()
if(NOT DEFINED TESTS)
  set(TESTS
      OFF
      CACHE BOOL)
endif()
if(EXE)
  add_subdirectory(src)
endif()
if(DOCS)
  add_subdirectory(docs)
endif()
if(TESTS)
  enable_testing()
  include(CTest)
  add_subdirectory(tests)
endif()
