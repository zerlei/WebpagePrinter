include("${CMAKE_SOURCE_DIR}/cmake/QtCommon.cmake")
qt_add_executable(WebpagePrinterTool MANUAL_FINALIZATION ${PROJECT_SOURCES})
target_include_directories(WebpagePrinterTool
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/db
        ${CMAKE_SOURCE_DIR}/src/excep
        ${CMAKE_SOURCE_DIR}/src/log
        ${CMAKE_SOURCE_DIR}/src/model
        ${CMAKE_SOURCE_DIR}/src/printer
    )
target_sources(
  WebpagePrinterTool PRIVATE main.cpp 
  connector/HttpServer.cpp
  connector/MsgStation.cpp
  connector/WebsocketServer.cpp
  connector/WebsocketClient.cpp

  db/SqliteDb.cpp

  log/log.cpp

  model/ModelsJson.cpp

  printer/Printer.cpp
  printer/WebRender.cpp

)
target_link_libraries(WebpagePrinterTool PRIVATE Qt6::Core Qt6::Sql
                                                 Qt6::Widgets Qt6::HttpServer Qt6::WebSockets Qt6::PrintSupport Qt6::WebEngineWidgets Qt6::Pdf)
include_directories()
qt_finalize_executable(WebpagePrinterTool)