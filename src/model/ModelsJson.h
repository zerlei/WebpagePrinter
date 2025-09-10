#include "PrintedPage.h"
#include "PrinterConfig.h"
#include <qjsonobject.h>

/**
 * Converts a PrintedPage object to its JSON representation.
 * @param page The PrintedPage object to convert
 * @return Json Object
 */
QJsonObject toPrintedPageJson(const PrintedPage& page);
/**
 * @brief Converts a JSON string into a PrintedPage object.
 * @param json The JSON string representing the PrintedPage data.
 * @return The deserialized PrintedPage object.
 */

PrintedPage fromPrintedPageJson(const QString& json);

/**
 * @brief converts a JSON object to a PrintedPage object.
 *
 * @param json source json
 * @param obj object
 * @return PrintedPage
 */
/**
 * @brief 从JSON字符串和QJsonObject对象构造PrintedPage对象。
 *
 * 该函数用于将JSON格式的数据转换为PrintedPage对象。通常用于从网络或文件中读取JSON数据后，
 * 将其转换为程序内部可操作的对象。
 *
 * @param json 包含PrintedPage数据的JSON字符串。
 * @param obj 包含PrintedPage数据的QJsonObject对象。
 * @return PrintedPage 转换后的PrintedPage对象。
 */
PrintedPage fromPrintedPageJson(const QString& json, const QJsonObject& obj);

/**
 * Converts a PrinterConfig object to its JSON string representation.
 *
 * @param config The PrinterConfig object to convert.
 * @return Json Object
 */
QJsonObject toPrinterConfigJson(const PrinterConfig& config);
/**
 * @brief Converts a JSON string to a PrinterConfig object.
 *
 * @param json The JSON string to be converted.
 * @return PrinterConfig The resulting PrinterConfig object.
 */
PrinterConfig fromPrinterConfigJson(const QString& json);
/**
 * @brief Converts a JSON object to a PrinterConfig object.
 *
 * @param source_json
 * @param obj obj
 * @return PrinterConfig
 */
PrinterConfig fromPrinterConfigJson(const QString& source_json, const QJsonObject& obj);