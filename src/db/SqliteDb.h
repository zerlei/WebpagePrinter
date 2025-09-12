#pragma once
#include "../model/PrintedPage.h"
#include "../model/PrinterConfig.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <deque>
#include <memory>
#include <tuple>
class SqliteDb {
  public:
    ~SqliteDb() = default;
    static SqliteDb& instance();

    /**
     * @brief   add a new config. config.id will be set to the database id
     *
     */
    void addConfig(PrinterConfig&) const;
    /**
     * @brief   update Config
     *
     */
    void updateConfig(const PrinterConfig&) const;
    /**
     * @brief   delete a config by id.
     *
     */
    void delConfig(int id) const;
    /**
     * @brief Get the Config By Id object
     *
     * @param id
     * @return PrinterConfig
     */
    PrinterConfig getConfigById(int id) const;
    /**
     * @brief   get all configs.
     *
     * @return std::deque<PrinterConfig>
     */
    std::deque<PrinterConfig> getAllConfigs() const;

    /**
     * @brief   add new page page.id will be set to the database id
     *
     */
    void addPage(PrintedPage&) const;
    /**
     * @brief   update Page
     *
     */
    void updatePage(const PrintedPage&) const;

    /**
     * Retrieves a deque of PrintedPage objects in descending order, paginated by the specified
     * index and size.
     *
     * @param page_index The starting index for pagination.
     * @param page_size The number of items to retrieve per page.
     * @return A deque of PrintedPage objects and count
     */
    std::tuple<std::deque<PrintedPage>, int> getPagesDesc(int page_index, int page_size) const;

  private:
    SqliteDb();
    SqliteDb(const SqliteDb&)                        = delete;
    SqliteDb& operator=(const SqliteDb&)             = delete;
    SqliteDb(SqliteDb&&)                             = delete;
    SqliteDb&                  operator=(SqliteDb&&) = delete;
    QSqlDatabase               db;
    std::unique_ptr<QSqlQuery> query;
    void                       createDb();

    void initDb();

    /**
     * @brief check tables is valid
     * 
     * @return true 
     * @return false 
     */

    bool checkValid();
};
