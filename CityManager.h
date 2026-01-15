#ifndef CITYMANAGER_H
#define CITYMANAGER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QTimeZone>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QStringConverter>
#include <QThread>
#include <QMutex>
#include <QFuture>
#include <QtConcurrent>

struct CityInfo {
    QString name;
    QString timezoneId;
    int displayOrder;
    
    CityInfo(const QString &cityName = "", const QString &tzId = "", int order = 0)
        : name(cityName), timezoneId(tzId), displayOrder(order) {}
    
    bool operator==(const CityInfo &other) const {
        return name == other.name && timezoneId == other.timezoneId;
    }
};

class CityManager : public QObject
{
    Q_OBJECT

public:
    static CityManager& instance();
    
    void addCity(const QString &cityName, const QString &timezoneId);
    void removeCity(const QString &cityName);
    void moveCity(int fromIndex, int toIndex);
    
    QList<CityInfo> getCities() const;
    QList<QString> getAllAvailableCities() const;
    
    bool cityExists(const QString &cityName) const;
    QString getTimezoneForCity(const QString &cityName) const;
    bool isCityValid(const QString &cityName) const;

signals:
    void citiesChanged();
    void cityAdded(const QString &cityName);
    void cityRemoved(const QString &cityName);
    void dataLoadingFinished(int cityCount);
    
private:
    explicit CityManager(QObject *parent = nullptr);
    ~CityManager();
    
    void loadCities();
    void saveCities();
    int calculateMatchScore(const QString &lowerCity, const QString &lowerKeyword) const;
    void loadAllCitiesData();
    void startBackgroundDataLoading();
    
    QList<CityInfo> m_cities;
    QMap<QString, QString> m_cityTimezoneMap;
    QStringList m_allAvailableCities;
    QSettings *m_settings;
    
    // 线程安全的数据结构
    mutable QMutex m_dataMutex;
    bool m_dataLoaded;
    QFuture<void> m_loadingFuture;
    
    static CityManager *m_instance;
};

#endif // CITYMANAGER_H