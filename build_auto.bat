@echo off
chcp 65001
echo =================================
echo Crypt Browser 智能编译脚本
echo =================================
echo.

REM 清理旧的构建目录
if exist build (
    echo 清理旧的构建目录...
    rmdir /s /q build
)

REM 创建构建目录
echo 创建构建目录...
mkdir build
cd build

REM 设置Qt路径
set QT_PATH=D:\Qt\5.15.2\msvc2019_64
echo 使用Qt路径: %QT_PATH%
echo.

REM 自动检测Visual Studio版本
echo 检测Visual Studio版本...

REM 检查VS 2022
if exist "D:\Program\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe" (
    echo 检测到: Visual Studio 2022 Community
    set VS_GENERATOR="Visual Studio 17 2022"
    goto :found_vs
)

if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\Common7\IDE\devenv.exe" (
    echo 检测到: Visual Studio 2022 Professional
    set VS_GENERATOR="Visual Studio 17 2022"
    goto :found_vs
)

if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\devenv.exe" (
    echo 检测到: Visual Studio 2022 Enterprise
    set VS_GENERATOR="Visual Studio 17 2022"
    goto :found_vs
)

REM 检查VS 2019
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\Common7\IDE\devenv.exe" (
    echo 检测到: Visual Studio 2019 Community
    set VS_GENERATOR="Visual Studio 16 2019"
    goto :found_vs
)

if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\Common7\IDE\devenv.exe" (
    echo 检测到: Visual Studio 2019 Professional
    set VS_GENERATOR="Visual Studio 16 2019"
    goto :found_vs
)

if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\devenv.exe" (
    echo 检测到: Visual Studio 2019 Enterprise
    set VS_GENERATOR="Visual Studio 16 2019"
    goto :found_vs
)

REM 未找到Visual Studio
echo 错误：未找到Visual Studio 2019或2022
echo 请确保已正确安装Visual Studio
echo.
echo 支持的版本：
echo - Visual Studio 2019 (Community/Professional/Enterprise)
echo - Visual Studio 2022 (Community/Professional/Enterprise)
pause
exit /b 1

:found_vs
echo 使用生成器: %VS_GENERATOR%
echo.

REM 配置CMake
echo 配置CMake项目...
cmake .. -DCMAKE_PREFIX_PATH="%QT_PATH%" -G %VS_GENERATOR%

if %ERRORLEVEL% NEQ 0 (
    echo CMake配置失败！请检查：
    echo 1. Qt路径是否正确: %QT_PATH%
    echo 2. CMake是否已安装
    echo 3. Visual Studio工具链是否完整
    pause
    exit /b 1
)

echo.
echo CMake配置成功！

REM 编译项目
echo 开始编译项目...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo 编译失败！请检查错误信息。
    pause
    exit /b 1
)

echo.
echo =================================
echo 编译成功！
echo 可执行文件位置: build\Release\crypt-browser.exe
echo =================================
echo.
echo 使用方法:
echo crypt-browser.exe "压缩包路径"
echo.
echo 快速测试:
echo 1. 运行: create_test_archive.bat
echo 2. 测试: build\Release\crypt-browser.exe test_documents.exe
echo.
pause 