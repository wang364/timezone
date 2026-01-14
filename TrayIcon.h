#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QWidget>

class SettingsWindow;
class TimezoneWindow;

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit TrayIcon(QObject *parent = nullptr);
    ~TrayIcon();

private slots:
    void onShowSettings();
    void onToggleTimezoneWindow();
    void onQuit();

private:
    void createTrayMenu();
    void updateTrayToolTip();

    QMenu *m_trayMenu;
    QAction *m_showSettingsAction;
    QAction *m_toggleTimezoneAction;
    QAction *m_quitAction;
    
    SettingsWindow *m_settingsWindow;
    TimezoneWindow *m_timezoneWindow;
    bool m_timezoneWindowVisible;
};

#endif // TRAYICON_H