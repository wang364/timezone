#include "CityManager.h"
#include <QDebug>
#include <QElapsedTimer>

CityManager* CityManager::m_instance = nullptr;

CityManager& CityManager::instance()
{
    if (!m_instance) {
        m_instance = new CityManager();
    }
    return *m_instance;
}

CityManager::CityManager(QObject *parent)
    : QObject(parent)
    , m_settings(new QSettings("TimezoneApp", "CityManager"))
    , m_dataLoaded(false)
{ 
    // 启动后台线程加载所有城市数据
    startBackgroundDataLoading();
    
    loadCities();
    
    // 调试输出：显示加载的城市数量
    qDebug() << "从设置加载的城市数量:" << m_cities.size();
    
}

CityManager::~CityManager()
{
    // 等待后台线程完成
    if (m_loadingFuture.isRunning()) {
        qDebug() << "等待后台数据加载线程完成...";
        m_loadingFuture.waitForFinished();
        qDebug() << "后台线程已完成";
    }
    
    saveCities();
    delete m_settings;
    
    qDebug() << "CityManager资源已正确释放";
}

void CityManager::addCity(const QString &cityName, const QString &timezoneId)
{
    if (cityExists(cityName)) {
        return;
    }
    
    CityInfo newCity(cityName, timezoneId, m_cities.size());
    qDebug() << "Add city name: " << cityName << " timezone: " << timezoneId;
    m_cities.append(newCity);
    saveCities();
    
    emit citiesChanged();
}

void CityManager::removeCity(const QString &cityName)
{
    for (int i = 0; i < m_cities.size(); ++i) {
        if (m_cities[i].name == cityName) {
            m_cities.removeAt(i);
            
            for (int j = i; j < m_cities.size(); ++j) {
                m_cities[j].displayOrder = j;
            }
            
            saveCities();
            emit citiesChanged();
            return;
        }
    }
}

void CityManager::moveCity(int fromIndex, int toIndex)
{
    if (fromIndex < 0 || fromIndex >= m_cities.size() || 
        toIndex < 0 || toIndex >= m_cities.size()) {
        return;
    }
    
    CityInfo city = m_cities.takeAt(fromIndex);
    m_cities.insert(toIndex, city);
    
    for (int i = 0; i < m_cities.size(); ++i) {
        m_cities[i].displayOrder = i;
    }
    
    saveCities();
    emit citiesChanged();
}

QList<CityInfo> CityManager::getCities() const
{
    QList<CityInfo> sortedCities = m_cities;
    std::sort(sortedCities.begin(), sortedCities.end(), 
              [](const CityInfo &a, const CityInfo &b) {
                  return a.displayOrder < b.displayOrder;
              });
    return sortedCities;
}

QList<QString> CityManager::getAllAvailableCities() const
{
    QMutexLocker locker(&m_dataMutex);
    
    if (m_dataLoaded) {
        // 数据已加载完成，返回完整城市列表
        return m_allAvailableCities;
    } else {
        // 数据尚未加载完成，返回常用城市列表
        return m_cityTimezoneMap.keys();
    }
}

int CityManager::calculateMatchScore(const QString &lowerCity, const QString &lowerKeyword) const
{
    int score = 0;
    
    // 精确匹配得分最高
    if (lowerCity == lowerKeyword) {
        score = 100;
    }
    // 开头匹配得分较高
    else if (lowerCity.startsWith(lowerKeyword)) {
        score = 80;
    }
    // 包含匹配得分中等
    else if (lowerCity.contains(lowerKeyword)) {
        score = 60;
    }
    // 部分匹配（按单词匹配）
    else {
        QStringList cityWords = lowerCity.split(' ', Qt::SkipEmptyParts);
        for (const QString &word : cityWords) {
            if (word.startsWith(lowerKeyword)) {
                score = 70;
                break;
            } else if (word.contains(lowerKeyword)) {
                score = 50;
                break;
            }
        }
    }
    
    return score;
}



bool CityManager::cityExists(const QString &cityName) const
{
    for (const CityInfo &city : m_cities) {
        if (city.name == cityName) {
            return true;
        }
    }
    return false;
}

bool CityManager::isCityValid(const QString &cityName) const
{
    if (cityName.isEmpty()) {
        return false;
    }
    
    // 使用互斥锁确保线程安全
    QMutexLocker locker(&m_dataMutex);
    
    // 检查城市是否存在于内存映射中
    return m_cityTimezoneMap.contains(cityName);
}

QString CityManager::getTimezoneForCity(const QString &cityName) const
{
    if (cityName.isEmpty()) {
        return QTimeZone::systemTimeZoneId();
    }
    
    // 使用互斥锁确保线程安全
    QMutexLocker locker(&m_dataMutex);
    
    // 只从内存映射中查找城市时区
    if (m_cityTimezoneMap.contains(cityName)) {
        return m_cityTimezoneMap.value(cityName);
    }
    
    // 如果找不到，返回系统时区ID
    return QTimeZone::systemTimeZoneId();
}

void CityManager::loadCities()
{
    int size = m_settings->beginReadArray("cities");
    for (int i = 0; i < size; ++i) {
        m_settings->setArrayIndex(i);
        CityInfo city;
        city.name = m_settings->value("name").toString();
        city.timezoneId = m_settings->value("timezone").toString();
        city.displayOrder = m_settings->value("order", i).toInt();
        m_cities.append(city);
    }
    m_settings->endArray();
}

void CityManager::saveCities()
{
    m_settings->beginWriteArray("cities");
    for (int i = 0; i < m_cities.size(); ++i) {
        m_settings->setArrayIndex(i);
        m_settings->setValue("name", m_cities[i].name);
        m_settings->setValue("timezone", m_cities[i].timezoneId);
        m_settings->setValue("order", m_cities[i].displayOrder);
    }
    m_settings->endArray();
    m_settings->sync();
}

void CityManager::startBackgroundDataLoading()
{
    qDebug() << "启动后台线程加载城市数据...";
    
    // 使用QtConcurrent在后台线程中加载数据
    m_loadingFuture = QtConcurrent::run(&CityManager::loadAllCitiesData, this);
}

void CityManager::loadAllCitiesData()
{
    QElapsedTimer timer;
    timer.start();
    
    QMutexLocker locker(&m_dataMutex);
    
    QFile file("data/cities.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开城市数据文件:" << file.errorString();
        return;
    }
    
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    // 一次性读取所有数据
    QString allData = in.readAll();
    file.close();

    QStringList lines = allData.split('\n', Qt::SkipEmptyParts);

    // 预分配内存以提高性能
    int estimatedSize = lines.size();
    m_allAvailableCities.reserve(estimatedSize * 2); // 假设每行平均有2个城市名
    
    // 使用索引循环而不是迭代器（性能更好）
    for (int i = 0; i < lines.size(); ++i) {
        QStringList parts = lines[i].split('\t');
        
        if (parts.size() >= 2) {
            QStringList cityNames = parts[0].split(',');
            
            // 使用 move 语义避免拷贝
            for (QString &cityName : cityNames) {
                QString trimmedName = cityName.trimmed();
                if (!trimmedName.isEmpty()) {
                    m_cityTimezoneMap[trimmedName] = parts[1];
                    m_allAvailableCities.append(std::move(trimmedName));
                }
            }
        }
    }
    
    m_dataLoaded = true;
    
    qint64 elapsed = timer.elapsed();
    qDebug() << "后台线程加载完成，共加载" << m_allAvailableCities.size() << "个城市数据，耗时" << elapsed << "毫秒";
    emit dataLoadingFinished(m_allAvailableCities.size());
}