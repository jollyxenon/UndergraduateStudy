@echo off
setlocal

set "OUTPUT=submission.zip"

if not exist src\ (
    echo Please run this script from the project root containing src\, include\, and CMakeLists.txt.
    exit /b 1
)
if not exist include\ (
    echo Please run this script from the project root containing src\, include\, and CMakeLists.txt.
    exit /b 1
)
if not exist CMakeLists.txt (
    echo Please run this script from the project root containing src\, include\, and CMakeLists.txt.
    exit /b 1
)

if exist "%OUTPUT%" del /f /q "%OUTPUT%"

powershell -NoProfile -ExecutionPolicy Bypass -Command "Compress-Archive -Path 'src','include','CMakeLists.txt' -DestinationPath '%OUTPUT%' -Force"
if errorlevel 1 (
    echo Failed to create %OUTPUT%. Please make sure PowerShell is available.
    exit /b 1
)

echo Created %OUTPUT%
echo Please submit %OUTPUT% to OJ.

pause