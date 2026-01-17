#include "StartupManager.h"
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

StartupManager::StartupManager(QObject *parent)
    : QObject(parent)
{
}

StartupManager& StartupManager::instance()
{
    static StartupManager instance;
    return instance;
}

QString StartupManager::getRegistryKey() const
{
    return "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
}

QString StartupManager::getExecutablePath() const
{
    QString appPath = QCoreApplication::applicationFilePath();
    
    if (appPath.contains(" ")) {
        appPath = "\"" + appPath + "\"";
    }
    
    return appPath;
}

bool StartupManager::isAutoStartEnabled() const
{
    QSettings settings(getRegistryKey(), QSettings::NativeFormat);
    QString appName = QCoreApplication::applicationName();
    
    QString registeredPath = settings.value(appName).toString();
    QString currentPath = getExecutablePath();
    
    if (registeredPath.isEmpty()) {
        return false;
    }
    
    return registeredPath == currentPath;
}

bool StartupManager::setAutoStart(bool enabled)
{
    QSettings settings(getRegistryKey(), QSettings::NativeFormat);
    QString appName = QCoreApplication::applicationName();
    
    if (enabled) {
        QString appPath = getExecutablePath();
        settings.setValue(appName, appPath);
        qDebug() << "已启用开机自启动:" << appPath;
    } else {
        settings.remove(appName);
        qDebug() << "已禁用开机自启动";
    }
    
    settings.sync();
    
    return isAutoStartEnabled() == enabled;
}
