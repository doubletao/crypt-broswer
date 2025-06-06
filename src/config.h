#ifndef CONFIG_H
#define CONFIG_H

// 7-Zip 程序路径配置
// 如果您的7-Zip安装在不同路径，请修改此处
#ifdef Q_OS_WIN
    #define SEVEN_ZIP_PATH "C:/Program Files (x86)/7-Zip/7z.exe"
#else
    #define SEVEN_ZIP_PATH "7z"  // Linux/Mac 使用系统PATH中的7z
#endif

// 应用程序配置
#define APP_NAME "Crypt Browser"
#define APP_VERSION "1.0.0"
#define APP_ORGANIZATION "CryptBrowser"

#endif // CONFIG_H 