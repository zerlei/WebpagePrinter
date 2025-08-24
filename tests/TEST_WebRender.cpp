#include "../src/printer/WebRender.h"
#include "../src/exception/FatalError.h"
#include "../src/InitConfig.h"
#include <QTest>
#include <QSignalSpy>
#include <QWebEngineView>
#include <QtPrintSupport/QPrinterInfo>
#include <future>

class TestWebRender : public QObject {
    Q_OBJECT

private:
    WebRender* web_render;

private slots:
    void initTestCase() {
        web_render = new WebRender();
    }

    void cleanupTestCase() {
        delete web_render;
    }

    void testConstructor() {
        QVERIFY(web_render != nullptr);
        QVERIFY(web_render->getAvaliblePrinterInfo().size() >= 0);
    }

    void testGetAvaliblePrinterInfo() {
        QList<QPrinterInfo> printers = web_render->getAvaliblePrinterInfo();
        QVERIFY(printers.size() >= 0);
    }

    void testWorkNormalCase() {
        PrinterDataPack pack;
        pack.page.page_url = "https://example.com";
        std::promise<void> promise;
        auto future = promise.get_future();

        QSignalSpy load_finished_spy(&web_render->render_view, &QWebEngineView::loadFinished);
        web_render->work(&pack, std::move(promise));
        QVERIFY(web_render->is_working);
        QVERIFY(load_finished_spy.wait(5000));
        future.wait();
    }

    void testWorkEdgeCase() {
        PrinterDataPack pack;
        pack.page.page_url = "https://example.com";
        std::promise<void> promise1;
        std::promise<void> promise2;

        web_render->work(&pack, std::move(promise1));
        QVERIFY(web_render->is_working);

        try {
            web_render->work(&pack, std::move(promise2));
            QFAIL("Expected FatalError not thrown");
        } catch (const FatalError& e) {
            QVERIFY(true);
        }
    }

    void testSlotLoadFinishTorint() {
        QSignalSpy spy(web_render, &WebRender::slotLoadFinishTorint);
        web_render->slotLoadFinishTorint(true);
        QCOMPARE(spy.count(), 1);
    }

    void testSlotJsPrintRequestToPrint() {
        QSignalSpy spy(web_render, &WebRender::slotJsPrintRequestToPrint);
        web_render->slotJsPrintRequestToPrint();
        QCOMPARE(spy.count(), 1);
    }

    void testToRenderPdfSuccess() {
        PrinterDataPack pack;
        pack.page.page_url = "https://example.com";
        pack.config.width_mm = 210;
        pack.config.height_mm = 297;
        pack.config.left_margin = 10;
        pack.config.top_margin = 10;
        pack.config.right_margin = 10;
        pack.config.bottom_margin = 10;

        std::promise<void> promise;
        auto future = promise.get_future();

        web_render->work(&pack, std::move(promise));
        web_render->toRenderPdf();
        future.wait();

        QVERIFY(!pack.page.page_file_path.isEmpty());
    }

    void testToRenderPdfFailure() {
        PrinterDataPack pack;
        pack.page.page_url = "https://example.com";
        pack.config.width_mm = 0;
        pack.config.height_mm = 0;

        std::promise<void> promise;
        auto future = promise.get_future();

        web_render->work(&pack, std::move(promise));
        web_render->toRenderPdf();
        future.wait();

        QVERIFY(!pack.page.error_message.isEmpty());
    }

    void testWorkFinishSuccess() {
        PrinterDataPack pack;
        std::promise<void> promise;
        auto future = promise.get_future();

        web_render->workFinish(true, "test.pdf");
        future.wait();

        QVERIFY(pack.page.page_file_path == "test");
    }

    void testWorkFinishFailure() {
        PrinterDataPack pack;
        std::promise<void> promise;
        auto future = promise.get_future();

        web_render->workFinish(false, "Error rendering PDF");
        future.wait();

        QVERIFY(pack.page.error_message == "Error rendering PDF");
        QVERIFY(pack.page.status == "ERROR");
    }
};

QTEST_MAIN(TestWebRender)
#include "TEST_WebRender.moc"