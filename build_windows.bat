@echo off
chcp 65001 >nul
g++ -std=c++17 -Wall -Wextra -Iinclude src\main.cpp src\MayBay.cpp src\KhachHang.cpp src\Ve.cpp src\ChuyenBay.cpp src\AirlineManager.cpp src\Utils.cpp -o PRO_CTDL_5.exe
if errorlevel 1 (
    echo Bien dich that bai. Kiem tra MSYS2 MinGW-w64 g++ trong PATH.
    pause
    exit /b 1
)
PRO_CTDL_5.exe
pause
