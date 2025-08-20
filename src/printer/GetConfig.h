#include "../db/SqliteDb.h"
#include "../exception/SqliteOpError.h"
#include "../model/WebInterface.h"
#include "DataPack.h"
template <typename T>
class GetConfig {
    static constexpr STEP step = GET_CONFIG;

    T    next;
    void work(PrinterDataPack& data_pack) {
        try {
            data_pack.config      = SqliteDb::instance().getConfigById(data_pack.page.config_id);
            data_pack.page.status = step_str[step];
            SqliteDb::instance().updatePage(data_pack.page);
            next.work(data_pack);
        } catch (const SqliteOpError& e) {
            data_pack.p.set_value(RespError::toJsonObject(data_pack.uid, e.what()));
        }
    }
};
