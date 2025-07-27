#include "PrintedPage.h"
#include "PrinterConfig.h"


/**
 * Converts a PrintedPage object to its JSON representation.
 * @param page The PrintedPage object to convert
 * @return JSON string representation of the PrintedPage
 */
QString toPrintedPageJson(const PrintedPage& page);
/**
 * @brief Converts a JSON string into a PrintedPage object.
 * @param json The JSON string representing the PrintedPage data.
 * @return The deserialized PrintedPage object.
 */
PrintedPage fromPrintedPageJson(const QString& json);

/**
 * Converts a PrinterConfig object to its JSON string representation.
 *
 * @param config The PrinterConfig object to convert.
 * @return QString JSON string representation of the PrinterConfig.
 */
QString toPrinterConfigJson(const PrinterConfig& config);
/**
 * @brief Converts a JSON string to a PrinterConfig object.
 *
 * @param json The JSON string to be converted.
 * @return PrinterConfig The resulting PrinterConfig object.
 */
PrinterConfig fromPrinterConfigJson(const QString& json);