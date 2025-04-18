#include "db.h"
#include "base.h"
#include <QApplication>
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <format>
#include <iostream>
#include <qdebug.h>
#include <tuple>
#include <vector>

PrintDatabase::PrintDatabase() {
  _db = QSqlDatabase::addDatabase("QSQLITE");
  _db.setDatabaseName(QCoreApplication::applicationDirPath() +
                      "/PrinterInfo.db");

  if (_db.open()) {
    _query = std::make_unique<QSqlQuery>(_db);

  } else {
    QApplication::quit();
  }
}

std::tuple<bool, std::string>
PrintDatabase::printerConfigInsert(const PrinterConfig &pc) {

  try {
    QString queryExit =
        QString(R"(SELECT 1 FROM printer_config where Name ='%1';)")
            .arg(QString::fromStdString(pc.Name));
    if (_query->exec(queryExit)) {
      if (_query->next()) {
        return std::make_tuple(false, "不能有重复的名称！");
      }
    }
    auto insert_sql = std::format(
        R"(
                  INSERT INTO printer_config (
                                  Name,
                                  PrinterName,
                                  TopMargin,
                                  BottomMargin,
                                  LeftMargin,
                                  RightMargin,
                                  Orientation,
                                  PaperName,
                                  SaveType,
                                  PaperWidthInmm,
                                  PaperHeightInmm
                              )
                              VALUES (
                              '{}',
                              '{}',
                               {},
                               {},
                               {},
                               {},
                              '{}',
                              '{}',
                              '{}',
                               {},
                               {}
                              );
            )",
        pc.Name, pc.PrinterName, pc.TopMargin, pc.BottomMargin, pc.LeftMargin,
        pc.RightMargin, pc.Orientation, pc.PaperName, pc.SaveType,
        pc.PaperWidthInmm, pc.PaperHeightInmm);
    if (_query->exec(QString::fromStdString(insert_sql))) {
      return std::make_tuple(true, "");
    }
    throw 1;

  } catch (...) {
    return std::make_tuple(false, "未知的错误发生了~");
  }
}

std::tuple<bool, std::string> PrintDatabase::printerConfigDel(const int Id) {

  try {
    QString delSql =
        QString(R"(delete from printer_config where Id = %1;)").arg(Id);

    if (_query->exec(delSql)) {
      return std::make_tuple(true, "");
    }
    return std::make_tuple(false, "删除失败！");

  } catch (...) {
    return std::make_tuple(false, "未知的错误发生了~");
  }
}

std::tuple<bool, std::string>
PrintDatabase::printerConfigUpdate(const PrinterConfig &pc) {
  try {
    auto update_sql = std::format(R"(
                update printer_config
                       set Name = '{}',
                       PrinterName = '{}',
                       PaperName = '{}',
                       Orientation = '{}',
                       TopMargin = {},
                       BottomMargin = {},
                       LeftMargin = {},
                       RightMargin = {},
                       SaveType = '{}',
                       PaperWidthInmm = {},
                       PaperHeightInmm = {}
                       where Id = {})",
                                  pc.Name, pc.PrinterName, pc.PaperName,
                                  pc.Orientation, pc.TopMargin, pc.BottomMargin,
                                  pc.LeftMargin, pc.RightMargin, pc.SaveType,
                                  pc.PaperWidthInmm, pc.PaperHeightInmm, pc.Id);
    if (_query->exec(QString::fromStdString(update_sql))) {
      return std::make_tuple(true, "");
    } else {
      return std::make_tuple(false, _query->lastError().text().toStdString());
    }

  } catch (...) {
    return std::make_tuple(false, "未知的错误发生了~");
  }
}

std::vector<PrinterConfigPtr> PrintDatabase::printerConfigQueryById(int Id) {
  std::vector<PrinterConfigPtr> pcs;
  try {
    QString querySql;
    if (Id == -1) { // 查询全部
      querySql = QString(R"(SELECT * FROM printer_config)");
      if (_query->exec(querySql)) {
        while (_query->next()) {
          pcs.push_back( std::make_shared<PrinterConfig>(_query.get()));
        }
      }
    } else {
      querySql = QString(R"(SELECT * FROM printer_config where Id=%1)").arg(Id);
      if (_query->exec(querySql)) {
        while (_query->next()) {
          pcs.push_back( std::make_shared<PrinterConfig>(_query.get()));
        }
      }
    }
    return pcs;
  } catch (...) {
    return pcs;
  }
}

std::vector<PrinterConfigPtr>
PrintDatabase::printerConfigQueryByName(const QString &Name) {
  std::vector<PrinterConfigPtr> pcs;
  try {
    QString querySql =
        QString(R"(SELECT * FROM printer_config where Name='%1')").arg(Name);
    if (_query->exec(querySql)) {
      while (_query->next()) {
        pcs.push_back(std::make_shared<PrinterConfig>(_query.get()));
      }
    }
    return pcs;

  } catch (...) {
    return pcs;
  }
}

std::tuple<int, std::vector<PrintedPage>>
PrintDatabase::printedPageQuery(int page_size_, int page_index_) {
  std::vector<PrintedPage> pps;
  try {
    QString query_sql = QString(R"(
   select  * from printed_page order by Id desc limit %1 offset %2
  )")
                            .arg(page_size_)
                            .arg(page_size_ * (page_index_ - 1));
    if (_query->exec(query_sql)) {
      while (_query->next()) {
        pps.push_back(PrintedPage(_query.get()));
      }
    }
    QString query_sql1 = QString("select COUNT(1) from printed_page");
    int Count = 0;
    if (_query->exec(query_sql1)) {
      while (_query->next()) {
        Count = _query->value(0).toLongLong();
      }
    }
    return std::make_tuple(Count, pps);
  } catch (...) {
    return std::make_tuple(0, pps);
  }
}

bool PrintDatabase::printedPageInsert(const PrintedPage &pp_) {
  try {

    auto query_sql =
        std::format(R"(
          insert into printed_page 
          (PrintTime,FromIp,FromType,PageName,ConfigName,PrintMode,IsSuccess)
          values('{}','{}','{}','{}','{}','{}',{})
  )",
                    pp_.PrintTime, pp_.FromIp, pp_.FromType, pp_.PageName,
                    pp_.ConfigName, pp_.PrintMode, pp_.IsSuccess ? 1 : 0);

    QString _q = QString::fromStdString(query_sql);
    if (_query->exec(_q)) {
      return true;
    }
    auto err = _query->lastQuery();
    return false;
  } catch (...) {
    return false;
  }
}

std::string PrintDatabase::getWebsocketUrl() {

  try {
    auto query_sql = "select * from remote_websocket_url";
    if (_query->exec(query_sql)) {
      while (_query->next()) {
        return _query->value(0).toString().toStdString();
      }
    }
    return "";
  } catch (...) {
    return "";
  }
}

bool PrintDatabase::insertOrUpdateWebsocketUrl(const std::string &websoc_url_) {
  try {
    auto query_sql1 = "select 1 from remote_websocket_url";
    if (_query->exec(query_sql1)) {
      while (_query->next()) {
        auto query_sql = std::format(
            "update remote_websocket_url set WebsocketUrl='{}'", websoc_url_);
        _query->exec(query_sql.c_str());
        return true;
      }
    }
    auto query_sql = std::format(
        "insert into remote_websocket_url(WebsocketUrl) values('{}')",
        websoc_url_);

    _query->exec(query_sql.c_str());
    return true;
  }

  catch (...) {
    return false;
  }
}
