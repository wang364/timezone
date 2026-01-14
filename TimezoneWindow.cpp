#include "TimezoneWindow.h"
#include "CityManager.h"
#include <QCloseEvent>
#include <QDateTime>
#include <QTimeZone>
#include <QScrollArea>
#include <QGroupBox>
#include <QSpacerItem>
#include <QPushButton>
#include <QMouseEvent>

TimezoneWindow::TimezoneWindow(QWidget *parent)
    : QWidget(parent)
    , m_updateTimer(new QTimer(this))
    , m_settings(new QSettings("TimezoneApp", "TimezoneTool"))
    , m_dragging(false)
    , m_dragPosition(0, 0)
{
    setupUI();
    loadSettings();
    
    setWindowTitle("时区显示");
    setFixedSize(300, 400);
    
    // 设置窗口标志：不在任务栏显示，无边框，保持置顶
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    
    connect(m_updateTimer, &QTimer::timeout, this, &TimezoneWindow::updateTimeDisplay);
    m_updateTimer->start(1000);
    
    connect(&CityManager::instance(), &CityManager::citiesChanged, this, &TimezoneWindow::onCitiesChanged);
    
    updateTimeDisplay();
    createCityTimeDisplay();
}

TimezoneWindow::~TimezoneWindow()
{
    delete m_settings;
}

void TimezoneWindow::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 添加标题栏和关闭按钮
    QHBoxLayout *titleLayout = new QHBoxLayout();
    
    // 创建标题栏容器
    QWidget *titleBar = new QWidget();
    titleBar->setFixedHeight(30);
    titleBar->setStyleSheet("QWidget { background: #f0f0f0; border-bottom: 1px solid #ccc; }");
    
    QHBoxLayout *titleBarLayout = new QHBoxLayout(titleBar);
    titleBarLayout->setContentsMargins(10, 0, 10, 0);
    
    QLabel *titleLabel = new QLabel("时区显示");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    
    QPushButton *closeButton = new QPushButton("×");
    closeButton->setFixedSize(20, 20);
    closeButton->setStyleSheet("QPushButton { background: transparent; border: none; font-size: 16px; } QPushButton:hover { background: #e0e0e0; }");
    
    titleBarLayout->addWidget(titleLabel);
    titleBarLayout->addStretch();
    titleBarLayout->addWidget(closeButton);
    
    titleLayout->addWidget(titleBar);
    
    m_scrollArea = new QScrollArea();
    m_scrollWidget = new QWidget();
    m_scrollLayout = new QVBoxLayout(m_scrollWidget);
    
    m_citiesGroup = new QGroupBox("城市时间");
    m_citiesLayout = new QVBoxLayout(m_citiesGroup);
    
    m_scrollLayout->addWidget(m_citiesGroup);
    m_scrollLayout->addStretch();
    
    m_scrollArea->setWidget(m_scrollWidget);
    m_scrollArea->setWidgetResizable(true);
    
    m_mainLayout->addLayout(titleLayout);
    m_mainLayout->addWidget(m_scrollArea);
    
    // 设置窗口样式
    setStyleSheet("QWidget { background: white; border: 1px solid #ccc; }");
    
    connect(closeButton, &QPushButton::clicked, this, &TimezoneWindow::hide);
}

void TimezoneWindow::loadSettings()
{
    m_timeFormat = m_settings->value("timeFormat", "HH:mm:ss").toString();
    m_dateFormat = m_settings->value("dateFormat", "yyyy-MM-dd").toString();
    m_showSeconds = m_settings->value("showSeconds", true).toBool();
    m_showDate = m_settings->value("showDate", true).toBool();
    
    if (!m_showSeconds) {
        m_timeFormat = m_timeFormat.replace(":ss", "");
    }
}

void TimezoneWindow::createCityTimeDisplay()
{
    QList<CityInfo> cities = CityManager::instance().getCities();
    
    QLayoutItem *child;
    while ((child = m_citiesLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    
    m_cityTimeLabels.clear();
    
    if (cities.isEmpty()) {
        QLabel *noCitiesLabel = new QLabel("暂无城市，请在设置中添加城市");
        noCitiesLabel->setAlignment(Qt::AlignCenter);
        noCitiesLabel->setStyleSheet("color: gray; font-style: italic; margin: 20px;");
        m_citiesLayout->addWidget(noCitiesLabel);
        return;
    }
    
    for (const CityInfo &city : cities) {
        QHBoxLayout *cityLayout = new QHBoxLayout();
        
        QLabel *cityNameLabel = new QLabel(city.name);
        cityNameLabel->setFixedWidth(80);
        
        QLabel *timeLabel = new QLabel();
        timeLabel->setAlignment(Qt::AlignRight);
        timeLabel->setStyleSheet("font-family: 'Courier New'; font-size: 12px;");
        
        cityLayout->addWidget(cityNameLabel);
        cityLayout->addWidget(timeLabel);
        
        m_citiesLayout->addLayout(cityLayout);
        m_cityTimeLabels.append(timeLabel);
    }
    
    updateCityTimes();
}

void TimezoneWindow::updateTimeDisplay()
{
    // 只更新城市时间，不再显示本地时间和UTC时间
    updateCityTimes();
}

void TimezoneWindow::updateCityTimes()
{
    QList<CityInfo> cities = CityManager::instance().getCities();
    
    if (cities.size() != m_cityTimeLabels.size()) {
        createCityTimeDisplay();
        return;
    }
    
    QDateTime utcTime = QDateTime::currentDateTime().toUTC();
    
    for (int i = 0; i < cities.size(); ++i) {
        const CityInfo &city = cities[i];
        QTimeZone timezone(city.timezoneId.toUtf8());
        
        if (timezone.isValid()) {
            QDateTime cityTime = utcTime.toTimeZone(timezone);
            QString timeStr;
            
            if (m_showDate) {
                QString dateStr = cityTime.toString(m_dateFormat);
                timeStr = QString("%1\n%2").arg(dateStr).arg(cityTime.toString(m_timeFormat));
            } else {
                timeStr = cityTime.toString(m_timeFormat);
            }
            
            m_cityTimeLabels[i]->setText(timeStr);
        } else {
            m_cityTimeLabels[i]->setText("时区无效");
        }
    }
}

void TimezoneWindow::onCitiesChanged()
{
    createCityTimeDisplay();
}

void TimezoneWindow::mousePressEvent(QMouseEvent *event)
{
    // 只在标题栏区域允许拖动
    if (event->button() == Qt::LeftButton && event->pos().y() <= 30) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        setCursor(Qt::ClosedHandCursor);
    }
    QWidget::mousePressEvent(event);
}

void TimezoneWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 更新鼠标光标
    if (event->pos().y() <= 30) {
        setCursor(Qt::OpenHandCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
    
    // 处理拖动
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
    }
    QWidget::mouseMoveEvent(event);
}

void TimezoneWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        // 鼠标释放后恢复光标状态
        if (event->pos().y() <= 30) {
            setCursor(Qt::OpenHandCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }
    }
    QWidget::mouseReleaseEvent(event);
}

void TimezoneWindow::closeEvent(QCloseEvent *event)
{
    // 对于无边框窗口，直接隐藏而不是关闭
    hide();
    event->ignore();
}