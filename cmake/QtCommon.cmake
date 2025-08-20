include_guard()
# qt 非debug模式下，qt的调试行数信息等，将会被取消，使用这个把它加回来
add_definitions(-DQT_MESSAGELOGCONTEXT)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 REQUIRED COMPONENTS Core Sql Test Widgets HttpServer WebSockets PrintSupport WebEngineWidgets)
qt_standard_project_setup()
