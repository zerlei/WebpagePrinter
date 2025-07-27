#pragma once
#include "../exception/FatalError.h"
#include <QString>

struct PrinterConfig {
    PrinterConfig(const PrinterConfig&&) {
        assert(false && "PrinterConfig should not be moved");
        throw FatalError("PrintedConfig should not be moved");
    };
    PrinterConfig() = default;
    /**
     * @brief id 唯一配置主键
     *
     */
    int id;
    /**
     * @brief 配置名称
     *
     */
    QString name;
    /**
     * @brief 是否额外保存一张png 图片
     *
     */
    int is_save_png;
    /**
     * @brief 是否使用打印机
     *
     */
    int is_to_printer;
    /**
     * @brief 输出宽度
     *
     */
    int width_mm;
    /**
     * @brief 输出高度
     *
     */
    int height_mm;
    /**
     * @brief 顶部边距
     *
     */
    int top_margin;
    /**
     * @brief 底部边距
     *
     */
    int bottom_margin;
    /**
     * @brief 左边边距
     *
     */
    int left_margin;
    /**
     * @brief 右边边距
     *
     */
    int right_margin;
    /**
     * @brief 打印机名称
     *
     */
    QString printer_name;
    /**
     * @brief 是否使用打印机默认配置
     *
     */
    int is_use_printer_default_config;
    /**
     * @brief 打印机纸张名称
     *
     */
    QString printer_paper_name;
    /**
     * @brief 纸张方向
     *
     */
    QString printer_orientation;
    /**
     * @brief 命令行在打印成功后执行
     *
     */
    QString cmd_at_successs_end;
};
bool inline operator==(const PrinterConfig& a, const PrinterConfig& b) {
    return (a.id == b.id && a.name == b.name && a.is_save_png == b.is_save_png &&
            a.is_to_printer == b.is_to_printer && a.width_mm == b.width_mm &&
            a.height_mm == b.height_mm && a.top_margin == b.top_margin &&
            a.bottom_margin == b.bottom_margin && a.left_margin == b.left_margin &&
            a.right_margin == b.right_margin && a.printer_name == b.printer_name &&
            a.is_use_printer_default_config == b.is_use_printer_default_config &&
            a.printer_paper_name == b.printer_paper_name &&
            a.printer_orientation == b.printer_orientation &&
            a.cmd_at_successs_end == b.cmd_at_successs_end);
}