@echo off

set SDK_VERSION=SDK11.14.0.08

set SDK_DIR=%ProgramFiles%\TeliumSDK\%SDK_VERSION%
if not exist "%SDK_DIR%" set SDK_DIR=C:\Program Files\TeliumSDK\%SDK_VERSION%
if not exist "%SDK_DIR%" (
	echo SDK not found !
	echo %SDK_DIR%
	pause
	goto :eof
)
set TOOL_DIR=%SDK_DIR%\Tools\FW_TPLUS\MSG tool
if not exist "%TOOL_DIR%" set TOOL_DIR=%SDK_DIR%\tools\MSG tool
if not exist "%TOOL_DIR%" (
	echo MSG tool in SDK not found !
	echo %SDK_DIR%
	pause
	goto :eof
)

if not "%~1"=="/check" goto :build
if not exist "%~dp0..\Inc\MessagesDefinitions.h" goto :build
if not exist "%~dp0..\Src\cu_lang.c" goto :build
if not exist "%~dp0..\Src\Messages\LANG.MSG" goto :build
goto :eof

:build
call "%~dp0cleanMessages.bat"

pushd "%~dp0"

rem ===========================================================
rem  Generate English Messages
rem ===========================================================
"%TOOL_DIR%\BuildMSG" -c -t..\Src\Messages\cu_msgEnglish.h -h..\Inc\MessagesDefinitions.h LANG.MSG


rem ===========================================================
rem  Generate French Messages (same command as previous without -C)
rem ===========================================================
"%TOOL_DIR%\BuildMSG" -t..\Src\Messages\cu_msgFrench.h -h..\Inc\MessagesDefinitions.h LANG.MSG
move /y LANG.MSG ..\Src\Messages\LANG.MSG > nul 2> nul

rem ===========================================================
rem  Create cu_lang.c file
rem ===========================================================
"%TOOL_DIR%\MSG2C" ..\Src\Messages\LANG.MSG ..\Src\cu_lang.c

popd

if "%~1"=="/nopause" goto :eof
if "%~2"=="/nopause" goto :eof
pause
