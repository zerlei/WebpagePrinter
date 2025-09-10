#pragma once
#include "../excep/CanExceptionCallback.h"
#include "DataPack.h"
#include <QBuffer>
#include <QObject>
#include <QTimer>
#include <QWebEngineView>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrinterInfo>
#include <qtmetamacros.h>
#include <tuple>

class WebRender : public QObject {
    Q_OBJECT
      public:
        explicit WebRender(QObject* parent = nullptr);
        // Q_SIGNALS:
        /**
         * @brief
         *
         * @param page
         * @param ra
         */
        void work(PrinterDataPack * pack, CanExceptionCallback ra);

        QWebEngineView render_view;

      private:
        QTimer           print_request_time;
        QList<QPrinter*> printers;
        QTimer           timeout_listen;
        bool             is_working{false};

        std::tuple<PrinterDataPack*, CanExceptionCallback> current_work;
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

      private:
        /**
         * @brief
         *
         * @param is_success
         * @param errmsg_or_pdfpath
         */
        void workFinish(bool is_success, const QString errmsg_or_pdfpath);

        void toRenderPdf();
};