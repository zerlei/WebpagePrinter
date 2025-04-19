# qt 非debug模式下，qt的调试行数信息等，将会被取消，使用这个把它加回来
add_definitions(-DQT_MESSAGELOGCONTEXT)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Sql)

qt_add_executable(WebpagePrinterTool MANUAL_FINALIZATION ${PROJECT_SOURCES})

target_sources(
  WebpagePrinterTool PRIVATE main.cpp log.h

  # src/model/model.h
                             # src/db/db.cpp
)
target_link_libraries(WebpagePrinterTool PRIVATE Qt6::Core  Qt6::Sql)

qt_finalize_executable(WebpagePrinterTool)
