#ifndef TIMEZONEWINDOW_H
#define TIMEZONEWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QTimeZone>
#include <QSettings>
#include <QScrollArea>
#include <QGroupBox>
#include "CityManager.h"

class TimezoneWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TimezoneWindow(QWidget *parent = nullptr);
    ~TimezoneWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void updateTimeDisplay();
    void onCitiesChanged();

private:
    void setupUI();
    void loadSettings();
    void createCityTimeDisplay();
    void updateCityTimes();
    
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_scrollWidget;
    QVBoxLayout *m_scrollLayout;
    
    QGroupBox *m_citiesGroup;
    QVBoxLayout *m_citiesLayout;
    
    QTimer *m_updateTimer;
    QSettings *m_settings;
    
    QString m_timeFormat;
    QString m_dateFormat;
    bool m_showSeconds;
    bool m_showDate;
    
    QList<QLabel*> m_cityTimeLabels;
    
    bool m_dragging;
    QPoint m_dragPosition;
};

#endif // TIMEZONEWINDOW_H