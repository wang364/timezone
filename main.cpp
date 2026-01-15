#include <QApplication>
#include <QIcon>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QTranslator>
#include <QSettings>
#include "TrayIcon.h"

// 全局翻译器变量，避免超出作用域
QTranslator g_translator;

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
    
    // 设置应用程序信息
    app.setOrganizationName("TimezoneApp");
    app.setApplicationName("TimezoneTool");
    
    // 加载翻译
    QSettings settings("TimezoneApp", "TimezoneTool");
    QString language = settings.value("language", "zh").toString();
    
    // 尝试加载对应的翻译文件
    if (g_translator.load(QString("timezone_%1.qm").arg(language), "./translations")) {
        app.installTranslator(&g_translator);
        qDebug() << "成功加载翻译文件: timezone_" << language << ".qm";
    } else {
        qDebug() << "未能加载翻译文件: timezone_" << language << ".qm";
    }
    
    TrayIcon trayIcon;
    
    return app.exec();
}
