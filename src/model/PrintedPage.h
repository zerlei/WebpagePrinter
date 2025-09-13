#pragma once
// #include "../excep/FatalError.h"
#include <QString>
// #include <qloggingcategory.h>
struct PrintedPage {
    // PrintedPage(const PrintedPage&&) {
    //     assert(false && "PrintedPage should not be moved");
    //     throw FatalError("PrintedPage should not be moved");
    // };
    PrintedPage() = default;
    /**
     * @brief 主键id
     *
     */
    int id;
    /**
     * @brief 配置id
     *
     */
    int config_id;

    /**
     * @brief PAGE_LOADED(页面加载完毕)  or JS_REQUEST(window.print()被调用)
     *
     */
    QString page_loaded_or_js_request;
    /**
     * @brief 打印时间
     *
     */
    QString time;
    /**
     * @brief 状态 STEP
     *
     */
    QString status;
    /**
     * @brief 来源ip
     *
     */
    QString from_ip;
    /**
     * @brief .png|.pdf 文件路径位j置
     *
     */
    QString page_file_path;
    /**
     * @brief 打印的网页, 如果是磁盘上的文件,请添加 `file://`前缀
     *
     */
    QString page_url;
    /**
     * @brief 最终执行命令的返回状态
     *
     */
    int end_cmd_exec_status{-1};
    /**
     * @brief 最终执行命令的返回信息
     *
     */
    QString end_cmd_exec_message;
    /**
     * @brief 错误信息
     *
     */
    QString error_message;
};

bool inline operator==(const PrintedPage& lhs, const PrintedPage& rhs) {
    return lhs.id == rhs.id && lhs.config_id == rhs.config_id && lhs.time == rhs.time &&
           lhs.status == rhs.status && lhs.from_ip == rhs.from_ip && lhs.page_url == rhs.page_url &&
           lhs.page_file_path == rhs.page_file_path &&
           lhs.page_loaded_or_js_request == rhs.page_loaded_or_js_request &&
           lhs.end_cmd_exec_status == rhs.end_cmd_exec_status &&
           lhs.end_cmd_exec_message == rhs.end_cmd_exec_message;
}
