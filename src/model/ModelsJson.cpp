#include "ModelsJson.h"
#include "../exception/JsonParseError.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <qjsonobject.h>
QJsonObject toPrintedPageJson(const PrintedPage& page) {
    QJsonObject obj;
    obj["id"]                   = page.id;
    obj["config_id"]            = page.config_id;
    obj["time"]                 = page.time;
    obj["status"]               = page.status;
    obj["from_ip"]              = page.from_ip;
    obj["page_url"]             = page.page_url;
    obj["end_cmd_exec_message"] = page.end_cmd_exec_message;
    return obj;
}

// PrintedPage fromPrintedPageJson(const QString& json) {
//     QJsonParseError parse_error;
//     QJsonDocument   doc = QJsonDocument::fromJson(json.toUtf8(), &parse_error);
//     if (parse_error.error != QJsonParseError::NoError) {
//         throw JsonParseError(json, "Parse error:" + parse_error.errorString());
//     }
//     if (!doc.isNull() && doc.isObject()) {
//         return fromPrintedPageJson(json, doc.object());
//     } else {
//         throw JsonParseError(json, "Parse error: is not an Json object");
//     }
// }
// PrintedPage fromPrintedPageJson(const QString& source_json, const QJsonObject& obj) {
//     PrintedPage page;
//     if (obj["id"].isDouble()) {
//         page.id = obj["id"].toInt();
//     }
//     if (obj["config_id"].isDouble()) {
//         page.config_id = obj["config_id"].toInt();
//     } else {
//         throw JsonParseError(source_json, "Parse error: config_id is not a number");
//     }

//     if (obj["time"].isString()) {
//         page.time = obj["time"].toString();
//     } else {
//         throw JsonParseError(source_json, "Parse error: time is not a string");
//     }

//     if (obj["status"].isString()) {
//         page.status = obj["status"].toString();
//     } else {
//         throw JsonParseError(source_json, "Parse error: status is not a string");
//     }

//     if (obj["from_ip"].isString()) {
//         page.from_ip = obj["from_ip"].toString();
//     } else {
//         throw JsonParseError(source_json, "Parse error: from_ip is not a string");
//     }

//     if (obj["page_url"].isString()) {
//         page.page_url = obj["page_url"].toString();
//     } else {
//         throw JsonParseError(source_json, "Parse error: page_url is not a string");
//     }

//     if (obj["end_cmd_exec_message"].isString()) {
//         page.end_cmd_exec_message = obj["end_cmd_exec_message"].toString();
//     } else {
//         throw JsonParseError(source_json, "Parse error: end_cmd_exec_message is not a string");
//     }
//     return page;
// }
QJsonObject toPrinterConfigJson(const PrinterConfig& config) {
    QJsonObject obj;
    obj["id"]                            = config.id;
    obj["name"]                          = config.name;
    obj["is_save_png"]                   = config.is_save_png;
    obj["is_to_printer"]                 = config.is_to_printer;
    obj["width_mm"]                      = config.width_mm;
    obj["height_mm"]                     = config.height_mm;
    obj["top_margin"]                    = config.top_margin;
    obj["bottom_margin"]                 = config.bottom_margin;
    obj["left_margin"]                   = config.left_margin;
    obj["right_margin"]                  = config.right_margin;
    obj["printer_name"]                  = config.printer_name;
    obj["is_use_printer_default_config"] = config.is_use_printer_default_config;
    obj["printer_paper_name"]            = config.printer_paper_name;
    obj["printer_orientation"]           = config.printer_orientation;
    obj["cmd_at_successs_end"]           = config.cmd_at_successs_end;
    return obj;
}
PrinterConfig fromPrinterConfigJson(const QString& json) {

    QJsonParseError parse_error;
    QJsonDocument   doc = QJsonDocument::fromJson(json.toUtf8(), &parse_error);
    if (parse_error.error != QJsonParseError::NoError) {
        throw JsonParseError(json, "Parse error:" + parse_error.errorString());
    }
    if (!doc.isNull() && doc.isObject()) {
        return fromPrinterConfigJson(json, doc.object());
    } else {
        throw JsonParseError(json, "Parse error: is not an Json object");
    }
}
PrinterConfig fromPrinterConfigJson(const QString& source_json, const QJsonObject& obj) {
    PrinterConfig config;
    if (obj["id"].isDouble()) {
        config.id = obj["id"].toInt();
    }
    if (obj["name"].isString()) {
        config.name = obj["name"].toString();
    } else {
        throw JsonParseError(source_json, "Parse error: name is not a string");
    }

    if (obj["is_save_png"].isBool()) {
        config.is_save_png = obj["is_save_png"].toBool();
    } else {
        throw JsonParseError(source_json, "Parse error: is_save_png is not a bool");
    }

    if (obj["is_to_printer"].isBool()) {
        config.is_to_printer = obj["is_to_printer"].toBool();
    } else {
        throw JsonParseError(source_json, "Parse error: is_to_printer is not a bool");
    }

    if (obj["width_mm"].isDouble()) {
        config.width_mm = obj["width_mm"].toInt();
    } else {
        throw JsonParseError(source_json, "Parse error: width_mm is not a number");
    }

    if (obj["height_mm"].isDouble()) {
        config.height_mm = obj["height_mm"].toInt();
    } else {
        throw JsonParseError(source_json, "Parse error: height_mm is not a number");
    }

    if (obj["top_margin"].isDouble()) {
        config.top_margin = obj["top_margin"].toInt();
    } else {
        throw JsonParseError(source_json, "Parse error: top_margin is not a number");
    }

    if (obj["bottom_margin"].isDouble()) {
        config.bottom_margin = obj["bottom_margin"].toInt();
    } else {
        throw JsonParseError(source_json, "Parse error: bottom_margin is not a number");
    }

    if (obj["left_margin"].isDouble()) {
        config.left_margin = obj["left_margin"].toInt();
    } else {
        throw JsonParseError(source_json, "Parse error: left_margin is not a number");
    }

    if (obj["right_margin"].isDouble()) {
        config.right_margin = obj["right_margin"].toInt();
    } else {
        throw JsonParseError(source_json, "Parse error: right_margin is not a number");
    }

    if (obj["printer_name"].isString()) {
        config.printer_name = obj["printer_name"].toString();
    } else {
        throw JsonParseError(source_json, "Parse error: printer_name is not a string");
    }

    if (obj["is_use_printer_default_config"].isBool()) {
        config.is_use_printer_default_config = obj["is_use_printer_default_config"].toBool();
    } else {
        throw JsonParseError(source_json,
                             "Parse error: is_use_printer_default_config is not a bool");
    }

    if (obj["printer_paper_name"].isString()) {
        config.printer_paper_name = obj["printer_paper_name"].toString();
    } else {
        throw JsonParseError(source_json, "Parse error: printer_paper_name is not a string");
    }

    if (obj["printer_orientation"].isString()) {
        config.printer_orientation = obj["printer_orientation"].toString();
    } else {
        throw JsonParseError(source_json, "Parse error: printer_orientation is not a string");
    }

    if (obj["cmd_at_successs_end"].isString()) {
        config.cmd_at_successs_end = obj["cmd_at_successs_end"].toString();
    } else {
        throw JsonParseError(source_json, "Parse error: cmd_at_successs_end is not a string");
    }
    return config;
}