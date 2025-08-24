#include <QString>
#include <array>
#include <cstddef>
#include <string_view>
template <size_t N>
struct ConstexprTableStr {
    mutable char data[N]{};
    constexpr ConstexprTableStr() = default;
    consteval auto addData(std::string_view str, size_t& current_size) const {
        for (size_t i = 0; i < str.size(); ++i) {
            data[current_size++] = str[i];
        }
    }
    consteval auto addData(size_t index, size_t& current_size) const {
        data[current_size++] = static_cast<char>(index + 48);
    }
};

struct PrinterConfigToTable {
    std::string_view field_name;
    std::string_view field_data_type;
};

constexpr auto printer_config_table_helper = std::to_array({
    PrinterConfigToTable{.field_name = "id", .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "name", .field_data_type = "TEXT"},
    PrinterConfigToTable{.field_name = "is_save_png", .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "is_to_printer", .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "width_mm", .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "height_mm", .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "top_margin", .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "bottom_margin", .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "left_margin", .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "right_margin", .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "printer_name", .field_data_type = "TEXT"},
    PrinterConfigToTable{.field_name      = "is_use_printer_default_config",
                         .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "printer_paper_name", .field_data_type = "TEXT"},
    PrinterConfigToTable{.field_name = "printer_orientation", .field_data_type = "TEXT"},
    PrinterConfigToTable{.field_name = "cmd_at_end", .field_data_type = "TEXT"},
});

constexpr auto printed_page_table_helper = std::to_array({
    PrinterConfigToTable{.field_name = "id", .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "config_id", .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "time", .field_data_type = "TEXT"},
    PrinterConfigToTable{.field_name = "status", .field_data_type = "TEXT"},
    PrinterConfigToTable{.field_name = "from_ip", .field_data_type = "TEXT"},
    PrinterConfigToTable{.field_name = "page_url", .field_data_type = "TEXT"},
    PrinterConfigToTable{.field_name = "page_file_path", .field_data_type = "TEXT"},
    PrinterConfigToTable{.field_name = "end_cmd_exec_status", .field_data_type = "INTEGER"},
    PrinterConfigToTable{.field_name = "end_cmd_exec_message", .field_data_type = "TEXT"},
    PrinterConfigToTable{.field_name = "error_message", .field_data_type = "TEXT"},
    PrinterConfigToTable{.field_name = "page_loaded_or_js_request", .field_data_type = "TEXT"},
});
template <size_t N>
consteval auto getCreateTableStrSize(const std::array<PrinterConfigToTable, N>& arr) {
    size_t size = 0;
    for (const auto& item : arr) {
        size +=
            item.field_name.size() + item.field_data_type.size() + 5; // +2 for space and newline
    }
    return size;
}
template <size_t N>
consteval auto getUpdateTableStrSize(const std::array<PrinterConfigToTable, N>& arr) {
    size_t size  = 0;
    int    index = 1;
    for (; index < arr.size(); ++index) {
        auto& item = arr[index];
        size += item.field_name.size();
        if (item.field_data_type == "INTEGER") {
            size += 5;
        } else {
            size += 7;
        }
        if (index < 10) {
            size += 2;
        } else {
            size += 3;
        }
    }
    return size;
}
template <size_t N>
consteval auto getSelectTableStrSize(const std::array<PrinterConfigToTable, N>& arr) {
    size_t size = 0;
    for (const auto& item : arr) {
        size += item.field_name.size() + 2; // +2 for space and newline
    }
    return size;
}

// 不要改变顺序注意 SqliteDb.cpp 的checkValid实现
constexpr auto             tables_name     = std::to_array({"printed_page", "printer_config"});
constexpr std::string_view page_head_str   = "CREATE TABLE \"printed_page\" (\n";
constexpr std::string_view config_head_str = "CREATE TABLE \"printer_config\" (\n";
constexpr std::string_view tail_str        = "PRIMARY KEY(\"id\" AUTOINCREMENT)\n)";
consteval auto             creatStrPageClass() {
    // for /0 +1
    return ConstexprTableStr<getCreateTableStrSize(printed_page_table_helper) + 1 +
                                         page_head_str.size() + tail_str.size()>{};
}
consteval auto updateStrPageClass() {
    return ConstexprTableStr<getCreateTableStrSize(printed_page_table_helper) + 1>{};
}
consteval auto selectStrPageClass() {
    return ConstexprTableStr<getSelectTableStrSize(printed_page_table_helper) + 1>{};
}
consteval auto creatStrConfigClass() {
    return ConstexprTableStr<getCreateTableStrSize(printer_config_table_helper) + 1 +
                             config_head_str.size() + tail_str.size()>{};
}
consteval auto updateStrConfigClass() {
    return ConstexprTableStr<getCreateTableStrSize(printer_config_table_helper) + 1>{};
}
consteval auto selectStrConfigClass() {
    return ConstexprTableStr<getSelectTableStrSize(printer_config_table_helper) + 1>{};
}
template <size_t N>
consteval auto getCreateTableStr(const std::array<PrinterConfigToTable, N>& arr, auto dat) {

    size_t current_size = 0;
    if (N == printer_config_table_helper.size()) {
        dat.addData(config_head_str, current_size);
    } else {
        dat.addData(page_head_str, current_size);
    }
    for (const auto& item : arr) {
        dat.addData("\"", current_size);
        dat.addData(item.field_name, current_size);
        dat.addData("\" ", current_size);
        dat.addData(item.field_data_type, current_size);
        dat.addData(",\n", current_size);
    }
    dat.addData(tail_str, current_size);
    dat.addData("\0", current_size);
    return dat;
}
consteval auto getUpdateTableStr(auto arr, auto dat) {

    size_t current_size = 0;
    for (size_t i = 1; i < arr.size(); ++i) {
        dat.addData(arr[i].field_name, current_size);
        dat.addData(" = ", current_size);
        if (arr[i].field_data_type == "INTEGER") {
            dat.addData("%", current_size);
            if (i < 10) {
                dat.addData(i, current_size);
            } else {
                dat.addData(i / 10, current_size);
                dat.addData(i % 10, current_size);
            }
        } else {
            dat.addData("'%", current_size);
            if (i < 10) {
                dat.addData(i, current_size);
            } else {
                dat.addData(i / 10, current_size);
                dat.addData(i % 10, current_size);
            }
            dat.addData("'", current_size);
        }
        if (i != arr.size() - 1) {
            dat.addData(",\n", current_size);
        } else {
            dat.addData("\n", current_size);
        }
    }
    dat.addData("\0", current_size);
    return dat;
}
consteval auto getSelectTableStr(auto arr, auto dat) {
    size_t current_size = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        dat.addData(arr[i].field_name, current_size);
        if (i != arr.size() - 1) {
            dat.addData(",\n", current_size);
        } else {
            dat.addData("\n", current_size);
        }
    }
    dat.addData("\0", current_size);
    return dat;
}
constexpr auto create_page_table_str =
    getCreateTableStr(printed_page_table_helper, creatStrPageClass());

/**
 * @brief only for QString, only fields, No Update etc.
 *
 */
constexpr auto update_page_table_str =
    getUpdateTableStr(printed_page_table_helper, updateStrPageClass());
/**
 * @brief  only fields, No Select etc.
 *
 */
constexpr auto select_page_table_str =
    getSelectTableStr(printed_page_table_helper, selectStrPageClass());
constexpr auto create_config_table_str =
    getCreateTableStr(printer_config_table_helper, creatStrConfigClass());
/**
 * @brief only for QString, only fields, No Update etc.
 *
 */
constexpr auto update_config_table_str =
    getUpdateTableStr(printer_config_table_helper, updateStrConfigClass());
/**
 * @brief  only fields, No Select etc.
 *
 */
constexpr auto select_config_table_str =
    getSelectTableStr(printer_config_table_helper, selectStrConfigClass());