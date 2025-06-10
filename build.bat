@echo off
echo =================================
echo Crypt Browser ����ű�
echo =================================
echo.

REM ����Ƿ����buildĿ¼�����������ɾ��
if exist build (
    echo �����ɵĹ���Ŀ¼...
    rmdir /s /q build
)

REM ��������Ŀ¼
echo ��������Ŀ¼...
mkdir build
cd build

REM ����Qt·�� (���������Qt��װ·���޸�)
set QT_PATH=D:\Qt\5.15.2\msvc2019_64
echo ʹ��Qt·��: %QT_PATH%
echo.

REM ����CMake
echo ����CMake��Ŀ...
cmake .. -DCMAKE_PREFIX_PATH="%QT_PATH%" -G "Visual Studio 17 2022"

if %ERRORLEVEL% NEQ 0 (
    echo CMake����ʧ�ܣ����飺
    echo 1. Qt·���Ƿ���ȷ
    echo 2. CMake�Ƿ��Ѱ�װ
    echo 3. Visual Studio�Ƿ��Ѱ�װ
    pause
    exit /b 1
)

echo.
echo CMake���óɹ���

REM ������Ŀ
echo ��ʼ������Ŀ...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo ����ʧ�ܣ����������Ϣ��
    pause
    exit /b 1
)

echo.
echo =================================
echo 编译成功！开始部署Qt依赖文件...
echo =================================

REM 部署Qt依赖文件
echo 正在部署Qt依赖文件到输出目录...
set WINDEPLOYQT_PATH=%QT_PATH%\bin\windeployqt.exe

if not exist "%WINDEPLOYQT_PATH%" (
    echo 警告：未找到windeployqt.exe工具
    echo 路径：%WINDEPLOYQT_PATH%
    echo 请手动复制Qt运行时文件
    goto :skip_deploy
)

"%WINDEPLOYQT_PATH%" --release --compiler-runtime --webenginewidgets --no-translations --no-system-d3d-compiler --no-opengl-sw Release\crypt-browser.exe

if %ERRORLEVEL% NEQ 0 (
    echo 警告：Qt依赖部署过程中出现问题，但程序可能仍然可用
)

echo Qt依赖文件部署完成！

:skip_deploy
echo.
echo =================================
echo 构建和部署完成！
echo 可执行文件位置: build\Release\crypt-browser.exe
echo =================================
echo.
echo 使用方法:
echo 1. 直接启动: build\Release\crypt-browser.exe
echo 2. 在文件选择对话框中选择压缩包
echo.
echo 注意：输出目录已包含所有必要的Qt运行时文件，可以独立运行
pause 