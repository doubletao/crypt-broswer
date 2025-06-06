#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QInputDialog>
#include <QList>
#include <QCloseEvent>
#include "archivemanager.h"
#include "webviewwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 设置压缩包路径
    bool setArchivePath(const QString &path);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onIndexPageLoadFinished(bool success);
    void onLinkClicked(const QUrl &url);
    void onWebViewWindowClosed();

private:
    void setupUI();
    void setupConnections();
    bool promptForPassword();
    QString generateIndexHtml();
    void createIndexPage();
    void cleanupWindows();

    // UI 组件
    QWebEngineView *m_webView;
    QWidget *m_centralWidget;
    QVBoxLayout *m_mainLayout;
    QLabel *m_statusLabel;

    // 业务逻辑
    ArchiveManager *m_archiveManager;
    QList<WebViewWindow*> m_webViewWindows;
    QString m_indexHtmlPath;
};

#endif // MAINWINDOW_H 