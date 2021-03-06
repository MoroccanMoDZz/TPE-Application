@echo off

set SDK_VERSION=SDK11.16.0.PatchG

set SDK_DIR=%ProgramFiles%\TeliumSDK\%SDK_VERSION%
if not exist "%SDK_DIR%" set SDK_DIR=C:\%SDK_VERSION%
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

rem ===========================================================
rem
rem  generate English Messages
rem
rem ===========================================================

rem 1- convert h file to MSG file

"%TOOL_DIR%\BuildMSG" -c -tClessSample_msgEnglish.h -hMessagesDefinitions.h LANG.MSG


rem ===========================================================
rem
rem  generate French Messages (same command as previous without -C)
rem
rem ===========================================================

"%TOOL_DIR%\BuildMSG" -tClessSample_msgFrench.h -hMessagesDefinitions.h LANG.MSG

rem ===========================================================
rem
rem  sign LANG.MSG before loadind it in the terminal
rem
rem ===========================================================


rem 2- create LANG.c file to include in the sample
"%TOOL_DIR%\MSG2C" LANG.MSG ClessSample_lang.c

move /y MessagesDefinitions.h ..\Inc
move /y ClessSample_lang.c ..\Src

if not "%~1" == "/nopause" pause
