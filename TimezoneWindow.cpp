#include "TimezoneWindow.h"
#include "CityManager.h"
#include <QCloseEvent>
#include <QDateTime>
#include <QTimeZone>
#include <QGroupBox>
#include <QPushButton>
#include <QMouseEvent>
#include <QSizePolicy>
#include <QResizeEvent>
#include <QPropertyAnimation>
#include <QPainter>
#include <QLinearGradient>
#include <QPainterPath>
#include <QPaintEvent>
#include <QHoverEvent>
#include <QEvent>
#include <QDebug>

TimezoneWindow::TimezoneWindow(QWidget *parent)
    : QWidget(parent)
    , m_updateTimer(new QTimer(this))
    , m_settings(new QSettings("TimezoneApp", "TimezoneTool"))
    , m_dragging(false)
    , m_dragPosition(0, 0)
    , m_hoveredCityIndex(-1)
{
    setupUI();
    loadSettings();
    
    setWindowTitle(tr("时区显示"));
    
    // 设置窗口标志：不在任务栏显示，无边框，保持置顶
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    
    // 设置窗口大小约束
    setMinimumSize(320, 90); // 确保足够的宽度显示时间
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    
    connect(m_updateTimer, &QTimer::timeout, this, &TimezoneWindow::updateTimeDisplay);
    m_updateTimer->start(1000);
    
    connect(&CityManager::instance(), &CityManager::citiesChanged, this, &TimezoneWindow::onCitiesChanged);
    
    updateTimeDisplay();
    createCityTimeDisplay();
    
    // 确保窗口根据内容自动调整大小
    adjustSize();
}

TimezoneWindow::~TimezoneWindow()
{
    delete m_settings;
}

void TimezoneWindow::setupUI()
{
    // 创建主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(15, 10, 15, 15);
    m_mainLayout->setSpacing(10);
    
    // 创建标题栏区域
    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(5, 0, 5, 0);
    
    // 标题标签
    QLabel *titleLabel = new QLabel(tr("时区工具"));
    titleLabel->setStyleSheet(
        "font-size: 12px; "
        "font-weight: bold; "
        "color: rgba(255, 255, 255, 200); "
        "padding: 0 5px;"
    );
    
    // 关闭按钮
    QPushButton *closeButton = new QPushButton("×");
    closeButton->setFixedSize(22, 22);
    closeButton->setStyleSheet(
        "QPushButton { "
        "    background: transparent; "
        "    border: none; "
        "    font-size: 16px; "
        "    color: rgba(255, 255, 255, 200); "
        "    border-radius: 4px; "
        "    padding: 0; "
        "}"
        "QPushButton:hover { "
        "    background: rgba(255, 255, 255, 30); "
        "}"
        "QPushButton:pressed { "
        "    background: rgba(255, 255, 255, 50); "
        "}"
    );
    
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(closeButton);
    
    // 创建城市列表容器
    m_citiesGroup = new QGroupBox();
    m_citiesGroup->setStyleSheet(
        "QGroupBox { "
        "    border: none; "
        "    background: transparent; "
        "    margin: 0; "
        "}"
    );
    
    m_citiesLayout = new QVBoxLayout(m_citiesGroup);
    m_citiesLayout->setContentsMargins(0, 0, 0, 0);
    m_citiesLayout->setSpacing(2);
    
    // 组装主布局
    m_mainLayout->addLayout(titleLayout);
    m_mainLayout->addWidget(m_citiesGroup, 1);
    
    // 设置窗口样式
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_Hover); // 启用悬停事件
    setStyleSheet(
        "QWidget#TimezoneWindow { "
        "    background: rgba(25, 25, 35, 200); "
        "    border: 1px solid rgba(255, 255, 255, 30); "
        "    border-radius: 10px; "
        "}"
    );
    
    // 设置对象名称以便样式应用
    setObjectName("TimezoneWindow");
    
    // 创建淡入淡出动画
    m_opacityAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    m_opacityAnimation->setDuration(FADE_DURATION);
    m_isFadingOut = false;
    m_hasFadedOut = false;
    m_fadeOutTimer = 0;
    
    // 连接信号槽
    connect(closeButton, &QPushButton::clicked, this, &TimezoneWindow::hide);
    connect(this, &QWidget::show, this, [this]() {
        m_isFadingOut = false;
        m_hasFadedOut = false;
        m_fadeOutTimer = 0;
        setWindowOpacity(1.0);
    });
    
    // 连接动画完成信号
    connect(m_opacityAnimation, &QPropertyAnimation::finished, this, [this]() {
        if (m_isFadingOut) {
            m_hasFadedOut = true;
        }
    });
}

void TimezoneWindow::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    m_isFadingOut = false;
    m_hasFadedOut = false;
    m_fadeOutTimer = 0;
    
    // 如果正在淡出，取消淡出动画
    if (m_opacityAnimation->state() == QPropertyAnimation::Running) {
        m_opacityAnimation->stop();
    }
    
    // 恢复不透明度
    setWindowOpacity(1.0);
}

void TimezoneWindow::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_isFadingOut = true;
    m_hasFadedOut = false;
    m_fadeOutTimer = 0;
}

void TimezoneWindow::paintEvent(QPaintEvent *event)
{
    // 创建自定义绘制
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    // 绘制半透明背景
    QRect backgroundRect = rect();
    QPainterPath path;
    path.addRoundedRect(backgroundRect, 10, 10);
    
    // 渐变背景
    QLinearGradient gradient(backgroundRect.topLeft(), backgroundRect.bottomRight());
    gradient.setColorAt(0.0, QColor(25, 25, 35, 220));
    gradient.setColorAt(1.0, QColor(35, 35, 45, 220));
    
    painter.fillPath(path, gradient);
    
    // 绘制边框
    painter.setPen(QPen(QColor(255, 255, 255, 40), 1));
    painter.drawRoundedRect(backgroundRect.adjusted(0.5, 0.5, -0.5, -0.5), 10, 10);
    
    QWidget::paintEvent(event);
}

void TimezoneWindow::resizeEvent(QResizeEvent *event)
{
    // 确保窗口大小不会小于最小尺寸
    if (width() < minimumWidth() || height() < minimumHeight()) {
        adjustSize();
        return;
    }
    
    QWidget::resizeEvent(event);
}

void TimezoneWindow::loadSettings()
{
    m_timeFormat = m_settings->value("timeFormat", "HH:mm:ss").toString();
    m_dateFormat = m_settings->value("dateFormat", "yyyy-MM-dd").toString();
    m_weekdayFormat = m_settings->value("weekdayFormat", "ddd").toString();
    m_showSeconds = m_settings->value("showSeconds", true).toBool();
    m_showDate = m_settings->value("showDate", true).toBool();
    m_showWeekday = m_settings->value("showWeekday", false).toBool();
    
    if (!m_showSeconds) {
        m_timeFormat = m_timeFormat.replace(":ss", "");
    }
}

void TimezoneWindow::reloadSettings()
{
    loadSettings();
    updateCityTimes();
    //adjustSize();
}

void TimezoneWindow::createCityTimeDisplay()
{
    QList<CityInfo> cities = CityManager::instance().getCities();
    
    // 清除旧的城市显示
    QLayoutItem *child;
    while ((child = m_citiesLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    
    // 清除标签列表
    m_cityTimeLabels.clear();
    m_cityNameLabels.clear();
    m_cityContainers.clear();
    
    if (cities.isEmpty()) {
        // 显示空状态
        QLabel *noCitiesLabel = new QLabel(tr("暂无城市，请在设置中添加"));
        noCitiesLabel->setAlignment(Qt::AlignCenter);
        noCitiesLabel->setStyleSheet(
            "color: rgba(255, 255, 255, 160); "
            "font-style: italic; "
            "font-size: 12px; "
            "padding: 30px 0;"
        );
        m_citiesLayout->addWidget(noCitiesLabel);
        return;
    }
    
    // 为每个城市创建显示容器
    for (int i = 0; i < cities.size(); ++i) {
        const CityInfo &city = cities[i];
        
        // 创建城市容器
        QWidget *cityContainer = new QWidget();
        cityContainer->setObjectName(QString("cityContainer_%1").arg(i));
        cityContainer->setStyleSheet(
            "background: transparent; "
            "border-radius: 6px; "
        );
        cityContainer->setProperty("cityIndex", i);
        cityContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        
        // 为容器添加鼠标事件
        cityContainer->installEventFilter(this);
        
        // 创建城市布局
        QHBoxLayout *cityLayout = new QHBoxLayout(cityContainer);
        cityLayout->setContentsMargins(0, 0, 0, 0);
        //cityLayout->setSpacing(15);
        
        // 城市名称标签
        QLabel *cityNameLabel = new QLabel(city.name);
        cityNameLabel->setStyleSheet(
            "font-size: 12px; "
            "font-weight: normal; "
            "color: rgba(255, 255, 255, 180); "
            "min-width: 65px;"
        );
        cityNameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        cityNameLabel->setToolTip(city.name);
        //cityNameLabel->setWordWrap(true);
        
        // 时间标签
        QLabel *timeLabel = new QLabel();
        timeLabel->setAlignment(Qt::AlignRight);
        timeLabel->setTextFormat(Qt::RichText);
        timeLabel->setStyleSheet(
            "color: white;"
        );
        // 设置最小宽度以确保时间始终可见
        timeLabel->setMinimumWidth(120);
        timeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        
        // 将组件添加到布局
        cityLayout->addWidget(cityNameLabel);
        cityLayout->addWidget(timeLabel);
        
        // 将容器添加到主布局
        m_citiesLayout->addWidget(cityContainer);
        
        // 保存引用
        m_cityNameLabels.append(cityNameLabel);
        m_cityTimeLabels.append(timeLabel);
        m_cityContainers.append(cityContainer);
    }
    
    updateCityTimes();
    
    // 调整窗口大小以适应城市数量变化
    //adjustSize();
}

void TimezoneWindow::updateTimeDisplay()
{
    // 更新城市时间
    updateCityTimes();
    
    // 处理自动淡出逻辑
    if (m_isFadingOut) {
        if (!m_hasFadedOut) {
            m_fadeOutTimer++;
            if (m_fadeOutTimer >= FADE_OUT_DELAY / 1000) {
                if (m_opacityAnimation->state() == QPropertyAnimation::Stopped) {
                    m_opacityAnimation->setStartValue(1.0);
                    m_opacityAnimation->setEndValue(0.5);
                    m_opacityAnimation->start();
                }
            }
        }
    }
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
            
            // 根据设置构建时间显示内容
            if (m_showDate && m_showWeekday) {
                QString dateStr = cityTime.toString(m_dateFormat);
                QString weekdayStr = cityTime.toString(m_weekdayFormat);
                QString timeOnlyStr = cityTime.toString(m_timeFormat);
                
                timeStr = QString(
                    "<div style='text-align: right;'>"
                    "<span style='font-size: 10px; color: rgba(255, 255, 255, 150);'>%1 %2</span><br>"
                    "<span style='font-size: 24px; font-weight: bold; color: white;'>%3</span>"
                    "</div>"
                ).arg(dateStr, weekdayStr, timeOnlyStr);
            } else if (m_showDate) {
                QString dateStr = cityTime.toString(m_dateFormat);
                QString timeOnlyStr = cityTime.toString(m_timeFormat);
                
                timeStr = QString(
                    "<div style='text-align: right;'>"
                    "<span style='font-size: 10px; color: rgba(255, 255, 255, 150);'>%1</span><br>"
                    "<span style='font-size: 24px; font-weight: bold; color: white;'>%2</span>"
                    "</div>"
                ).arg(dateStr, timeOnlyStr);
            } else if (m_showWeekday) {
                QString weekdayStr = cityTime.toString(m_weekdayFormat);
                QString timeOnlyStr = cityTime.toString(m_timeFormat);
                
                timeStr = QString(
                    "<div style='text-align: right;'>"
                    "<span style='font-size: 10px; color: rgba(255, 255, 255, 150);'>%1</span><br>"
                    "<span style='font-size: 24px; font-weight: bold; color: white;'>%2</span>"
                    "</div>"
                ).arg(weekdayStr, timeOnlyStr);
            } else {
                QString timeOnlyStr = cityTime.toString(m_timeFormat);
                
                timeStr = QString(
                    "<div style='text-align: right;'>"
                    "<span style='font-size: 24px; font-weight: bold; color: white;'>%1</span>"
                    "</div>"
                ).arg(timeOnlyStr);
            }
            
            m_cityTimeLabels[i]->setText(timeStr);
        } else {
            m_cityTimeLabels[i]->setText(
                "<div style='text-align: right;'>"
                "<span style='font-size: 12px; color: rgba(255, 100, 100, 200);'>时区无效</span>"
                "</div>"
            );
        }
    }
    
    // 当时间标签内容发生变化（单行/多行切换）时，调整窗口大小
    adjustSize();
}

// 城市容器的悬停事件处理
void TimezoneWindow::onCityHovered(int index, bool isHovered)
{
    if (index >= 0 && index < m_cityContainers.size()) {
        if (isHovered) {
            m_hoveredCityIndex = index;
            m_cityContainers[index]->setStyleSheet(
                "background: rgba(255, 255, 255, 10); "
                "border-radius: 6px;"
            );
        } else {
            if (m_hoveredCityIndex == index) {
                m_hoveredCityIndex = -1;
            }
            m_cityContainers[index]->setStyleSheet(
                "background: transparent; "
                "border-radius: 6px;"
            );
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
    
    // 任何鼠标操作都重置淡出状态
    m_isFadingOut = false;
    m_hasFadedOut = false;
    m_fadeOutTimer = 0;
    setWindowOpacity(1.0);
    
    QWidget::mousePressEvent(event);
}

void TimezoneWindow::mouseMoveEvent(QMouseEvent *event)
{
    
    // 处理拖动
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
    }
    else {
        // 任何鼠标移动都重置淡出状态
        m_isFadingOut = false;
        m_hasFadedOut = false;
        m_fadeOutTimer = 0;
        setWindowOpacity(1.0);
    }
    QWidget::mouseMoveEvent(event);
}

void TimezoneWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
    }
    
    // 任何鼠标操作都重置淡出状态
    m_isFadingOut = false;
    m_hasFadedOut = false;
    m_fadeOutTimer = 0;
    setWindowOpacity(1.0);
    
    QWidget::mouseReleaseEvent(event);
}

void TimezoneWindow::closeEvent(QCloseEvent *event)
{
    // 对于无边框窗口，直接隐藏而不是关闭
    hide();
    event->ignore();
}

bool TimezoneWindow::eventFilter(QObject *obj, QEvent *event)
{
    // 处理城市容器的悬停事件
    if (obj->property("cityIndex").isValid()) {
        int index = obj->property("cityIndex").toInt();
        
        switch (event->type()) {
        case QEvent::Enter:
            onCityHovered(index, true);
            return true;
        case QEvent::Leave:
            onCityHovered(index, false);
            return true;
        default:
            break;
        }
    }
    
    return QWidget::eventFilter(obj, event);
}