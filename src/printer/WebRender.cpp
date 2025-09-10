#include "WebRender.h"
#include "../InitConfig.h"
#include "../excep/FatalError.h"
#include "DataPack.h"
#include <tuple>

WebRender::WebRender(QObject* parent)
    : QObject(parent) {
    connect(&render_view, &QWebEngineView::loadFinished, this, &WebRender::slotLoadFinishTorint);
    connect(&render_view, &QWebEngineView::printRequested, this,
            &WebRender::slotJsPrintRequestToPrint);
    connect(&timeout_listen, &QTimer::timeout, this, &WebRender::slotPrintRequestTimeOut);
}
void WebRender::work(PrinterDataPack* pack, CanExceptionCallback ra) {
    if (is_working) {
        throw FatalError("WebRender::work: is working!!");
    }
    is_working   = true;
    current_work = std::make_tuple(pack, ra);

    render_view.load(QUrl(pack->page.page_url));
    // render_view.showMaximized();
    timeout_listen.start(200000);
}

void WebRender::slotLoadFinishTorint(bool ok) {
    if (!ok) {
        workFinish(false, "can't load page!");
    } else {

        auto& page = std::get<0>(current_work)->page;
        if (page.page_loaded_or_js_request == "PAGE_LOADED") {
            toRenderPdf();
        }
    }
}
void WebRender::slotJsPrintRequestToPrint() {
    toRenderPdf();
}
void WebRender::slotPrintRequestTimeOut() {
    workFinish(false, "Render timeout!");
}

void WebRender::toRenderPdf() {
    auto& config = std::get<0>(current_work)->config;

    auto base_dir = InitConfig::base_dir;

    // render_view.printToPdf("/home/zerlei/git/WebpagePrinter/t.pdf",
    //                        QPageLayout(QPageSize(QSize(config.width_mm, config.height_mm)),
    //                                    QPageLayout::Portrait,
    //                                    QMarginsF(config.left_margin, config.top_margin,
    //                                              config.right_margin, config.bottom_margin),
    //                                    QPageLayout::Millimeter));
    render_view.printToPdf(
        [this, base_dir](const QByteArray& data) {
            if (data.isEmpty()) {
                workFinish(false, "Unknown error,can't render pdf!");
            } else {
                QString dir_path = base_dir + "resoures/";
                QDir    dir(dir_path);
                if (!dir.exists()) {
                    dir.mkpath(dir_path);
                }
                auto  file_name = dir_path + QString::number(std::time(nullptr)) + ".pdf";
                QFile file(file_name);
                if (!file.open(QIODevice::WriteOnly)) {
                    workFinish(false, "Can't write file!");
                } else {
                    file.write(data);
                    file.close();
                }
                workFinish(true, file_name.replace(".pdf", ""));
            }
        },
        QPageLayout(QPageSize(QSize(config.width_mm, config.height_mm)), QPageLayout::Portrait,
                    QMarginsF(config.left_margin, config.top_margin, config.right_margin,
                              config.bottom_margin),
                    QPageLayout::Millimeter));
}
void WebRender::workFinish(bool success, const QString errormag_or_pdfpath) {
    timeout_listen.stop();
    render_view.stop();
    auto [pack, ra] = std::move(current_work);
    if (success) {
        pack->page.page_file_path = errormag_or_pdfpath;
        ra.correct();
    } else {
        pack->page.error_message = errormag_or_pdfpath;
        ra.exception(PrintWorkFlowError());
    }
    is_working = false;
}