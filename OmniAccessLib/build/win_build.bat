// win_build.bat - Placeholder content
@echo off
setlocal enabledelayedexpansion

:: Configuration
set BUILD_DIR=build\windows
set SRC_DIR=src
set INCLUDE_DIR=include
set DRIVER_DIR=drivers\windows
set EXAMPLES_DIR=examples
set DRIVER_NAME=OmniAccessDriver

:: Clean and create build directory
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
mkdir "%BUILD_DIR%"
mkdir "%BUILD_DIR%\driver"
mkdir "%BUILD_DIR%\lib"
mkdir "%BUILD_DIR%\examples"

:: Compile the library
cl /nologo /O2 /D_CRT_SECURE_NO_WARNINGS /D_WINDOWS /I%INCLUDE_DIR% /I%INCLUDE_DIR%\windows ^
   %SRC_DIR%\core.c %SRC_DIR%\windows\*.c %SRC_DIR%\common\*.c ^
   /c /Fd"%BUILD_DIR%\lib\omniaccess.pdb" /Fo"%BUILD_DIR%\lib\"

lib /nologo "%BUILD_DIR%\lib\*.obj" /OUT:"%BUILD_DIR%\lib\omniaccess.lib" /MACHINE:X64

:: Compile the driver
set DRIVER_SRCS=%DRIVER_DIR%\driver.c %DRIVER_DIR%\memory.c %DRIVER_DIR%\io.c
cl /nologo /O2 /D_AMD64_ /D_WIN64 /D_WINDOWS /D_NDEBUG /kernel ^
   /I%INCLUDE_DIR% /I%INCLUDE_DIR%\windows ^
   %DRIVER_SRCS% ^
   /c /Fd"%BUILD_DIR%\driver\%DRIVER_NAME%.pdb" /Fo"%BUILD_DIR%\driver\"

link /nologo /DRIVER /MACHINE:X64 /SUBSYSTEM:NATIVE /RELEASE ^
     /OUT:"%BUILD_DIR%\%DRIVER_NAME%.sys" /PDB:"%BUILD_DIR%\%DRIVER_NAME%.pdb" ^
     "%BUILD_DIR%\driver\*.obj"

:: Build the installer
cl /nologo /O2 /D_CRT_SECURE_NO_WARNINGS %DRIVER_DIR%\installer.c ^
   /link /OUT:"%BUILD_DIR%\install_%DRIVER_NAME%.exe"

:: Build examples
cl /nologo /O2 /D_CRT_SECURE_NO_WARNINGS /I%INCLUDE_DIR% ^
   %EXAMPLES_DIR%\physical_read.c ^
   /link "%BUILD_DIR%\lib\omniaccess.lib" /OUT:"%BUILD_DIR%\examples\physical_read.exe"

cl /nologo /O2 /D_CRT_SECURE_NO_WARNINGS /I%INCLUDE_DIR% ^
   %EXAMPLES_DIR%\pci_scan.c ^
   /link "%BUILD_DIR%\lib\omniaccess.lib" /OUT:"%BUILD_DIR%\examples\pci_scan.exe"

cl /nologo /O2 /D_CRT_SECURE_NO_WARNINGS /I%INCLUDE_DIR% ^
   %EXAMPLES_DIR%\msr_dump.c ^
   /link "%BUILD_DIR%\lib\omniaccess.lib" /OUT:"%BUILD_DIR%\examples\msr_dump.exe"

:: Copy INF and catalog files
copy /Y %DRIVER_DIR%\inf\OmniAccessDriver.inf "%BUILD_DIR%\" > nul
copy /Y %DRIVER_DIR%\inf\OmniAccessDriver.cat "%BUILD_DIR%\" > nul

:: Create batch file to install driver
echo @echo off > "%BUILD_DIR%\install_driver.bat"
echo setlocal enabledelayedexpansion >> "%BUILD_DIR%\install_driver.bat"
echo sc create OmniAccessDriver binPath= "%%~dp0%DRIVER_NAME%.sys" type= kernel start= demand >> "%BUILD_DIR%\install_driver.bat"
echo sc start OmniAccessDriver >> "%BUILD_DIR%\install_driver.bat"
echo echo Driver installed and started successfully >> "%BUILD_DIR%\install_driver.bat"

echo.
echo ====================================================
echo Build completed successfully in %BUILD_DIR% directory
echo ====================================================
echo.
echo Contents:
echo   %DRIVER_NAME%.sys        - Kernel driver
echo   install_%DRIVER_NAME%.exe - Driver installer
echo   omniaccess.lib           - Library for applications
echo   examples\                - Sample applications
echo   install_driver.bat       - Manual installation script
echo ====================================================

endlocal