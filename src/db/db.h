#ifndef PrintDatabase_H
#define PrintDatabase_H

#include "../model/model.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <json/value.h>
#include <string>
#include <tuple>
#include <vector>


class PrintDatabase : QObject {
  Q_OBJECT
public:
  PrintDatabase();

  std::tuple<bool, std::string> printerConfigInsert(const PrinterConfig &pc_);
  std::tuple<bool, std::string> printerConfigDel(const int Id_);
  std::tuple<bool, std::string> printerConfigUpdate(const PrinterConfig &pc_);
  std::vector<PrinterConfigPtr> printerConfigQueryById(int Id_ = -1);
  std::vector<PrinterConfigPtr> printerConfigQueryByName(const QString &Name_);

  std::tuple<int,std::vector<PrintedPage>>  printedPageQuery(int page_size_, int page_index_);
  bool printedPageInsert(const PrintedPage &pp_);

  std::string getWebsocketUrl();
  bool insertOrUpdateWebsocketUrl(const std::string& );

private:
  QSqlDatabase _db;
  std::unique_ptr<QSqlQuery> _query;
};

#endif // PrintDatabase_H
