#include <QApplication>
#include <QIcon>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include "TrayIcon.h"

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString logMessage;
    
    switch (type) {
    case QtDebugMsg:
        logMessage = QString("[DEBUG] %1").arg(msg);
        break;
    case QtInfoMsg:
        logMessage = QString("[INFO] %1").arg(msg);
        break;
    case QtWarningMsg:
        logMessage = QString("[WARNING] %1").arg(msg);
        break;
    case QtCriticalMsg:
        logMessage = QString("[CRITICAL] %1").arg(msg);
        break;
    case QtFatalMsg:
        logMessage = QString("[FATAL] %1").arg(msg);
        break;
    }
    
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString fullMessage = QString("%1 %2").arg(timestamp, logMessage);
    
    QString logFilePath = "logs/app.log";
    QDir().mkpath("logs");
    
    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream out(&logFile);
        out << fullMessage << "\n";
        logFile.close();
    }
    
    fprintf(stderr, "%s\n", fullMessage.toLocal8Bit().constData());
    fflush(stderr);
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(customMessageHandler);
    
    QApplication app(argc, argv);
    
    app.setWindowIcon(QIcon("icon/timezone.svg"));
    app.setQuitOnLastWindowClosed(false);
    
    TrayIcon trayIcon;
    
    return app.exec();
}
