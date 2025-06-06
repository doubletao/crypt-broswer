#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QTextStream>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QCloseEvent>
#include <QStandardPaths>

// 自定义WebEnginePage类用于拦截链接点击
class CustomWebEnginePage : public QWebEnginePage
{
    Q_OBJECT

public:
    explicit CustomWebEnginePage(QWebEngineProfile *profile, QObject *parent = nullptr)
        : QWebEnginePage(profile, parent) {}

signals:
    void linkClicked(const QUrl &url);

protected:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override
    {
        if (type == QWebEnginePage::NavigationTypeLinkClicked && isMainFrame) {
            emit linkClicked(url);
            return false; // 阻止默认导航
        }
        return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_webView(nullptr)
    , m_centralWidget(nullptr)
    , m_mainLayout(nullptr)
    , m_statusLabel(nullptr)
    , m_archiveManager(nullptr)
{
    setupUI();
    setupConnections();
    
    // 设置窗口属性
    setWindowTitle("Crypt Browser - 目录");
    resize(800, 600);
    
    // 创建归档管理器
    m_archiveManager = new ArchiveManager(this);
}

MainWindow::~MainWindow()
{
    cleanupWindows();
}

bool MainWindow::setArchivePath(const QString &path)
{
    if (!m_archiveManager->setArchivePath(path)) {
        QMessageBox::critical(this, "错误", "无法设置压缩包路径");
        return false;
    }
    
    // 提示输入密码并解压
    if (!promptForPassword()) {
        return false;
    }
    
    // 创建目录页面
    createIndexPage();
    
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    cleanupWindows();
    
    // 清理临时文件
    if (m_archiveManager) {
        m_archiveManager->cleanup();
    }
    
    event->accept();
}

void MainWindow::setupUI()
{
    // 创建中央窗口部件
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    // 创建主布局
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);
    m_mainLayout->setSpacing(10);
    
    // 创建状态标签
    m_statusLabel = new QLabel("正在初始化...", this);
    m_statusLabel->setStyleSheet("font-size: 14px; color: #666; padding: 5px;");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    
    // 创建Web视图
    m_webView = new QWebEngineView(this);
    
    // 创建自定义页面来拦截链接点击
    CustomWebEnginePage *page = new CustomWebEnginePage(m_webView->page()->profile(), this);
    m_webView->setPage(page);
    
    // 添加到布局
    m_mainLayout->addWidget(m_statusLabel);
    m_mainLayout->addWidget(m_webView, 1);
}

void MainWindow::setupConnections()
{
    // 连接页面加载完成信号
    connect(m_webView, &QWebEngineView::loadFinished, this, &MainWindow::onIndexPageLoadFinished);
    
    // 连接自定义页面的链接点击信号
    CustomWebEnginePage *page = qobject_cast<CustomWebEnginePage*>(m_webView->page());
    if (page) {
        connect(page, &CustomWebEnginePage::linkClicked, this, &MainWindow::onLinkClicked);
    }
}

bool MainWindow::promptForPassword()
{
    bool ok;
    QString password = QInputDialog::getText(this, "输入密码", 
                                           "请输入压缩包密码:", 
                                           QLineEdit::Password, 
                                           QString(), &ok);
    
    if (!ok) {
        return false; // 用户取消
    }
    
    m_statusLabel->setText("正在解压文件...");
    QApplication::processEvents();
    
    if (!m_archiveManager->extractArchive(password)) {
        return false; // 解压失败，错误信息已在ArchiveManager中显示
    }
    
    m_statusLabel->setText("解压完成，正在生成目录...");
    return true;
}

QString MainWindow::generateIndexHtml()
{
    QStringList htmlFiles = m_archiveManager->getHtmlFiles();
    
    QString html = QString(
        "<!DOCTYPE html>\n"
        "<html lang=\"zh-CN\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>文档目录</title>\n"
        "    <style>\n"
        "        body {\n"
        "            font-family: 'Microsoft YaHei', Arial, sans-serif;\n"
        "            margin: 0;\n"
        "            padding: 20px;\n"
        "            background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%);\n"
        "            min-height: 100vh;\n"
        "        }\n"
        "        .container {\n"
        "            max-width: 1000px;\n"
        "            margin: 0 auto;\n"
        "            background: white;\n"
        "            border-radius: 10px;\n"
        "            box-shadow: 0 10px 30px rgba(0,0,0,0.3);\n"
        "            overflow: hidden;\n"
        "        }\n"
        "        .header {\n"
        "            background: linear-gradient(90deg, #4CAF50, #45a049);\n"
        "            color: white;\n"
        "            padding: 30px;\n"
        "            text-align: center;\n"
        "        }\n"
        "        .header h1 {\n"
        "            margin: 0;\n"
        "            font-size: 2.2em;\n"
        "            font-weight: 300;\n"
        "        }\n"
        "        .header p {\n"
        "            margin: 10px 0 0 0;\n"
        "            opacity: 0.9;\n"
        "            font-size: 1.1em;\n"
        "        }\n"
        "        .content {\n"
        "            padding: 30px;\n"
        "        }\n"
        "        .file-list {\n"
        "            list-style: none;\n"
        "            padding: 0;\n"
        "            margin: 0;\n"
        "        }\n"
        "        .file-item {\n"
        "            margin: 10px 0;\n"
        "            border-radius: 8px;\n"
        "            overflow: hidden;\n"
        "            box-shadow: 0 2px 10px rgba(0,0,0,0.1);\n"
        "            transition: transform 0.2s, box-shadow 0.2s;\n"
        "        }\n"
        "        .file-item:hover {\n"
        "            transform: translateY(-2px);\n"
        "            box-shadow: 0 5px 20px rgba(0,0,0,0.2);\n"
        "        }\n"
        "        .file-link {\n"
        "            display: block;\n"
        "            padding: 20px 25px;\n"
        "            text-decoration: none;\n"
        "            color: #333;\n"
        "            background: #f8f9fa;\n"
        "            border-left: 4px solid #4CAF50;\n"
        "            transition: all 0.2s;\n"
        "        }\n"
        "        .file-link:hover {\n"
        "            background: #e9ecef;\n"
        "            border-left-color: #45a049;\n"
        "        }\n"
        "        .file-name {\n"
        "            font-size: 1.1em;\n"
        "            font-weight: 500;\n"
        "            margin-bottom: 5px;\n"
        "        }\n"
        "        .file-path {\n"
        "            font-size: 0.9em;\n"
        "            color: #666;\n"
        "            opacity: 0.8;\n"
        "        }\n"
        "        .empty-message {\n"
        "            text-align: center;\n"
        "            color: #666;\n"
        "            font-size: 1.2em;\n"
        "            padding: 40px;\n"
        "        }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <div class=\"container\">\n"
        "        <div class=\"header\">\n"
        "            <h1>📚 文档目录</h1>\n"
        "            <p>共找到 %1 个文档文件</p>\n"
        "        </div>\n"
        "        <div class=\"content\">\n"
    ).arg(htmlFiles.size());
    
    if (htmlFiles.isEmpty()) {
        html += "            <div class=\"empty-message\">未找到HTML或MHTML文件</div>\n";
    } else {
        html += "            <ul class=\"file-list\">\n";
        
        for (const QString &filePath : htmlFiles) {
            QFileInfo fileInfo(filePath);
            QString fileName = fileInfo.fileName();
            QString relativePath = QDir(m_archiveManager->getTempDir()).relativeFilePath(filePath);
            
            html += QString(
                "                <li class=\"file-item\">\n"
                "                    <a href=\"file:///%1\" class=\"file-link\">\n"
                "                        <div class=\"file-name\">%2</div>\n"
                "                        <div class=\"file-path\">%3</div>\n"
                "                    </a>\n"
                "                </li>\n"
            ).arg(filePath).arg(fileName).arg(relativePath);
        }
        
        html += "            </ul>\n";
    }
    
    html += 
        "        </div>\n"
        "    </div>\n"
        "</body>\n"
        "</html>";
    
    return html;
}

void MainWindow::createIndexPage()
{
    // 生成目录HTML
    QString htmlContent = generateIndexHtml();
    
    // 保存到临时文件
    QString tempDir = m_archiveManager->getTempDir();
    m_indexHtmlPath = QDir(tempDir).absoluteFilePath("index.html");
    
    QFile indexFile(m_indexHtmlPath);
    if (indexFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&indexFile);
        out.setCodec("UTF-8");
        out << htmlContent;
        indexFile.close();
        
        // 加载目录页面
        QUrl indexUrl = QUrl::fromLocalFile(m_indexHtmlPath);
        m_webView->load(indexUrl);
        
        m_statusLabel->setText("目录页面已生成");
    } else {
        QMessageBox::critical(this, "错误", "无法创建目录页面文件");
    }
}

void MainWindow::onIndexPageLoadFinished(bool success)
{
    if (success) {
        m_statusLabel->setText("就绪 - 点击文件名打开文档");
    } else {
        m_statusLabel->setText("目录页面加载失败");
    }
}

void MainWindow::onLinkClicked(const QUrl &url)
{
    if (url.isLocalFile()) {
        QString filePath = url.toLocalFile();
        
        // 检查文件是否存在
        if (QFile::exists(filePath)) {
            // 创建新的WebView窗口
            WebViewWindow *webViewWindow = new WebViewWindow(filePath, nullptr);
            m_webViewWindows.append(webViewWindow);
            
            // 连接窗口关闭信号
            connect(webViewWindow, &WebViewWindow::destroyed, this, &MainWindow::onWebViewWindowClosed);
            
            // 显示窗口
            webViewWindow->show();
            webViewWindow->raise();
            webViewWindow->activateWindow();
        } else {
            QMessageBox::warning(this, "警告", QString("文件不存在: %1").arg(filePath));
        }
    }
}

void MainWindow::onWebViewWindowClosed()
{
    // 从列表中移除已关闭的窗口
    WebViewWindow *closedWindow = qobject_cast<WebViewWindow*>(sender());
    if (closedWindow) {
        m_webViewWindows.removeAll(closedWindow);
    }
}

void MainWindow::cleanupWindows()
{
    // 关闭所有打开的WebView窗口
    for (WebViewWindow *window : m_webViewWindows) {
        if (window) {
            window->close();
            window->deleteLater();
        }
    }
    m_webViewWindows.clear();
}

#include "mainwindow.moc" 