#pragma once
#include "../excep/CanExceptionCallback.h"
#include "DataPack.h"
#include <QImage>
#include <QObject>
#include <QPrinter>
#include <QTimer>
#include <QtPdf/QPdfDocument>
#include <functional>
#include <memory>
#include <qprinterinfo.h>
class Printer : public QObject {
    Q_OBJECT
      public:
        explicit Printer(QObject* parent = nullptr);
        static QList<QPrinterInfo> getAvaliblePrinterInfo();

        void setDataPack(PrinterDataPack * data_pack);
        void renderPng(CanExceptionCallback cb);
        void toPrinter(CanExceptionCallback cb);

      private:
        QPdfDocument            pdf_doc;
        std::unique_ptr<QImage> image{nullptr};
        PrinterDataPack*        data_pack{nullptr};
        /**
         * @brief 记录打印内容是否发生改变
         * 
         */
        int                     current_page_id{0};
        QList<QPrinter*>        printers;

        std::function<void(bool, const QString&)> pdf_load_success_callback;

      private slots:
        void slotPdfdocumentStatusChanged(QPdfDocument::Status status);

      private:
        void toPrinterResult(bool success, const QString& message, CanExceptionCallback cb);

        bool printImage(QPrinter * printer);
};
