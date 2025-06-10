@echo off
chcp 65001
echo =================================
echo Crypt Browser Simple Build Script
echo =================================
echo.

REM Clear old build
if exist build (
    echo Cleaning old build directory...
    rmdir /s /q build
)

REM Create build directory
echo Creating build directory...
mkdir build
cd build

REM Set Qt path
set QT_PATH=D:\Qt\5.15.2\msvc2019_64
echo Using Qt path: %QT_PATH%
echo.

REM Configure with UTF-8 support
echo Configuring CMake project with UTF-8 support...
cmake .. -DCMAKE_PREFIX_PATH="%QT_PATH%" -G "Visual Studio 17 2022" -DCMAKE_CXX_FLAGS="/utf-8"

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo CMake configuration successful!

REM Build project
echo Building project...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo =================================
echo Build successful! Deploying Qt dependencies...
echo =================================

REM Deploy Qt dependencies
echo Deploying Qt dependencies to output directory...
set WINDEPLOYQT_PATH=%QT_PATH%\bin\windeployqt.exe

if not exist "%WINDEPLOYQT_PATH%" (
    echo Warning: windeployqt.exe not found
    echo Path: %WINDEPLOYQT_PATH%
    echo Please manually copy Qt runtime files
    goto :skip_deploy
)

"%WINDEPLOYQT_PATH%" --release --compiler-runtime --webenginewidgets --no-translations --no-system-d3d-compiler --no-opengl-sw Release\crypt-browser.exe

if %ERRORLEVEL% NEQ 0 (
    echo Warning: Qt deployment had issues, but the program may still work
)

echo Qt dependencies deployed successfully!

:skip_deploy

REM Generate config file
echo Generating config file...
(
echo [Paths]
echo SevenZip=C:/Program Files ^(x86^)/7-Zip/7z.exe
echo.
echo [App]
echo Name=Crypt Browser
echo Version=1.0.0
echo Organization=CryptBrowser
) > Release\config.ini

echo Config file generated: Release\config.ini
echo.
echo =================================
echo Build and deployment complete!
echo Executable: build\Release\crypt-browser.exe
echo =================================
echo.
echo Usage:
echo 1. Launch: build\Release\crypt-browser.exe
echo 2. Select archive file in the dialog
echo.
echo Note: Output directory contains all necessary Qt runtime files
pause 