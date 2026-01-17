#ifndef STARTUPMANAGER_H
#define STARTUPMANAGER_H

#include <QString>
#include <QObject>

class StartupManager : public QObject
{
    Q_OBJECT

public:
    static StartupManager& instance();
    
    bool isAutoStartEnabled() const;
    bool setAutoStart(bool enabled);
    QString getExecutablePath() const;
    QString getRegistryKey() const;

private:
    explicit StartupManager(QObject *parent = nullptr);
    ~StartupManager() = default;
    
    StartupManager(const StartupManager&) = delete;
    StartupManager& operator=(const StartupManager&) = delete;
};

#endif // STARTUPMANAGER_H
