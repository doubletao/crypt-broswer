#include "archivemanager.h"
#include "config.h"
#include <QApplication>
#include <QStandardPaths>
#include <QMessageBox>
#include <QFileInfo>
#include <QDirIterator>
#include <QDebug>

ArchiveManager::ArchiveManager(QObject *parent)
    : QObject(parent)
    , m_tempDir(nullptr)
    , m_extracted(false)
{
}

ArchiveManager::~ArchiveManager()
{
    cleanup();
}

bool ArchiveManager::setArchivePath(const QString &path)
{
    if (!QFile::exists(path)) {
        return false;
    }
    
    m_archivePath = path;
    
    // 创建临时目录
    if (m_tempDir) {
        delete m_tempDir;
    }
    
    m_tempDir = new QTemporaryDir();
    if (!m_tempDir->isValid()) {
        QMessageBox::critical(nullptr, "Error", "Cannot create temporary directory");
        return false;
    }
    
    return true;
}

bool ArchiveManager::extractArchive(const QString &password)
{
    if (m_archivePath.isEmpty() || !m_tempDir || !m_tempDir->isValid()) {
        return false;
    }
    
    // 构建 7z 解压命令
    QProcess process;
    QString program = Config::instance()->sevenZipPath();
    QStringList arguments;
    
    arguments << "x"                    // 解压命令
              << m_archivePath          // 压缩包路径
              << "-o" + m_tempDir->path() // 输出目录
              << "-y";                  // 自动确认
    
    if (!password.isEmpty()) {
        arguments << "-p" + password;   // 密码
    }
    
    // 执行解压命令
    process.start(program, arguments);
    
    if (!process.waitForStarted()) {
        QMessageBox::critical(nullptr, "Error", "Cannot start 7z.exe, please ensure 7z is installed");
        return false;
    }
    
    if (!process.waitForFinished(30000)) { // 30秒超时
        QMessageBox::critical(nullptr, "Error", "Extraction timeout");
        process.kill();
        return false;
    }
    
    if (process.exitCode() != 0) {
        QString errorOutput = process.readAllStandardError();
        QMessageBox::critical(nullptr, "Extraction Failed", QString("Extraction failed, possibly wrong password or corrupted file.\nError: %1").arg(errorOutput));
        return false;
    }
    
    m_extracted = true;
    scanHtmlFiles();
    
    return true;
}

QStringList ArchiveManager::getHtmlFiles() const
{
    return m_htmlFiles;
}

QString ArchiveManager::getTempDir() const
{
    if (m_tempDir && m_tempDir->isValid()) {
        return m_tempDir->path();
    }
    return QString();
}

void ArchiveManager::cleanup()
{
    if (m_tempDir) {
        delete m_tempDir;
        m_tempDir = nullptr;
    }
    m_extracted = false;
    m_htmlFiles.clear();
}

bool ArchiveManager::isExtracted() const
{
    return m_extracted;
}

void ArchiveManager::scanHtmlFiles()
{
    m_htmlFiles.clear();
    
    if (!m_tempDir || !m_tempDir->isValid()) {
        return;
    }
    
    QDir tempDir(m_tempDir->path());
    QDirIterator iterator(tempDir.absolutePath(), 
                         QStringList() << "*.html" << "*.htm" << "*.mhtml" << "*.mht",
                         QDir::Files, 
                         QDirIterator::Subdirectories);
    
    while (iterator.hasNext()) {
        QString filePath = iterator.next();
        QFileInfo fileInfo(filePath);
        m_htmlFiles.append(fileInfo.absoluteFilePath());
    }
    
    // 按文件名排序
    m_htmlFiles.sort();
}

bool ArchiveManager::removeDir(const QString &dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists()) {
        return true;
    }
    
    QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const QFileInfo &entry : entries) {
        if (entry.isDir()) {
            if (!removeDir(entry.absoluteFilePath())) {
                return false;
            }
        } else {
            if (!QFile::remove(entry.absoluteFilePath())) {
                return false;
            }
        }
    }
    
    return dir.rmdir(dirPath);
} 