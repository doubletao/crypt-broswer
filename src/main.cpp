#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFileDialog>
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
    
    // 弹出文件选择对话框
    QString archivePath = QFileDialog::getOpenFileName(
        nullptr,
        "Select Encrypted Archive File",
        QDir::homePath(),
        "Archive Files (*.7z *.zip *.rar);;7z Files (*.7z);;All Files (*.*)"
    );
    
    // 检查用户是否取消了选择
    if (archivePath.isEmpty()) {
        return 0; // 用户取消，正常退出
    }
    
    // 检查文件是否存在
    if (!QFile::exists(archivePath)) {
        QMessageBox::critical(nullptr, "Error", QString("The selected file does not exist: %1").arg(archivePath));
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