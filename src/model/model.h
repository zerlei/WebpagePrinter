#ifndef BASE_H
#define BASE_H
#include <QSqlQuery>
#include <iostream>
#include <json/reader.h>
#include <json/value.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <memory>
#include <string>

struct PrinterConfig {

  PrinterConfig(const Json::Value &config_) {
    try {
      if (!config_["Id"].isNull()) {
        Id = config_["Id"].asInt();
      }
      Name = config_["Name"].asString();
      PrinterName = config_["PrinterName"].asString();
      PaperName = config_["PaperName"].asString();
      Orientation = config_["Orientation"].asString();
      TopMargin = config_["TopMargin"].asInt();
      BottomMargin = config_["BottomMargin"].asInt();
      LeftMargin = config_["LeftMargin"].asInt();
      RightMargin = config_["RightMargin"].asInt();
      SaveType = config_["SaveType"].asString();
      PaperWidthInmm = config_["PaperWidthInmm"].asInt();
      PaperHeightInmm = config_["PaperHeightInmm"].asInt();
      _is_correct = true;
    } catch (Json::Exception v) {
      std::cout << v.what();
    } catch (...) {
    }
  }
  PrinterConfig(QSqlQuery *query_) {
    try {
      Id = query_->value(0).toInt();
      Name = query_->value(1).toString().toStdString();
      TopMargin = query_->value(2).toInt();
      BottomMargin = query_->value(3).toInt();
      LeftMargin = query_->value(4).toInt();
      RightMargin = query_->value(5).toInt();
      Orientation = query_->value(6).toString().toStdString();
      PaperName = query_->value(7).toString().toStdString();
      PrinterName = query_->value(8).toString().toStdString();
      SaveType = query_->value(9).toString().toStdString();
      PaperWidthInmm = query_->value(10).toInt();
      PaperHeightInmm = query_->value(11).toInt();
      _is_correct = true;
    } catch (...) {
    }
  }
  Json::Value getReflectionJson() {

    Json::Value config;
    config["Id"] = Id;
    config["Name"] = Name;
    config["TopMargin"] = TopMargin;
    config["BottomMargin"] = BottomMargin;
    config["LeftMargin"] = LeftMargin;
    config["RightMargin"] = RightMargin;
    config["PaperName"] = PaperName;
    config["PrinterName"] = PrinterName;
    config["Orientation"] = Orientation;
    config["SaveType"] = SaveType;
    config["PaperWidthInmm"] = PaperWidthInmm;
    config["PaperHeightInmm"] = PaperHeightInmm;
    return config;
  }
  bool _is_correct{false};
  /**
   * @brief Id
   * 
   */
  int Id{0};
  /**
   * @brief 打印机名称
   * 
   */
  std::string Name;
  /**
   * @brief bit 位 从左至右 是否保存pdf | 是否保存 png 图片 | 是否打印 | 是否使用打印机配置
   * 
   */
  std::string SaveType;

  int PaperWidthInmm;
  int PaperHeightInmm;
  int TopMargin;
  int BottomMargin;
  int LeftMargin;
  int RightMargin;
  std::string PaperName;
  std::string PrinterName;
  std::string Orientation;
};

struct PrintedPage {
  PrintedPage(){};

  PrintedPage(Json::Value &page_) {

    try {
      if (!page_["Id"].isNull()) {
        Id = page_["Id"].asInt();
      }
      PrintTime = page_["PrintTime"].asString();
      FromIp = page_["FromIp"].asString();
      FromType = page_["FromType"].asString();
      PageName = page_["PageName"].asString();
      ConfigName = page_["ConfigName"].asString();
      PrintMode = page_["PrintMode"].asString();
      IsSuccess = page_["IsSuccess"].asBool();
      _is_correct = true;
    } catch (Json::Exception v) {
      std::cout << v.what();
    } catch (...) {
    }
  }
  PrintedPage(QSqlQuery *query_) {
    try {
      Id = query_->value(0).toInt();
      PrintTime = query_->value(1).toString().toStdString();
      FromIp = query_->value(2).toString().toStdString();
      FromType = query_->value(3).toString().toStdString();
      PageName = query_->value(4).toString().toStdString();
      ConfigName = query_->value(5).toString().toStdString();
      PrintMode = query_->value(6).toString().toStdString();
      IsSuccess = query_->value(7).toBool();
    } catch (...) {
    }
  }
  Json::Value getReflectionJson() {
    Json::Value page;
    page["PrintTime"] = PrintTime;
    page["FromIp"] = FromIp;
    page["PageName"] = PageName;
    page["FromType"] = FromType;
    page["ConfigName"] = ConfigName;
    page["PrintMode"] = PrintMode;
    page["IsSuccess"] = IsSuccess;
    return page;
  }
  bool _is_correct{false};
  int Id;
  std::string PrintTime;
  std::string FromIp;
  std::string FromType;
  std::string PageName;
  std::string ConfigName;
  std::string PrintMode;
  bool IsSuccess;
};
using PrinterConfigPtr = std::shared_ptr<PrinterConfig>;
using PrintedPagePtr = std::shared_ptr<PrintedPage>;
#endif // PrintDatabase_H
