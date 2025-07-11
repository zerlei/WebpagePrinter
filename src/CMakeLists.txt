include("${CMAKE_SOURCE_DIR}/cmake/QtCommon.cmake")
qt_add_executable(WebpagePrinterTool MANUAL_FINALIZATION ${PROJECT_SOURCES})

target_sources(
  WebpagePrinterTool PRIVATE main.cpp log.h # src/model/model.h # src/db/db.cpp
)
target_link_libraries(WebpagePrinterTool PRIVATE Qt6::Core Qt6::Sql
                                                 Qt6::Widgets)
include_directories()
# target_include_directories(WebpagePrinterTool
#                            PRIVATE $ENV{QT6_BASE_PATH}/include)
qt_finalize_executable(WebpagePrinterTool)
