include("${CMAKE_SOURCE_DIR}/cmake/QtCommon.cmake")


qt_add_executable(TEST_LOG)
target_sources(TEST_LOG PRIVATE TEST_log.cpp  ../src/log/log.cpp)
target_link_libraries(TEST_LOG PRIVATE Qt6::Test)
add_test(NAME TEST_LOG COMMAND TEST_LOG)

qt_add_executable(TEST_CONFIG)
target_compile_definitions(TEST_CONFIG PRIVATE
    BASE_DIR="/.WebpagePrinter_TESTCONFIG/"
)
target_sources(TEST_CONFIG PRIVATE TEST_InitConfig.cpp)
target_link_libraries(TEST_CONFIG PRIVATE Qt6::Test)
add_test(NAME TEST_CONFIG COMMAND TEST_CONFIG)

qt_add_executable(TEST_SqliteDb)
target_compile_definitions(TEST_SqliteDb PRIVATE
    BASE_DIR="/.WebpagePrinter_TESTSqliteDb/"
)
target_sources(TEST_SqliteDb PRIVATE TEST_SqliteDb.cpp ../src/db/SqliteDb.cpp ../src/log/log.cpp)
target_link_libraries(TEST_SqliteDb PRIVATE Qt6::Test Qt6::Sql)
add_test(NAME TEST_SqliteDb COMMAND TEST_SqliteDb)

qt_add_executable(TEST_Print)
target_compile_definitions(TEST_Print PRIVATE
    PrintName="CUPS-PDF"
    PaperName="A3"
    BASE_DIR="/.WebpagePrinter_TESTPrint/"
)
target_sources(TEST_Print PRIVATE 
TEST_Print.cpp 
../src/db/SqliteDb.cpp 
../src/log/log.cpp
../src/model/ModelsJson.cpp
../src/printer/Printer.cpp
../src/printer/WebRender.cpp
)
target_link_libraries(TEST_Print PRIVATE Qt6::Test Qt6::Sql
                                                 Qt6::Widgets Qt6::HttpServer Qt6::WebSockets Qt6::PrintSupport Qt6::WebEngineWidgets Qt6::Pdf)
add_test(NAME TEST_Print COMMAND TEST_Print)
#  注意测试的运行环境,在nix 上这是正确的,在宿主机上,需要注释掉下面这一行
set_tests_properties(TEST_Print PROPERTIES
    ENVIRONMENT "QT_XCB_GL_INTEGRATION=none;QT_QUICK_BACKEND=software"
)