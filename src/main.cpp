#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>
#include "mainwindow.h"
#include "archivemanager.h"
#include "config.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);
    app.setOrganizationName(APP_ORGANIZATION);
    
    // 检查命令行参数
    if (argc < 2) {
        QMessageBox::critical(nullptr, "Error", "Please specify the encrypted archive file path.\nUsage: crypt-browser <archive path>");
        return 1;
    }
    
    QString archivePath = QString::fromLocal8Bit(argv[1]);
    
    // 检查文件是否存在
    if (!QFile::exists(archivePath)) {
        QMessageBox::critical(nullptr, "Error", QString("The specified file does not exist: %1").arg(archivePath));
        return 1;
    }
    
    // 创建主窗口
    MainWindow window;
    
    // 设置压缩包路径并初始化
    if (!window.setArchivePath(archivePath)) {
        return 1;
    }
    
    window.show();
    
    return app.exec();
} 