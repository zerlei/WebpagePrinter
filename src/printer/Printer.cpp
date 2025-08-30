#include "Printer.h"
#include "../exception/PrintWorkFlowError.h"
#include "DataPack.h"
#include <QImageWriter>
#include <QPainter>
#include <algorithm>
#include <exception>
#include <future>
QList<QPrinterInfo> Printer::getAvaliblePrinterInfo() {
    return QPrinterInfo::availablePrinters();
}
Printer::Printer(QObject* parent)
    : QObject(parent) {

    pdf_load_success_callback = [](bool, const QString&) {};
    connect(&pdf_doc, &QPdfDocument::statusChanged, this, &Printer::slotPdfdocumentStatusChanged);
}

void Printer::setDataPack(PrinterDataPack* pack) {

    if (pack != data_pack) {
        delete image;
        image                     = nullptr;
        pdf_load_success_callback = [](bool, const QString&) {};
        data_pack                 = pack;
    }
}
void Printer::renderPng(std::promise<void>&& p) {
    auto f = [&p, this](bool success, const QString& message) {
        if (success) {

            QImageWriter writer(data_pack->page.page_file_path + ".png", "png");
            writer.setQuality(100);
            if (writer.canWrite()) {
                writer.write(*image);
                p.set_value();
            } else {
                data_pack->page.error_message = "can't write png file";
                p.set_exception(std::make_exception_ptr(PrintWorkFlowError()));
            }
        } else {
            data_pack->page.error_message = message;
            p.set_exception(std::make_exception_ptr(PrintWorkFlowError()));
        }
    };

    if (image == nullptr) {
        pdf_load_success_callback = f;
        pdf_doc.load(data_pack->page.page_file_path + ".pdf");
    } else {
        f(true, "");
    }
}
void Printer::toPrinterResult(bool success, const QString& message, std::promise<void>&& p) {

    if (success) {
        auto avalible_printer_infos = Printer::getAvaliblePrinterInfo();
        auto printer_info           = std::find_if(
            avalible_printer_infos.cbegin(), avalible_printer_infos.cend(), [this](const auto& e) {
                if (e.printerName() == data_pack->config.printer_name) {
                    return true;
                }
                return false;
            });
        if (printer_info == avalible_printer_infos.cend()) {
            toPrinterResult(false, "can't find avalible printer in system printers", std::move(p));
        } else {
            auto c_printer_iter =
                std::find_if(printers.cbegin(), printers.cend(), [printer_info](const auto& e) {
                    if (e->printerName() == printer_info->printerName()) {
                        return true;
                    }
                    return false;
                });
            QPrinter* c_printer = nullptr;
            if (c_printer_iter == printers.cend()) {
                c_printer = new QPrinter(*printer_info, QPrinter::HighResolution);
                printers.append(c_printer);
            } else {
                c_printer = *c_printer_iter;
            }

            if (data_pack->config.is_use_printer_default_config == 1) {
                if (printImage(c_printer)) {
                    p.set_value();
                } else {
                    toPrinterResult(false, "Painter to printer error!", std::move(p));
                }

            } else {
                auto supportd_page_sizes = printer_info->supportedPageSizes();
                auto paper_info =
                    std::find_if(supportd_page_sizes.cbegin(), supportd_page_sizes.cend(),
                                 [this](const auto& e) {
                                     if (e.name() == data_pack->config.printer_paper_name) {
                                         return true;
                                     }
                                     return false;
                                 });

                if (paper_info == supportd_page_sizes.cend()) {
                    toPrinterResult(false, "can't find avalible paper in printer", std::move(p));
                } else {
                    QPageLayout layout;
                    layout.setPageSize(*paper_info);
                    if (data_pack->config.printer_orientation == "Landscape") {
                        layout.setOrientation(QPageLayout::Landscape);
                    } else {
                        layout.setOrientation(QPageLayout::Portrait);
                    }
                    c_printer->setPageLayout(layout);
                    c_printer->setResolution(600);
                    if (printImage(c_printer)) {
                        p.set_value();
                    } else {
                        toPrinterResult(false, "Painter to printer error!", std::move(p));
                    }
                }
            }
        }
    } else {
        data_pack->page.error_message = message;
        p.set_exception(std::make_exception_ptr(PrintWorkFlowError()));
    }
}
bool Printer::printImage(QPrinter* printer) {
    QPainter painter;
    if (!painter.begin(printer)) {
        return false;
        printers.removeIf([printer](auto& e) { return e == printer; });
        delete printer;
    }
    const auto rect = printer->pageRect(QPrinter::Unit::DevicePixel);
    painter.drawImage(rect, *image, QRectF(0, 0, image->width(), image->height()));
    painter.end();
    return true;
}
void Printer::toPrinter(std::promise<void>&& p) {

    auto f = [&p, this](bool success, const QString& message) {
        toPrinterResult(success, message, std::move(p));
    };

    if (image == nullptr) {
        pdf_load_success_callback = f;
        pdf_doc.load(data_pack->page.page_file_path + ".pdf");
    } else {
        f(true, "");
    }
}

void Printer::slotPdfdocumentStatusChanged(QPdfDocument::Status status) {
    if (status == QPdfDocument::Status::Ready) {
        image = new QImage(pdf_doc.render(
            0, QSize(data_pack->config.width_mm * 10, data_pack->config.height_mm * 10)));
        pdf_doc.close();
        pdf_load_success_callback(true, "");

    } else if (status == QPdfDocument::Status::Error) {
        pdf_load_success_callback(false, "pdf render error!");
    }
}