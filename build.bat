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
echo ����ɹ���
echo ��ִ���ļ�λ��: build\Release\crypt-browser.exe
echo =================================
echo.
echo ʹ�÷���:
echo crypt-browser.exe "ѹ����·��"
echo.
pause 