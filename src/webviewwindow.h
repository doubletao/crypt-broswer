#ifndef WEBVIEWWINDOW_H
#define WEBVIEWWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>

class WebViewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WebViewWindow(const QString &filePath, QWidget *parent = nullptr);
    ~WebViewWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onLoadStarted();
    void onLoadProgress(int progress);
    void onLoadFinished(bool success);
    void onBackButtonClicked();
    void onForwardButtonClicked();
    void onRefreshButtonClicked();

private:
    void setupUI();
    void setupConnections();

    QWebEngineView *m_webView;
    QWidget *m_centralWidget;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_toolbarLayout;
    
    QPushButton *m_backButton;
    QPushButton *m_forwardButton;
    QPushButton *m_refreshButton;
    QLabel *m_titleLabel;
    QProgressBar *m_progressBar;
    
    QString m_filePath;
};

#endif // WEBVIEWWINDOW_H 