@echo off
setlocal enabledelayedexpansion

set SRC_DIR=src
set BUILD_DIR=build\windows
set INCLUDE_DIR=include
set EXAMPLES_DIR=examples
set TESTS_DIR=tests

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: کامپایل کتابخانه
cl /nologo /O2 /D_CRT_SECURE_NO_WARNINGS /I%INCLUDE_DIR% ^
   %SRC_DIR%\*.c ^
   /c /Fd"%BUILD_DIR%\omnicontrol.pdb" /Fo"%BUILD_DIR%\"

lib /nologo "%BUILD_DIR%\*.obj" /OUT:"%BUILD_DIR%\omnicontrol.lib" /MACHINE:X64

:: کامپایل مثال‌ها
cl /nologo /O2 /D_CRT_SECURE_NO_WARNINGS /I%INCLUDE_DIR% ^
   "%EXAMPLES_DIR%\system_monitor.c" ^
   "%BUILD_DIR%\omnicontrol.lib" ^
   /link /OUT:"%BUILD_DIR%\system_monitor.exe"

:: کامپایل تست‌ها
cl /nologo /O2 /D_CRT_SECURE_NO_WARNINGS /I%INCLUDE_DIR% ^
   "%TESTS_DIR%\unit_tests.c" ^
   "%BUILD_DIR%\omnicontrol.lib" ^
   /link /OUT:"%BUILD_DIR%\unit_tests.exe"

echo Build completed. Output: %BUILD_DIR%\*.exe
endlocal