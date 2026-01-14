#include "SettingsWindow.h"
#include "CityManager.h"
#include <QApplication>
#include <QTimeZone>
#include <QTabWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QInputDialog>
#include <QKeyEvent>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent)
    , m_settings(new QSettings("TimezoneApp", "TimezoneTool"))
    , m_dataLoaded(false)
{
    setupUI();
    loadSettings();
    
    setWindowTitle("时区工具设置");
    setFixedSize(600, 500);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    
    // 设置窗口图标
    setWindowIcon(QIcon("icon/timezone.svg"));
    
    // 连接数据加载完成信号
    connect(&CityManager::instance(), &CityManager::dataLoadingFinished, 
            this, &SettingsWindow::onDataLoadingFinished);
}

SettingsWindow::~SettingsWindow()
{
    delete m_settings;
}

void SettingsWindow::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    m_tabWidget = new QTabWidget();
    
    QWidget *generalTab = new QWidget();
    QVBoxLayout *generalLayout = new QVBoxLayout(generalTab);
    
    m_formatLabel = new QLabel("时间格式:");
    m_formatComboBox = new QComboBox();
    m_formatComboBox->addItem("24小时制", "HH:mm:ss");
    m_formatComboBox->addItem("12小时制", "hh:mm:ss AP");
    
    m_dateFormatLabel = new QLabel("日期格式:");
    m_dateFormatComboBox = new QComboBox();
    m_dateFormatComboBox->addItem("YYYY-MM-DD", "yyyy-MM-dd");
    m_dateFormatComboBox->addItem("MM/DD/YYYY", "MM/dd/yyyy");
    m_dateFormatComboBox->addItem("DD/MM/YYYY", "dd/MM/yyyy");
    
    m_startWithSystemCheckBox = new QCheckBox("开机自启动");
    m_showSecondsCheckBox = new QCheckBox("显示秒数");
    m_showDateCheckBox = new QCheckBox("显示日期");
    
    generalLayout->addWidget(m_formatLabel);
    generalLayout->addWidget(m_formatComboBox);
    generalLayout->addSpacing(10);
    generalLayout->addWidget(m_dateFormatLabel);
    generalLayout->addWidget(m_dateFormatComboBox);
    generalLayout->addSpacing(10);
    generalLayout->addWidget(m_startWithSystemCheckBox);
    generalLayout->addWidget(m_showSecondsCheckBox);
    generalLayout->addWidget(m_showDateCheckBox);
    generalLayout->addStretch();
    
    setupCityManagementTab();
    
    m_tabWidget->addTab(generalTab, "常规设置");
    m_tabWidget->addTab(m_cityManagementTab, "城市管理");
    
    m_mainLayout->addWidget(m_tabWidget);
    
    m_buttonLayout = new QHBoxLayout();
    m_saveButton = new QPushButton("保存");
    m_cancelButton = new QPushButton("取消");
    
    m_buttonLayout->addStretch();
    m_buttonLayout->addWidget(m_saveButton);
    m_buttonLayout->addWidget(m_cancelButton);
    
    m_mainLayout->addLayout(m_buttonLayout);
    
    connect(m_saveButton, &QPushButton::clicked, this, &SettingsWindow::onSaveSettings);
    connect(m_cancelButton, &QPushButton::clicked, this, &SettingsWindow::onCancel);
}

void SettingsWindow::setupCityManagementTab()
{
    m_cityManagementTab = new QWidget();
    QVBoxLayout *cityLayout = new QVBoxLayout(m_cityManagementTab);
    
    // 搜索和添加区域
    QVBoxLayout *searchLayout = new QVBoxLayout();
    
    QLabel *searchLabel = new QLabel("添加城市:");
    searchLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin-bottom: 5px;");
    
    m_citySearchEdit = new QLineEdit();
    m_citySearchEdit->setPlaceholderText("输入城市名称搜索，回车或从下拉列表选择添加...");
    
    m_cityListModel = new QStringListModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_cityListModel);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterKeyColumn(0);
    
    m_cityCompleter = new QCompleter(m_proxyModel, this);
    m_cityCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    m_cityCompleter->setCompletionMode(QCompleter::PopupCompletion);
    m_cityCompleter->setMaxVisibleItems(10);
    
    m_cityCompleter->popup()->setStyleSheet(
        "QListView {"
        "    border: 1px solid #ccc;"
        "    background-color: white;"
        "    selection-background-color: #e6f3ff;"
        "    outline: none;"
        "}"
        "QListView::item {"
        "    padding: 5px 10px;"
        "    border-bottom: 1px solid #f0f0f0;"
        "}"
        "QListView::item:selected {"
        "    background-color: #e6f3ff;"
        "}"
    );
    
    m_citySearchEdit->setCompleter(m_cityCompleter);
    m_citySearchEdit->setEnabled(false);
    
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(m_citySearchEdit);
    
    // 已添加城市列表
    QLabel *cityListLabel = new QLabel("已添加的城市:");
    cityListLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin-top: 15px; margin-bottom: 5px;");
    
    m_cityListWidget = new QListWidget();
    m_cityListWidget->setStyleSheet("QListWidget { border: 1px solid #ccc; border-radius: 3px; }");
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_removeCityButton = new QPushButton("删除");
    m_moveUpButton = new QPushButton("上移");
    m_moveDownButton = new QPushButton("下移");
    
    buttonLayout->addWidget(m_removeCityButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_moveUpButton);
    buttonLayout->addWidget(m_moveDownButton);
    
    cityLayout->addLayout(searchLayout);
    cityLayout->addWidget(cityListLabel);
    cityLayout->addWidget(m_cityListWidget);
    cityLayout->addLayout(buttonLayout);
    cityLayout->addStretch();
    
    refreshCityList();
    updateCityButtons();
    
    connect(m_citySearchEdit, &QLineEdit::returnPressed, this, &SettingsWindow::onAddCityFromSearch);
    connect(m_cityCompleter, QOverload<const QString &>::of(&QCompleter::activated), this, &SettingsWindow::onCompleterActivated);
    connect(m_removeCityButton, &QPushButton::clicked, this, &SettingsWindow::onRemoveCity);
    connect(m_moveUpButton, &QPushButton::clicked, this, [this]() {
        int currentRow = m_cityListWidget->currentRow();
        if (currentRow > 0) {
            CityManager::instance().moveCity(currentRow, currentRow - 1);
            refreshCityList();
            m_cityListWidget->setCurrentRow(currentRow - 1);
        }
    });
    connect(m_moveDownButton, &QPushButton::clicked, this, [this]() {
        int currentRow = m_cityListWidget->currentRow();
        if (currentRow < m_cityListWidget->count() - 1) {
            CityManager::instance().moveCity(currentRow, currentRow + 1);
            refreshCityList();
            m_cityListWidget->setCurrentRow(currentRow + 1);
        }
    });
    connect(m_cityListWidget, &QListWidget::currentRowChanged, this, &SettingsWindow::onCitySelectionChanged);
}

void SettingsWindow::refreshCityList()
{
    m_cityListWidget->clear();
    
    QList<CityInfo> cities = CityManager::instance().getCities();
    for (const CityInfo &city : cities) {
        m_cityListWidget->addItem(city.name);
    }
}

void SettingsWindow::updateCityButtons()
{
    bool hasSelection = m_cityListWidget->currentRow() >= 0;
    m_removeCityButton->setEnabled(hasSelection);
    m_moveUpButton->setEnabled(hasSelection && m_cityListWidget->currentRow() > 0);
    m_moveDownButton->setEnabled(hasSelection && m_cityListWidget->currentRow() < m_cityListWidget->count() - 1);
}

void SettingsWindow::loadSettings()
{
    QString timeFormat = m_settings->value("timeFormat", "HH:mm:ss").toString();
    QString dateFormat = m_settings->value("dateFormat", "yyyy-MM-dd").toString();
    bool startWithSystem = m_settings->value("startWithSystem", false).toBool();
    bool showSeconds = m_settings->value("showSeconds", true).toBool();
    bool showDate = m_settings->value("showDate", true).toBool();
    
    int formatIndex = m_formatComboBox->findData(timeFormat);
    if (formatIndex >= 0) {
        m_formatComboBox->setCurrentIndex(formatIndex);
    }
    
    int dateFormatIndex = m_dateFormatComboBox->findData(dateFormat);
    if (dateFormatIndex >= 0) {
        m_dateFormatComboBox->setCurrentIndex(dateFormatIndex);
    }
    
    m_startWithSystemCheckBox->setChecked(startWithSystem);
    m_showSecondsCheckBox->setChecked(showSeconds);
    m_showDateCheckBox->setChecked(showDate);
}

void SettingsWindow::onSaveSettings()
{
    m_settings->setValue("timeFormat", m_formatComboBox->currentData().toString());
    m_settings->setValue("dateFormat", m_dateFormatComboBox->currentData().toString());
    m_settings->setValue("startWithSystem", m_startWithSystemCheckBox->isChecked());
    m_settings->setValue("showSeconds", m_showSecondsCheckBox->isChecked());
    m_settings->setValue("showDate", m_showDateCheckBox->isChecked());
    
    m_settings->sync();
    close();
}

void SettingsWindow::onCancel()
{
    close();
}

bool SettingsWindow::eventFilter(QObject *obj, QEvent *event)
{
    // 事件过滤器现在主要用于其他功能
    // QCompleter会自动处理键盘导航，无需额外处理
    return QWidget::eventFilter(obj, event);
}

void SettingsWindow::onCompleterActivated(const QString &completion)
{
    // 从补全文本中提取城市名称（格式："城市名称 (时区)"）
    QString cityName;
    int parenIndex = completion.indexOf('(');
    if (parenIndex > 0) {
        cityName = completion.left(parenIndex).trimmed();
    } else {
        cityName = completion;
    }
    
    addCityByName(cityName);
    m_citySearchEdit->clear();
}

void SettingsWindow::onDataLoadingFinished(int cityCount)
{
    qDebug() << "数据加载完成，共加载" << cityCount << "个城市";
    
    m_dataLoaded = true;
    
    m_allCitiesList = CityManager::instance().getAllAvailableCities();
    
    m_cityListModel->setStringList(m_allCitiesList);
    m_citySearchEdit->setEnabled(true);
    
    qDebug() << "补全器模型已更新，包含" << m_allCitiesList.size() << "个城市";
}

void SettingsWindow::onAddCityFromSearch()
{
    QString cityName = m_citySearchEdit->text().trimmed();
    if (cityName.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入城市名称");
        return;
    }
    
    // 检查输入的城市是否有效（有时区信息）
    QString timezoneId = CityManager::instance().getTimezoneForCity(cityName);
    if (timezoneId == QTimeZone::systemTimeZoneId()) {
        // 如果返回的是系统时区，说明城市可能不存在
        QMessageBox::warning(this, "警告", QString("城市 '%1' 未找到或不在可用列表中").arg(cityName));
        return;
    }
    
    addCityByName(cityName);
    m_citySearchEdit->clear();
}

void SettingsWindow::addCityByName(const QString &cityName)
{
    QString timezoneId;
    if (cityName == "北京" || cityName == "上海" || cityName == "广州" || cityName == "深圳") {
        timezoneId = "Asia/Shanghai";
    } else if (cityName == "纽约" || cityName == "华盛顿") {
        timezoneId = "America/New_York";
    } else if (cityName == "洛杉矶" || cityName == "旧金山") {
        timezoneId = "America/Los_Angeles";
    } else if (cityName == "伦敦") {
        timezoneId = "Europe/London";
    } else if (cityName == "东京") {
        timezoneId = "Asia/Tokyo";
    } else {
        timezoneId = QTimeZone::systemTimeZoneId();
    }
    
    CityManager::instance().addCity(cityName, timezoneId);
    refreshCityList();
}

void SettingsWindow::onRemoveCity()
{
    int currentRow = m_cityListWidget->currentRow();
    if (currentRow < 0) {
        return;
    }
    
    QListWidgetItem *item = m_cityListWidget->item(currentRow);
    QString cityName = item->text().replace(" ★", "");
    
    int result = QMessageBox::question(this, "确认删除", 
                                      QString("确定要删除城市 '%1' 吗？").arg(cityName));
    if (result == QMessageBox::Yes) {
        CityManager::instance().removeCity(cityName);
        refreshCityList();
    }
}



void SettingsWindow::onCitySelectionChanged()
{
    updateCityButtons();
}

void SettingsWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}