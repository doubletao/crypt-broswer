#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>
#include "mainwindow.h"
#include "archivemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("Crypt Browser");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("CryptBrowser");
    
    // 检查命令行参数
    if (argc < 2) {
        QMessageBox::critical(nullptr, "错误", "请指定要打开的加密压缩包文件路径。\n用法: crypt-browser <压缩包路径>");
        return 1;
    }
    
    QString archivePath = QString::fromLocal8Bit(argv[1]);
    
    // 检查文件是否存在
    if (!QFile::exists(archivePath)) {
        QMessageBox::critical(nullptr, "错误", QString("指定的文件不存在: %1").arg(archivePath));
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