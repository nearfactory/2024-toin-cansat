@echo off

rem Relative Dir from CMD file to MWSDK root dir.
SET REL_MWSDK=..\MWSDK
rem Relative Dir from MWSDK root dir to Tools dir.
SET REL_TOOLS=..\Tools

rem Get DIR where this CMD file is.
cd /D %~dp0%REL_MWSDK%
SET WD=%CD%\

SET WDS=%WD:\=/%

SET PATH=
SET PATH=%WD%%REL_TOOLS%\ba-elf-ba2-r36379\ba-elf\bin;%PATH%
SET PATH=%WD%%REL_TOOLS%\ba-elf-ba2-r36379\bin;%PATH%
SET PATH=%WD%%REL_TOOLS%\MinGW\bin;%WD%%REL_TOOLS%\MinGW\msys\1.0\bin;%PATH%

SET MWSDK=1
SET MWSDK_ROOT=%WDS%
SET HOME=%WD%

start cmd.exe
GOTO _End

:_Err
echo PLEASE RUN ON MWSDK TOP DIRECTORY...
PAUSE
GOTO _End

:_End
