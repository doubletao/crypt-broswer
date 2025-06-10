#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>
#include <QString>

class Config : public QObject
{
    Q_OBJECT

public:
    // 单例模式
    static Config* instance();
    
    // 获取配置值
    QString sevenZipPath() const;
    QString appName() const;
    QString appVersion() const;
    QString appOrganization() const;
    QString configFilePath() const;
    
    // 设置配置值
    void setSevenZipPath(const QString &path);

private:
    explicit Config(QObject *parent = nullptr);
    ~Config();
    
    // 设置默认值
    void setDefaultValues();
    
    static Config* m_instance;
    QSettings* m_settings;
    QString m_configFilePath;
};

#endif // CONFIG_H 