#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
int main(int argc,char* argv[]) {
    QCoreApplication a(argc, argv);
    QProcess p;
    p.start(QString("echo"),{QString("/home/zerlei/.WebpagePrinter/resoures/1757492878")});
    p.waitForFinished();
    qDebug() << p.readAllStandardOutput();
    qDebug() << p.readAllStandardError();
    qDebug() << p.exitCode();
    qDebug() << p.exitStatus();
    return a.exec();
}