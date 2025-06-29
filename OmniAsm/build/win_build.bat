@echo off
setlocal enabledelayedexpansion

set SRC_DIR=src\windows
set BUILD_DIR=build\windows
set INCLUDE_DIR=include
set EXAMPLES_DIR=examples

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: کامپایل اسمبلی
ml64 /nologo /c "%SRC_DIR%\omniasm.asm" /Fo"%BUILD_DIR%\omniasm.obj"

:: کامپایل مثال
cl /nologo /O2 /D_CRT_SECURE_NO_WARNINGS /I"%INCLUDE_DIR%" ^
   "%EXAMPLES_DIR%\test_perf.c" ^
   "%BUILD_DIR%\omniasm.obj" ^
   /link /OUT:"%BUILD_DIR%\test_perf.exe"

:: کپی فایل‌های نهایی
copy "%BUILD_DIR%\test_perf.exe" "%BUILD_DIR%\omni_test.exe" >nul

echo Build completed. Output: %BUILD_DIR%\omni_test.exe
endlocal