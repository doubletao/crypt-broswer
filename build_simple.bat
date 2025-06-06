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
echo Build successful!
echo Executable: build\Release\crypt-browser.exe
echo =================================
echo.
pause 