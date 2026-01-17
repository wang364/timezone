#include "TrayIcon.h"
#include "SettingsWindow.h"
#include "TimezoneWindow.h"
#include "StartupManager.h"
#include <QApplication>
#include <QTimer>
#include <QDateTime>
#include <QPixmap>

TrayIcon::TrayIcon(QObject *parent)
    : QSystemTrayIcon(parent)
    , m_trayMenu(nullptr)
    , m_settingsWindow(nullptr)
    , m_timezoneWindow(nullptr)
    , m_timezoneWindowVisible(false)
{
    // 创建托盘图标 - 使用SVG并转换为适合托盘的大小
    QPixmap pixmap(16, 16);
    pixmap.fill(Qt::transparent);
    
    // 尝试加载SVG图标
    QIcon appIcon("icon/timezone.svg");
    if (!appIcon.isNull()) {
        setIcon(appIcon);
    } else {
        // 如果SVG加载失败，使用备用图标
        pixmap.fill(QColor(74, 144, 226)); // 蓝色图标
        setIcon(QIcon(pixmap));
    }
    
    setToolTip(tr("时区工具"));
    
    createTrayMenu();
    
    connect(this, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) {
            onToggleTimezoneWindow();
        }
    });
    
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TrayIcon::updateTrayToolTip);
    timer->start(1000);
    
    // 检查系统是否支持托盘图标
    if (!isSystemTrayAvailable()) {
        qWarning() << "系统托盘不可用";
    }
    
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning() << "没有检测到系统托盘";
    }

    m_timezoneWindow = new TimezoneWindow();
    m_settingsWindow = new SettingsWindow(m_timezoneWindow);

    show();
    
    onToggleTimezoneWindow();
}

TrayIcon::~TrayIcon()
{
    delete m_settingsWindow;
    delete m_timezoneWindow;
    delete m_trayMenu;
}

void TrayIcon::createTrayMenu()
{
    m_trayMenu = new QMenu();
    
    m_showSettingsAction = new QAction(tr("设置"), this);
    m_toggleTimezoneAction = new QAction(tr("显示时区窗口"), this);
    m_quitAction = new QAction(tr("退出"), this);
    
    connect(m_showSettingsAction, &QAction::triggered, this, &TrayIcon::onShowSettings);
    connect(m_toggleTimezoneAction, &QAction::triggered, this, &TrayIcon::onToggleTimezoneWindow);
    connect(m_quitAction, &QAction::triggered, this, &TrayIcon::onQuit);
    
    m_trayMenu->addAction(m_showSettingsAction);
    m_trayMenu->addAction(m_toggleTimezoneAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_quitAction);
    
    setContextMenu(m_trayMenu);
}

void TrayIcon::onShowSettings()
{
    if (m_settingsWindow) {
        m_settingsWindow->show();
        m_settingsWindow->activateWindow();
    }
}

void TrayIcon::onToggleTimezoneWindow()
{
    if (m_timezoneWindowVisible) {
        m_timezoneWindow->hide();
        m_toggleTimezoneAction->setText(tr("显示时区窗口"));
    } else {
        m_timezoneWindow->show();
        m_timezoneWindow->activateWindow();
        m_toggleTimezoneAction->setText(tr("隐藏时区窗口"));
    }
    
    m_timezoneWindowVisible = !m_timezoneWindowVisible;
}

void TrayIcon::onQuit()
{
    qApp->quit();
}

void TrayIcon::updateTrayToolTip()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString tooltip = QString(tr("当前时间: %1\n时区工具")).arg(currentTime.toString("yyyy-MM-dd hh:mm:ss"));
    setToolTip(tooltip);
}