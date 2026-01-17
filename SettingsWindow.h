#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSettings>
#include <QToolButton>
#include <QCompleter>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QSizePolicy>
#include "CityManager.h"
#include "StartupManager.h"

class TimezoneWindow;

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(TimezoneWindow *timezoneWindow = nullptr, QWidget *parent = nullptr);
    ~SettingsWindow();

private slots:
    void onSaveSettings();
    void onCancel();
    void onAddCityFromSearch();
    void onCompleterActivated(const QString &completion);
    void onRemoveCity();
    void onCitySelectionChanged();
    void onDataLoadingFinished(int cityCount);

private:
    void addCityByName(const QString &cityName);

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void loadSettings();
    void setupUI();
    void setupCityManagementTab();
    void refreshCityList();
    void updateCityButtons();
    
    QTabWidget *m_tabWidget;
    
    QVBoxLayout *m_mainLayout;
    
    QLabel *m_formatLabel;
    QComboBox *m_formatComboBox;
    
    QLabel *m_dateFormatLabel;
    QComboBox *m_dateFormatComboBox;
    
    QLabel *m_weekdayFormatLabel;
    QComboBox *m_weekdayFormatComboBox;
    
    // 语言设置
    QLabel *m_languageLabel;
    QComboBox *m_languageComboBox;
    
    QCheckBox *m_startWithSystemCheckBox;
    QCheckBox *m_showSecondsCheckBox;
    QCheckBox *m_showDateCheckBox;
    QCheckBox *m_showWeekdayCheckBox;
    
    QHBoxLayout *m_buttonLayout;
    QPushButton *m_saveButton;
    QPushButton *m_cancelButton;
    
    QSettings *m_settings;
    
    QWidget *m_cityManagementTab;
    QLineEdit *m_citySearchEdit;
    QListWidget *m_cityListWidget;
    QPushButton *m_removeCityButton;
    QPushButton *m_moveUpButton;
    QPushButton *m_moveDownButton;
    QCompleter *m_cityCompleter;
    QStringListModel *m_cityListModel;
    QSortFilterProxyModel *m_proxyModel;
    QStringList m_allCitiesList;
    bool m_dataLoaded;
    bool m_settingsModified; // 跟踪设置是否被修改
    
    TimezoneWindow *m_timezoneWindow;
};

#endif // SETTINGSWINDOW_H
