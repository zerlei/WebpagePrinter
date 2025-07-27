cmake_minimum_required(VERSION 3.16)
project(
  WebpagePrinterTool
  VERSION 0.1
  LANGUAGES CXX)
add_subdirectory(docs)
add_subdirectory(src)
enable_testing()
include(CTest)
add_subdirectory(tests)