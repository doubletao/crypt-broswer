# Crypt Browser - 加密压缩包浏览器

## 项目介绍

Crypt Browser 是一个基于 Qt6 WebEngine 的加密压缩包浏览器，专门用于安全地浏览加密压缩包中的HTML/MHTML文件。

### 主要功能

1. **安全解压**: 支持密码保护的7z压缩包解压
2. **临时缓存**: 自动将文件解压到系统临时目录，程序退出时自动清理
3. **美观目录**: 自动生成带有现代化UI的目录页面
4. **多窗口浏览**: 每个文档都在独立的窗口中打开，支持多文档同时查看
5. **安全清理**: 程序退出时自动清理所有临时文件，保证数据安全

## 系统要求

- Windows 10 或更高版本
- Qt6 (推荐 Qt 6.5 或更高版本)
- CMake 3.16 或更高版本
- 7-Zip (7z.exe 需要在系统PATH中)
- MSVC 2019 或更高版本 (或其他兼容的C++17编译器)

## 安装依赖

### 1. 安装Qt6

从 [Qt官网](https://www.qt.io/download) 下载并安装Qt6，确保安装以下组件：
- Qt WebEngine
- Qt WebEngine Widgets
- Qt Core
- Qt Widgets

### 2. 安装7-Zip

从 [7-Zip官网](https://www.7-zip.org/) 下载并安装7-Zip，确保安装后 `7z.exe` 在系统PATH中可用。

### 3. 安装CMake

从 [CMake官网](https://cmake.org/download/) 下载并安装CMake。

## 编译构建

### 使用CMake构建

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目 (请根据实际情况调整Qt路径)
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.5.0/msvc2019_64"

# 编译项目
cmake --build . --config Release
```

### 使用Visual Studio

```bash
# 生成Visual Studio项目
cmake .. -G "Visual Studio 16 2019" -DCMAKE_PREFIX_PATH="C:/Qt/6.5.0/msvc2019_64"

# 然后用Visual Studio打开生成的.sln文件进行编译
```

## 压缩包准备

### 创建加密压缩包

假设您有一个包含HTML文件的目录 `files/`，使用以下命令创建加密压缩包：

```bash
# 创建自解压的加密压缩包
7z.exe a -sfx -pYourPassword archive.exe .\files\*

# 或者创建普通的加密压缩包
7z.exe a -pYourPassword archive.7z .\files\*
```

**注意**: 
- `-sfx` 参数创建自解压文件
- `-pYourPassword` 中的 `YourPassword` 替换为您的实际密码
- 支持的文件格式：`.html`, `.htm`, `.mhtml`, `.mht`

## 使用方法

### 1. 启动程序

```bash
# 通过命令行启动，指定压缩包路径
crypt-browser.exe "C:\path\to\your\archive.7z"
```

### 2. 输入密码

程序启动后会弹出密码输入对话框，输入正确的密码后开始解压。

### 3. 浏览文档

- 解压完成后会自动显示美观的目录页面
- 点击任意文件名即可在新窗口中打开对应的文档
- 每个文档都在独立的窗口中显示，支持前进、后退、刷新等操作
- 可以同时打开多个文档窗口

### 4. 安全退出

关闭程序时，所有临时文件会被自动清理，确保数据安全。

## 项目结构

```
crypt-browser/
├── CMakeLists.txt          # CMake配置文件
├── src/                    # 源代码目录
│   ├── main.cpp           # 主程序入口
│   ├── mainwindow.h       # 主窗口头文件
│   ├── mainwindow.cpp     # 主窗口实现
│   ├── webviewwindow.h    # 网页浏览窗口头文件
│   ├── webviewwindow.cpp  # 网页浏览窗口实现
│   ├── archivemanager.h   # 压缩包管理器头文件
│   └── archivemanager.cpp # 压缩包管理器实现
└── README.md              # 项目说明文档
```

## 特性说明

### 安全性
- 所有文件都解压到系统临时目录
- 程序退出时自动清理所有临时文件
- 支持密码保护的压缩包

### 用户体验
- 现代化的目录页面UI设计
- 响应式布局，适配不同屏幕尺寸
- 支持多文档同时浏览
- 直观的文件浏览界面

### 兼容性
- 支持HTML和MHTML格式文件
- 兼容7z压缩包格式
- 支持Windows平台

## 故障排除

### 常见问题

1. **无法启动7z.exe**
   - 确保7-Zip已正确安装
   - 检查7z.exe是否在系统PATH中

2. **密码错误**
   - 确认输入的密码正确
   - 检查压缩包是否损坏

3. **页面显示异常**
   - 确保HTML文件编码为UTF-8
   - 检查HTML文件是否完整

4. **编译错误**
   - 确保Qt6正确安装并配置
   - 检查CMake版本是否符合要求

## 开发说明

### 技术栈
- C++17
- Qt6 WebEngine
- CMake
- 7-Zip

### 核心组件
- `ArchiveManager`: 负责压缩包解压和临时文件管理
- `MainWindow`: 主窗口，显示目录页面
- `WebViewWindow`: 文档浏览窗口，支持多开
- `CustomWebEnginePage`: 自定义页面类，用于拦截链接点击

## 许可证

本项目采用MIT许可证。详情请查看LICENSE文件。

