#include "config.h"
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

Config* Config::m_instance = nullptr;

Config::Config(QObject *parent) : QObject(parent)
{
    // 配置文件路径：程序所在目录下的config.ini
    QString appDir = QCoreApplication::applicationDirPath();
    m_configFilePath = QDir(appDir).absoluteFilePath("config.ini");
    
    // 创建QSettings对象
    m_settings = new QSettings(m_configFilePath, QSettings::IniFormat, this);
    
    // 设置默认值
    setDefaultValues();
}

Config::~Config()
{
    if (m_settings) {
        m_settings->sync();
    }
}

Config* Config::instance()
{
    if (!m_instance) {
        m_instance = new Config();
    }
    return m_instance;
}

void Config::setDefaultValues()
{
    // 如果配置文件不存在或值为空，设置默认值
    if (!m_settings->contains("Paths/SevenZip")) {
        m_settings->setValue("Paths/SevenZip", "C:/Program Files (x86)/7-Zip/7z.exe");
    }
    
    if (!m_settings->contains("App/Name")) {
        m_settings->setValue("App/Name", "Crypt Browser");
    }
    
    if (!m_settings->contains("App/Version")) {
        m_settings->setValue("App/Version", "1.0.0");
    }
    
    if (!m_settings->contains("App/Organization")) {
        m_settings->setValue("App/Organization", "CryptBrowser");
    }
    
    m_settings->sync();
}

QString Config::sevenZipPath() const
{
    return m_settings->value("Paths/SevenZip", "C:/Program Files (x86)/7-Zip/7z.exe").toString();
}

QString Config::appName() const
{
    return m_settings->value("App/Name", "Crypt Browser").toString();
}

QString Config::appVersion() const
{
    return m_settings->value("App/Version", "1.0.0").toString();
}

QString Config::appOrganization() const
{
    return m_settings->value("App/Organization", "CryptBrowser").toString();
}

void Config::setSevenZipPath(const QString &path)
{
    m_settings->setValue("Paths/SevenZip", path);
    m_settings->sync();
}

QString Config::configFilePath() const
{
    return m_configFilePath;
} 