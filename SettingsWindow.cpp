#include "SettingsWindow.h"
#include "CityManager.h"
#include "TimezoneWindow.h"
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

SettingsWindow::SettingsWindow(TimezoneWindow *timezoneWindow, QWidget *parent)
    : QWidget(parent)
    , m_settings(new QSettings("TimezoneApp", "TimezoneTool"))
    , m_dataLoaded(false)
    , m_settingsModified(false)
    , m_timezoneWindow(timezoneWindow)
{
    setupUI();
    loadSettings();
    
    setWindowTitle(tr("时区工具设置"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    
    // 添加现代化的UI样式，减小间距
    setStyleSheet(
        "/* QGroupBox样式 */"
        "QGroupBox {"
        "    border: 1px solid #d0d0d0;"
        "    border-radius: 6px;"
        "    margin-top: 6px;"
        "    margin-bottom: 4px;"
        "    padding: 6px;"
        "    background-color: #fafafa;"
        "    box-shadow: 0 1px 2px rgba(0, 0, 0, 0.03);"
        "}"
        
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 4px 0 4px;"
        "    font-weight: bold;"
        "    color: #333;"
        "    font-size: 13px;"
        "}"
        
        "/* 按钮样式 */"
        "QPushButton {"
        "    border: 1px solid #ccc;"
        "    border-radius: 4px;"
        "    padding: 6px 12px;"
        "    background-color: #f0f0f0;"
        "    color: #333;"
        "    font-size: 12px;"
        "    min-width: 70px;"
        "}"
        
        "QPushButton:hover {"
        "    background-color: #e6e6e6;"
        "    border-color: #bbb;"
        "}"
        
        "QPushButton:pressed {"
        "    background-color: #d9d9d9;"
        "}"
        
        "/* QComboBox样式 */"
        "QComboBox {"
        "    border: 1px solid #ccc;"
        "    border-radius: 4px;"
        "    padding: 5px 25px 5px 7px;"
        "    background-color: white;"
        "    font-size: 12px;"
        "    min-width: 140px;"
        "}"
        
        "QComboBox:hover {"
        "    border-color: #999;"
        "}"
        
        "QComboBox:focus {"
        "    border-color: #4a90e2;"
        "    outline: none;"
        "}"
        
        "/* QCheckBox样式 */"
        "QCheckBox {"
        "    font-size: 12px;"
        "    margin: 3px 0;"
        "}"
        
        "/* QLabel样式 */"
        "QLabel {"
        "    font-size: 12px;"
        "    color: #333;"
        "    margin: 3px 0;"
        "}"
        
        "/* QLineEdit样式 */"
        "QLineEdit {"
        "    border: 1px solid #ccc;"
        "    border-radius: 5px;"
        "    padding: 6px 8px;"
        "    font-size: 13px;"
        "    background-color: white;"
        "}"
        
        "QLineEdit:hover {"
        "    border-color: #999;"
        "}"
        
        "QLineEdit:focus {"
        "    border-color: #4a90e2;"
        "    outline: none;"
        "}"
        
        "/* QListWidget样式 */"
        "QListWidget {"
        "    border: 1px solid #ccc;"
        "    border-radius: 5px;"
        "    font-size: 13px;"
        "    background-color: white;"
        "}"
        
        "QListWidget::item {"
        "    padding: 8px 10px;"
        "    border-bottom: 1px solid #f0f0f0;"
        "}"
        
        "QListWidget::item:hover {"
        "    background-color: #f5f5f5;"
        "}"
        
        "QListWidget::item:selected {"
        "    background-color: #e6f3ff;"
        "    color: #333;"
        "}"
        
        "/* QTabWidget样式 */"
        "QTabWidget::pane {"
        "    border: 1px solid #ddd;"
        "    border-radius: 5px;"
        "    background-color: white;"
        "    margin-top: 8px;"
        "}"
        
        "QTabBar::tab {"
        "    background-color: #f0f0f0;"
        "    border: 1px solid #ddd;"
        "    border-bottom: none;"
        "    border-top-left-radius: 5px;"
        "    border-top-right-radius: 5px;"
        "    padding: 8px 16px;"
        "    margin-right: 2px;"
        "    font-size: 13px;"
        "    font-weight: 500;"
        "    color: #666;"
        "}"
        
        "QTabBar::tab:selected {"
        "    background-color: white;"
        "    color: #333;"
        "    border-bottom-color: white;"
        "}"
        
        "QTabBar::tab:hover:!selected {"
        "    background-color: #e6e6e6;"
        "}"
    );
    
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
    
    // 时间显示格式设置组
    QGroupBox *timeDisplayGroup = new QGroupBox(tr("时间显示格式"));
    QVBoxLayout *timeDisplayLayout = new QVBoxLayout(timeDisplayGroup);
    
    // 水平排列时间格式和显示秒数选项
    QHBoxLayout *timeFormatLayout = new QHBoxLayout();
    m_formatLabel = new QLabel(tr("时间格式:"));
    m_formatComboBox = new QComboBox();
    m_formatComboBox->addItem(tr("24小时制"), "HH:mm:ss");
    m_formatComboBox->addItem(tr("12小时制"), "hh:mm:ss AP");
    m_formatComboBox->setToolTip(tr("选择时间的显示格式，24小时制或12小时制"));
    
    m_showSecondsCheckBox = new QCheckBox(tr("显示秒数"));
    m_showSecondsCheckBox->setToolTip(tr("选择是否在时间显示中包含秒数"));
    
    timeFormatLayout->addWidget(m_formatLabel);
    timeFormatLayout->addWidget(m_formatComboBox);
    timeFormatLayout->addStretch();
    timeFormatLayout->addWidget(m_showSecondsCheckBox);
    
    timeDisplayLayout->addLayout(timeFormatLayout);
    
    // 日期显示格式设置组
    QGroupBox *dateDisplayGroup = new QGroupBox(tr("日期显示格式"));
    QVBoxLayout *dateDisplayLayout = new QVBoxLayout(dateDisplayGroup);
    
    // 水平排列日期格式和星期格式选项
    QHBoxLayout *dateFormatsLayout = new QHBoxLayout();
    
    QVBoxLayout *dateFormatColumn = new QVBoxLayout();
    m_dateFormatLabel = new QLabel(tr("日期格式:"));
    m_dateFormatComboBox = new QComboBox();
    m_dateFormatComboBox->addItem(tr("YYYY-MM-DD"), "yyyy-MM-dd");
    m_dateFormatComboBox->addItem(tr("MM/DD/YYYY"), "MM/dd/yyyy");
    m_dateFormatComboBox->addItem(tr("DD/MM/YYYY"), "dd/MM/yyyy");
    m_dateFormatComboBox->setToolTip(tr("选择日期的显示格式，支持多种国际标准格式"));
    dateFormatColumn->addWidget(m_dateFormatLabel);
    dateFormatColumn->addWidget(m_dateFormatComboBox);
    
    QVBoxLayout *weekdayFormatColumn = new QVBoxLayout();
    m_weekdayFormatLabel = new QLabel(tr("星期格式:"));
    m_weekdayFormatComboBox = new QComboBox();
    m_weekdayFormatComboBox->addItem(tr("Mon"), "ddd");
    m_weekdayFormatComboBox->addItem(tr("Monday"), "dddd");
    m_weekdayFormatComboBox->setToolTip(tr("选择星期的显示格式，简写或全称"));
    weekdayFormatColumn->addWidget(m_weekdayFormatLabel);
    weekdayFormatColumn->addWidget(m_weekdayFormatComboBox);
    
    dateFormatsLayout->addLayout(dateFormatColumn);
    dateFormatsLayout->addStretch();
    dateFormatsLayout->addLayout(weekdayFormatColumn);
    
    // 水平排列显示选项
    QHBoxLayout *dateDisplayOptionsLayout = new QHBoxLayout();
    m_showDateCheckBox = new QCheckBox(tr("显示日期"));
    m_showDateCheckBox->setToolTip(tr("选择是否在主界面显示日期信息"));
    
    m_showWeekdayCheckBox = new QCheckBox(tr("显示星期"));
    m_showWeekdayCheckBox->setToolTip(tr("选择是否在主界面显示星期信息"));
    
    dateDisplayOptionsLayout->addWidget(m_showDateCheckBox);
    dateDisplayOptionsLayout->addStretch();
    dateDisplayOptionsLayout->addWidget(m_showWeekdayCheckBox);
    
    dateDisplayLayout->addLayout(dateFormatsLayout);
    dateDisplayLayout->addLayout(dateDisplayOptionsLayout);
    
    // 语言设置组
    QGroupBox *languageGroup = new QGroupBox(tr("语言设置"));
    QHBoxLayout *languageLayout = new QHBoxLayout(languageGroup);
    
    m_languageLabel = new QLabel(tr("语言:"));
    m_languageComboBox = new QComboBox();
    m_languageComboBox->addItem(tr("中文"), "zh");
    m_languageComboBox->addItem(tr("English"), "en");
    m_languageComboBox->addItem(tr("日本語"), "ja");
    m_languageComboBox->addItem(tr("한국어"), "ko");
    m_languageComboBox->addItem(tr("Français"), "fr");
    m_languageComboBox->addItem(tr("Deutsch"), "de");
    m_languageComboBox->addItem(tr("Español"), "es");
    m_languageComboBox->addItem(tr("Português"), "pt");
    m_languageComboBox->setToolTip(tr("选择应用程序的界面语言"));
    
    languageLayout->addWidget(m_languageLabel);
    languageLayout->addWidget(m_languageComboBox);
    languageLayout->addStretch();
    
    // 启动设置组
    QGroupBox *startupGroup = new QGroupBox(tr("启动设置"));
    QHBoxLayout *startupLayout = new QHBoxLayout(startupGroup);
    
    m_startWithSystemCheckBox = new QCheckBox(tr("开机自启动"));
    m_startWithSystemCheckBox->setToolTip(tr("选择是否在系统启动时自动运行应用程序"));
    
    startupLayout->addWidget(m_startWithSystemCheckBox);
    startupLayout->addStretch();
    
    // 将各组添加到常规设置页面，减少组之间的间距
    generalLayout->addWidget(timeDisplayGroup);
    generalLayout->addWidget(dateDisplayGroup);
    generalLayout->addWidget(languageGroup);
    generalLayout->addWidget(startupGroup);
    generalLayout->addStretch(1); // 减少拉伸空间
    
    // 调整窗口的最小尺寸和默认尺寸
    setMinimumSize(600, 450); // 减少高度
    resize(700, 550); // 减少默认高度
    
    setupCityManagementTab();
    
    m_tabWidget->addTab(generalTab, tr("常规设置"));
    m_tabWidget->addTab(m_cityManagementTab, tr("城市管理"));
    
    m_mainLayout->addWidget(m_tabWidget);
    
    m_buttonLayout = new QHBoxLayout();
    m_saveButton = new QPushButton(tr("保存"));
    m_cancelButton = new QPushButton(tr("取消"));
    
    m_buttonLayout->addStretch();
    m_buttonLayout->addWidget(m_saveButton);
    m_buttonLayout->addWidget(m_cancelButton);
    
    m_mainLayout->addLayout(m_buttonLayout);
    
    // 连接设置修改信号
    connect(m_formatComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SettingsWindow::updateModifiedStatus);
    connect(m_dateFormatComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SettingsWindow::updateModifiedStatus);
    connect(m_weekdayFormatComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SettingsWindow::updateModifiedStatus);
    connect(m_languageComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SettingsWindow::updateModifiedStatus);
    connect(m_startWithSystemCheckBox, &QCheckBox::checkStateChanged, this, &SettingsWindow::updateModifiedStatus);
    connect(m_showSecondsCheckBox, &QCheckBox::checkStateChanged, this, &SettingsWindow::updateModifiedStatus);
    connect(m_showDateCheckBox, &QCheckBox::checkStateChanged, this, &SettingsWindow::updateModifiedStatus);
    connect(m_showWeekdayCheckBox, &QCheckBox::checkStateChanged, this, &SettingsWindow::updateModifiedStatus);
    
    connect(m_saveButton, &QPushButton::clicked, this, &SettingsWindow::onSaveSettings);
    connect(m_cancelButton, &QPushButton::clicked, this, &SettingsWindow::onCancel);
}

void SettingsWindow::setupCityManagementTab()
{
    m_cityManagementTab = new QWidget();
    QVBoxLayout *cityLayout = new QVBoxLayout(m_cityManagementTab);
    cityLayout->setContentsMargins(10, 10, 10, 10); // 设置整体边距
    cityLayout->setSpacing(0); // 减少布局间距
    
    // 搜索和添加区域
    QVBoxLayout *searchLayout = new QVBoxLayout();
    searchLayout->setContentsMargins(0, 0, 0, 8); // 减少底部边距
    searchLayout->setSpacing(3); // 减小控件间距
    
    QLabel *searchLabel = new QLabel(tr("添加城市:"));
    searchLabel->setStyleSheet("font-weight: bold; font-size: 13px; margin-bottom: 0px;");
    
    m_citySearchEdit = new QLineEdit();
    m_citySearchEdit->setPlaceholderText(tr("输入城市名称搜索，回车或从下拉列表选择添加..."));
    m_citySearchEdit->setStyleSheet("QLineEdit { padding: 6px 8px; font-size: 12px; }");
    
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
    
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(m_citySearchEdit);
    
    // 已添加城市列表    
    m_cityListWidget = new QListWidget();
    m_cityListWidget->setStyleSheet(
        "QListWidget { "
        "    border: 1px solid #ccc; "
        "    border-radius: 3px; "
        "    padding: 0px; "
        "    margin: 0px; "
        "}"
        "QListWidget::item { "
        "    padding: 6px 10px; "
        "    border-bottom: 1px solid #f0f0f0; "
        "    font-size: 12px; "
        "}"
    );
    m_cityListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 设置城市列表占据尽可能多的空间
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 8, 0, 0); // 减少顶部边距
    buttonLayout->setSpacing(8); // 调整按钮间距
    
    m_removeCityButton = new QPushButton(tr("删除"));
    m_removeCityButton->setStyleSheet("QPushButton { padding: 5px 12px; font-size: 12px; }");
    
    m_moveUpButton = new QPushButton(tr("上移"));
    m_moveUpButton->setStyleSheet("QPushButton { padding: 5px 12px; font-size: 12px; }");
    
    m_moveDownButton = new QPushButton(tr("下移"));
    m_moveDownButton->setStyleSheet("QPushButton { padding: 5px 12px; font-size: 12px; }");
    
    buttonLayout->addWidget(m_removeCityButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_moveUpButton);
    buttonLayout->addWidget(m_moveDownButton);
    
    cityLayout->addLayout(searchLayout);
    cityLayout->addWidget(m_cityListWidget, 1);
    //cityLayout->addStretch();
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
            updateModifiedStatus(); // 标记设置已修改
        }
    });
    connect(m_moveDownButton, &QPushButton::clicked, this, [this]() {
        int currentRow = m_cityListWidget->currentRow();
        if (currentRow < m_cityListWidget->count() - 1) {
            CityManager::instance().moveCity(currentRow, currentRow + 1);
            refreshCityList();
            m_cityListWidget->setCurrentRow(currentRow + 1);
            updateModifiedStatus(); // 标记设置已修改
        }
    });
    connect(m_cityListWidget, &QListWidget::currentRowChanged, this, &SettingsWindow::onCitySelectionChanged);
}

void SettingsWindow::refreshCityList()
{
    m_cityListWidget->clear();
    
    QList<CityInfo> cities = CityManager::instance().getCities();
    for (const CityInfo &city : cities) {
        // 在城市列表中显示时区信息
        QListWidgetItem *item = new QListWidgetItem(QString("%1 (%2)").arg(city.name).arg(city.timezoneId));
        m_cityListWidget->addItem(item);
    }
}

void SettingsWindow::updateCityButtons()
{
    bool hasSelection = m_cityListWidget->currentRow() >= 0;
    m_removeCityButton->setEnabled(hasSelection);
    m_moveUpButton->setEnabled(hasSelection && m_cityListWidget->currentRow() > 0);
    m_moveDownButton->setEnabled(hasSelection && m_cityListWidget->currentRow() < m_cityListWidget->count() - 1);
}

void SettingsWindow::updateModifiedStatus()
{
    if (!m_settingsModified) {
        m_settingsModified = true;
        setWindowTitle(tr("时区工具设置 (*)") + ""); // 添加星号表示有未保存的修改
        m_saveButton->setText(tr("保存"));
    }
}

void SettingsWindow::loadSettings()
{
    QString timeFormat = m_settings->value("timeFormat", "HH:mm:ss").toString();
    QString dateFormat = m_settings->value("dateFormat", "yyyy-MM-dd").toString();
    QString weekdayFormat = m_settings->value("weekdayFormat", "ddd").toString();
    QString language = m_settings->value("language", "zh").toString();
    bool startWithSystem = m_settings->value("startWithSystem", false).toBool();
    bool showSeconds = m_settings->value("showSeconds", true).toBool();
    bool showDate = m_settings->value("showDate", true).toBool();
    bool showWeekday = m_settings->value("showWeekday", false).toBool();
    
    int formatIndex = m_formatComboBox->findData(timeFormat);
    if (formatIndex >= 0) {
        m_formatComboBox->setCurrentIndex(formatIndex);
    }
    
    int dateFormatIndex = m_dateFormatComboBox->findData(dateFormat);
    if (dateFormatIndex >= 0) {
        m_dateFormatComboBox->setCurrentIndex(dateFormatIndex);
    }
    
    int weekdayFormatIndex = m_weekdayFormatComboBox->findData(weekdayFormat);
    if (weekdayFormatIndex >= 0) {
        m_weekdayFormatComboBox->setCurrentIndex(weekdayFormatIndex);
    }
    
    // 加载语言设置
    int languageIndex = m_languageComboBox->findData(language);
    if (languageIndex >= 0) {
        m_languageComboBox->setCurrentIndex(languageIndex);
    }
    
    m_startWithSystemCheckBox->setChecked(startWithSystem);
    m_showSecondsCheckBox->setChecked(showSeconds);
    m_showDateCheckBox->setChecked(showDate);
    m_showWeekdayCheckBox->setChecked(showWeekday);
}

void SettingsWindow::onSaveSettings()
{
    m_settings->setValue("timeFormat", m_formatComboBox->currentData().toString());
    m_settings->setValue("dateFormat", m_dateFormatComboBox->currentData().toString());
    m_settings->setValue("weekdayFormat", m_weekdayFormatComboBox->currentData().toString());
    m_settings->setValue("language", m_languageComboBox->currentData().toString());
    m_settings->setValue("startWithSystem", m_startWithSystemCheckBox->isChecked());
    m_settings->setValue("showSeconds", m_showSecondsCheckBox->isChecked());
    m_settings->setValue("showDate", m_showDateCheckBox->isChecked());
    m_settings->setValue("showWeekday", m_showWeekdayCheckBox->isChecked());
    
    m_settings->sync();
    
    if (m_timezoneWindow) {
        m_timezoneWindow->reloadSettings();
    }
    
    // 重置修改状态
    m_settingsModified = false;
    setWindowTitle(tr("时区工具设置"));
    m_saveButton->setText(tr("保存"));
    
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
    
    qDebug() << "补全器模型已更新，包含" << m_allCitiesList.size() << "个城市";
}

void SettingsWindow::onAddCityFromSearch()
{
    QString cityName = m_citySearchEdit->text().trimmed();
    if (cityName.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请输入城市名称"));
        return;
    }
    
    addCityByName(cityName);
    m_citySearchEdit->clear();
}

void SettingsWindow::addCityByName(const QString &cityName)
{
    // 检查输入的城市是否有效（存在于可用列表中）
    if (!CityManager::instance().isCityValid(cityName)) {
        QMessageBox::warning(this, tr("警告"), QString(tr("城市 '%1' 未找到或不在可用列表中")).arg(cityName));  
        return;
    }
    
    QString timezoneId = CityManager::instance().getTimezoneForCity(cityName);
    CityManager::instance().addCity(cityName, timezoneId);
    refreshCityList();
    updateModifiedStatus(); // 标记设置已修改
    
    // 显示添加成功提示
    QMessageBox::information(this, tr("添加成功"), QString(tr("已成功添加城市 '%1'！")).arg(cityName));
}

void SettingsWindow::onRemoveCity()
{
    int currentRow = m_cityListWidget->currentRow();
    if (currentRow < 0) {
        return;
    }
    
    QListWidgetItem *item = m_cityListWidget->item(currentRow);
    QString cityText = item->text();
    QString cityName = cityText.left(cityText.indexOf('(')).trimmed();
    
    int result = QMessageBox::question(this, tr("确认删除"), QString(tr("确定要删除城市 '%1' 吗？")).arg(cityName));
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