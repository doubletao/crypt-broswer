#include "webviewwindow.h"
#include <QCloseEvent>
#include <QFileInfo>
#include <QUrl>
#include <QMessageBox>
#include <QApplication>
#include <QIcon>

WebViewWindow::WebViewWindow(const QString &filePath, QWidget *parent)
    : QMainWindow(parent)
    , m_webView(nullptr)
    , m_centralWidget(nullptr)
    , m_mainLayout(nullptr)
    , m_toolbarLayout(nullptr)
    , m_backButton(nullptr)
    , m_forwardButton(nullptr)
    , m_refreshButton(nullptr)
    , m_titleLabel(nullptr)
    , m_progressBar(nullptr)
    , m_filePath(filePath)
{
    setupUI();
    setupConnections();
    
    // 设置窗口标题
    QFileInfo fileInfo(filePath);
    setWindowTitle(QString("Crypt Browser - %1").arg(fileInfo.fileName()));
    
    // 设置窗口大小
    resize(1024, 768);
    
    // 加载文件
    QUrl url = QUrl::fromLocalFile(filePath);
    m_webView->load(url);
}

WebViewWindow::~WebViewWindow()
{
}

void WebViewWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void WebViewWindow::setupUI()
{
    // 创建中央窗口部件
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    // 创建主布局
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    m_mainLayout->setContentsMargins(5, 5, 5, 5);
    m_mainLayout->setSpacing(5);
    
    // 创建工具栏布局
    m_toolbarLayout = new QHBoxLayout();
    m_toolbarLayout->setSpacing(5);
    
    // 创建导航按钮
    m_backButton = new QPushButton("←", this);
    m_backButton->setToolTip("后退");
    m_backButton->setFixedSize(30, 30);
    m_backButton->setEnabled(false);
    
    m_forwardButton = new QPushButton("→", this);
    m_forwardButton->setToolTip("前进");
    m_forwardButton->setFixedSize(30, 30);
    m_forwardButton->setEnabled(false);
    
    m_refreshButton = new QPushButton("⟳", this);
    m_refreshButton->setToolTip("刷新");
    m_refreshButton->setFixedSize(30, 30);
    
    // 创建标题标签
    m_titleLabel = new QLabel("加载中...", this);
    m_titleLabel->setStyleSheet("font-weight: bold; color: #333;");
    
    // 创建进度条
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    m_progressBar->setMaximumHeight(5);
    
    // 添加到工具栏布局
    m_toolbarLayout->addWidget(m_backButton);
    m_toolbarLayout->addWidget(m_forwardButton);
    m_toolbarLayout->addWidget(m_refreshButton);
    m_toolbarLayout->addWidget(m_titleLabel);
    m_toolbarLayout->addStretch();
    
    // 创建Web视图
    m_webView = new QWebEngineView(this);
    
    // 添加到主布局
    m_mainLayout->addLayout(m_toolbarLayout);
    m_mainLayout->addWidget(m_progressBar);
    m_mainLayout->addWidget(m_webView, 1);
}

void WebViewWindow::setupConnections()
{
    // 连接导航按钮信号
    connect(m_backButton, &QPushButton::clicked, this, &WebViewWindow::onBackButtonClicked);
    connect(m_forwardButton, &QPushButton::clicked, this, &WebViewWindow::onForwardButtonClicked);
    connect(m_refreshButton, &QPushButton::clicked, this, &WebViewWindow::onRefreshButtonClicked);
    
    // 连接Web视图信号
    connect(m_webView, &QWebEngineView::loadStarted, this, &WebViewWindow::onLoadStarted);
    connect(m_webView, &QWebEngineView::loadProgress, this, &WebViewWindow::onLoadProgress);
    connect(m_webView, &QWebEngineView::loadFinished, this, &WebViewWindow::onLoadFinished);
    
    // 连接标题变化信号
    connect(m_webView, &QWebEngineView::titleChanged, [this](const QString &title) {
        if (!title.isEmpty()) {
            m_titleLabel->setText(title);
        }
    });
    
    // 连接URL变化信号，更新导航按钮状态
    connect(m_webView, &QWebEngineView::urlChanged, [this]() {
        m_backButton->setEnabled(m_webView->history()->canGoBack());
        m_forwardButton->setEnabled(m_webView->history()->canGoForward());
    });
}

void WebViewWindow::onLoadStarted()
{
    m_progressBar->setVisible(true);
    m_progressBar->setValue(0);
    m_titleLabel->setText("加载中...");
}

void WebViewWindow::onLoadProgress(int progress)
{
    m_progressBar->setValue(progress);
}

void WebViewWindow::onLoadFinished(bool success)
{
    m_progressBar->setVisible(false);
    
    if (!success) {
        m_titleLabel->setText("加载失败");
        QMessageBox::warning(this, "警告", "页面加载失败");
    } else {
        // 如果没有标题，使用文件名
        if (m_webView->title().isEmpty()) {
            QFileInfo fileInfo(m_filePath);
            m_titleLabel->setText(fileInfo.fileName());
        }
    }
    
    // 更新导航按钮状态
    m_backButton->setEnabled(m_webView->history()->canGoBack());
    m_forwardButton->setEnabled(m_webView->history()->canGoForward());
}

void WebViewWindow::onBackButtonClicked()
{
    if (m_webView->history()->canGoBack()) {
        m_webView->back();
    }
}

void WebViewWindow::onForwardButtonClicked()
{
    if (m_webView->history()->canGoForward()) {
        m_webView->forward();
    }
}

void WebViewWindow::onRefreshButtonClicked()
{
    m_webView->reload();
} 