#pragma once
#include "DataPack.h"
#include <QImage>
#include <QObject>
#include <QPrinter>
#include <QTimer>
#include <QtPdf/QPdfDocument>
#include <functional>
#include <future>
#include <qprinterinfo.h>
class Printer : public QObject {
    Q_OBJECT
      public:
        explicit Printer(QObject* parent = nullptr);
        static QList<QPrinterInfo> getAvaliblePrinterInfo();

        void setDataPack(PrinterDataPack * data_pack);
        void renderPng(std::promise<void> &&p);
        void toPrinter(std::promise<void> &&p);

      private:
        QPdfDocument       pdf_doc;
        QImage*            image{nullptr};
        PrinterDataPack*   data_pack;
        QTimer             print_request_timer;
        QList<QPrinter*>   printers;

        std::function<void(bool,const QString&)> pdf_load_success_callback;

      private slots:
        void slotPdfdocumentStatusChanged(QPdfDocument::Status status);
      private :
        void toPrinterResult(bool success, const QString& message,std::promise<void> &&p);

        bool printImage(QPrinter* printer);
};
