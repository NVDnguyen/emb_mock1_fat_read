@echo off
SET OUTPUT_NAME=fat
SET BUILD_DIR=build
SET INCLUDE_DIR=include

@REM create build dir
mkdir "%BUILD_DIR%" 2>nul

@REM Compile  all *.c file in ---src--- folder
gcc -std=gnu99 -o "%OUTPUT_NAME%" src\*.c || (echo "> Compilation failed!" & exit /b)

@REM move to build folder  
move "%OUTPUT_NAME%.exe" "%BUILD_DIR%\%OUTPUT_NAME%.exe"


@echo =========== FAT FILE SYSTEM ===========
@REM run
"%BUILD_DIR%\%OUTPUT_NAME%.exe"

pause
