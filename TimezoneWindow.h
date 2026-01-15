#ifndef TIMEZONEWINDOW_H
#define TIMEZONEWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QTimeZone>
#include <QSettings>
#include <QGroupBox>
#include <QEnterEvent>
#include "CityManager.h"

class TimezoneWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TimezoneWindow(QWidget *parent = nullptr);
    ~TimezoneWindow();
    
    void reloadSettings();

protected:
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void updateTimeDisplay();
    void onCitiesChanged();
    void onCityHovered(int index, bool isHovered);

private:
    void setupUI();
    void loadSettings();
    void createCityTimeDisplay();
    void updateCityTimes();
    
    // 事件处理
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    
    QVBoxLayout *m_mainLayout;
    
    QGroupBox *m_citiesGroup;
    QVBoxLayout *m_citiesLayout;
    
    QTimer *m_updateTimer;
    QSettings *m_settings;
    
    QString m_timeFormat;
    QString m_dateFormat;
    QString m_weekdayFormat;
    bool m_showSeconds;
    bool m_showDate;
    bool m_showWeekday;
    
    QList<QLabel*> m_cityTimeLabels;
    QList<QLabel*> m_cityNameLabels;
    QList<QWidget*> m_cityContainers;
    
    bool m_dragging;
    QPoint m_dragPosition;
    int m_hoveredCityIndex;
    
    // 动画和过渡效果
    QPropertyAnimation *m_opacityAnimation;
    bool m_isFadingOut;
    int m_fadeOutTimer;
    const int FADE_OUT_DELAY = 5000; // 5秒后淡出
    const int FADE_DURATION = 1000; // 1秒淡出动画
};

#endif // TIMEZONEWINDOW_H