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

            try {
                data_pack.config = SqliteDb::instance().getConfigById(data_pack.page.config_id);
            } catch (const SqliteOpError& e) {
                data_pack.page.error_message = e.what();
                throw e;
            }
            data_pack.page.status = step_str[step];
            SqliteDb::instance().updatePage(data_pack.page);
            next.work(data_pack);
        } catch (const SqliteOpError& e) {
            data_pack.setPromiseValue(RespError::toJsonObject(data_pack.uid, e.what()));
        }
    }
};
