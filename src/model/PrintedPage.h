#pragma once
#include "../exception/FatalError.h"
#include <QString>
#include <qloggingcategory.h>
struct PrintedPage {
    PrintedPage(const PrintedPage&&) {
        assert(false && "PrintedPage should not be moved");
        throw FatalError("PrintedPage should not be moved");
    };
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
     * @brief 打印时间
     *
     */
    QString time;
    /**
     * @brief 状态
     *
     */
    QString status;
    /**
     * @brief 来源ip
     *
     */
    QString from_ip;
    /**
     * @brief 打印的网页
     *
     */
    QString page_url;
    /**
     * @brief 最终执行命令的返回信息
     *
     */
    QString end_cmd_exec_message;
};

bool inline operator==(const PrintedPage& lhs, const PrintedPage& rhs) {
    return lhs.id == rhs.id && lhs.config_id == rhs.config_id && lhs.time == rhs.time &&
           lhs.status == rhs.status && lhs.from_ip == rhs.from_ip && lhs.page_url == rhs.page_url &&
           lhs.end_cmd_exec_message == rhs.end_cmd_exec_message;
}