@echo off
echo =================================
echo 创建测试用加密压缩包
echo =================================
echo.

REM 设置7z路径
set SEVEN_ZIP="C:\Program Files (x86)\7-Zip\7z.exe"

REM 检查7z是否可用
if not exist %SEVEN_ZIP% (
    echo 错误：未找到 7z.exe 在路径 %SEVEN_ZIP%
    echo 请确保已安装 7-Zip 到此路径，或修改批处理文件中的路径
    echo 下载地址：https://www.7-zip.org/
    pause
    exit /b 1
)

REM 检查test目录是否存在
if not exist test (
    echo 错误：未找到 test 目录
    echo 请确保在项目根目录中运行此脚本
    pause
    exit /b 1
)

echo 找到以下测试文件：
dir test\*.html /b
echo.

REM 提示用户输入密码
set /p PASSWORD="请输入压缩包密码 (默认: test123): "
if "%PASSWORD%"=="" set PASSWORD=test123
echo.

REM 创建压缩包
echo 正在创建加密压缩包...
echo 密码: %PASSWORD%
echo.

%SEVEN_ZIP% a -sfx -p%PASSWORD% -mx9 test_documents.exe .\test\*.html

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo 压缩包创建失败！
    pause
    exit /b 1
)

echo.
echo =================================
echo 压缩包创建成功！
echo =================================
echo.
echo 文件名: test_documents.exe
echo 密码: %PASSWORD%
echo 包含文件:
%SEVEN_ZIP% l test_documents.exe
echo.
echo 测试方法:
echo 1. 编译项目: build.bat
echo 2. 运行程序: build\Release\crypt-browser.exe test_documents.exe
echo 3. 输入密码: %PASSWORD%
echo.
pause 