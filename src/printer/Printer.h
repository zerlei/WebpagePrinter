#pragma once
#include <QBuffer>
#include <QObject>
#include <QTimer>
#include <QWebEngineView>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrinterInfo>
#include <qprinterinfo.h>
#include <qtmetamacros.h>

class Printer : public QObject {
    Q_OBJECT
      public:
        explicit Printer(QObject* parent = nullptr);
        // Q_SIGNALS:

        const QList<QPrinterInfo>& getAvaliblePrinterInfo() const;

      private:
        QList<QPrinterInfo> avaliable_printer_info;

        QTimer           print_request_time;
        QList<QPrinter*> printers;
        QTimer           timeout_listen;
        QWebEngineView   render_view;
      private slots:
        ///
        /// \brief 网页加载完毕打印
        ///
        void slotLoadFinishTorint(bool);
        ///
        /// \brief Js window.print 调用打印页面
        ///
        void slotJsPrintRequestToPrint();

        ///
        /// \brief SlotPrintRequestTimeOut
        ///

        void slotPrintRequestTimeOut();

        /// 移动到GUI线程开启打印工作
        /// \brief SlotMoveToGUIThreadWork
        ///

        void slotMoveToPrinterThreadWork();
};