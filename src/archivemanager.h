#ifndef ARCHIVEMANAGER_H
#define ARCHIVEMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QProcess>
#include <QTemporaryDir>

class ArchiveManager : public QObject
{
    Q_OBJECT

public:
    explicit ArchiveManager(QObject *parent = nullptr);
    ~ArchiveManager();

    // 设置压缩包路径
    bool setArchivePath(const QString &path);
    
    // 解压压缩包（需要密码）
    bool extractArchive(const QString &password);
    
    // 获取解压后的文件列表（只返回HTML/MHTML文件）
    QStringList getHtmlFiles() const;
    
    // 获取临时目录路径
    QString getTempDir() const;
    
    // 清理临时文件
    void cleanup();
    
    // 检查是否已解压
    bool isExtracted() const;

private:
    QString m_archivePath;
    QTemporaryDir *m_tempDir;
    QStringList m_htmlFiles;
    bool m_extracted;
    
    // 扫描HTML文件
    void scanHtmlFiles();
    
    // 递归删除目录
    bool removeDir(const QString &dirPath);
};

#endif // ARCHIVEMANAGER_H 