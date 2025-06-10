@echo off
chcp 65001
echo =================================
echo 7z 安全性对比演示
echo =================================
echo.

REM 设置7z路径
set SEVEN_ZIP="C:\Program Files (x86)\7-Zip\7z.exe"

REM 检查7z是否可用
if not exist %SEVEN_ZIP% (
    echo 错误：未找到 7z.exe
    pause
    exit /b 1
)

REM 检查test目录
if not exist test (
    echo 错误：未找到 test 目录
    pause
    exit /b 1
)

echo 创建两种不同安全级别的压缩包进行对比...
echo.

REM 创建普通加密压缩包（文件名可见）
echo 1. 创建普通加密压缩包（文件名可见）...
%SEVEN_ZIP% a -ptest123 normal_encrypted.7z .\test\*.html
echo.

REM 创建高安全性加密压缩包（文件名隐藏）
echo 2. 创建高安全性加密压缩包（文件名隐藏）...
%SEVEN_ZIP% a -ptest123 -mhe=on secure_encrypted.7z .\test\*.html
echo.

echo =================================
echo 安全性对比演示
echo =================================
echo.
echo 【普通加密压缩包】- 不输入密码也能看到文件列表：
echo.
%SEVEN_ZIP% l normal_encrypted.7z
echo.
echo =================================
echo.
echo 【高安全性加密压缩包】- 不输入密码无法查看任何信息：
echo.
%SEVEN_ZIP% l secure_encrypted.7z
echo.
echo =================================
echo 总结：
echo - normal_encrypted.7z : 文件名可见，安全性较低
echo - secure_encrypted.7z : 文件名隐藏，安全性高（推荐）
echo.
echo 两个文件都使用相同密码: test123
echo 推荐在项目中使用 secure_encrypted.7z 类型的压缩包
echo.
pause 