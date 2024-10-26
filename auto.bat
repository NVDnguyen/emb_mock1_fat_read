@echo off
SET OUTPUT_NAME=fat
SET BUILD_DIR=build
SET INCLUDE_DIR=include

mkdir "%BUILD_DIR%" 2>nul

gcc -std=gnu99 -o "%OUTPUT_NAME%" src\*.c || (echo "> Compilation failed!" & exit /b)

move "%OUTPUT_NAME%.exe" "%BUILD_DIR%\%OUTPUT_NAME%.exe"


"%BUILD_DIR%\%OUTPUT_NAME%.exe"

pause
