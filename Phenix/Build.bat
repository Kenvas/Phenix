@echo off

REM @call "D:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsMSBuildCmd.bat"
@call "D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsMSBuildCmd.bat"

if "%1"=="debug" (
    if "%2"==""    (goto debug_x64)
    if "%2"=="x64" (goto debug_x64)
    if "%2"=="x86" (goto debug_x86)
)

if "%1"=="release" (
    if "%2"==""    (goto release_x64)
    if "%2"=="x64" (goto release_x64)
    if "%2"=="x86" (goto release_x86)
)

if "%1"=="all" (goto all)

:debug_x64
    @msbuild ./Phenix.sln /p:Configuration="Debug";Platform="x64"
    goto eof

:debug_x86
    @msbuild ./Phenix.sln /p:Configuration="Debug";Platform="x86"
    goto eof

:release_x64
    @msbuild ./Phenix.sln /p:Configuration="Release";Platform="x64"
    goto eof

:release_x86
    @msbuild ./Phenix.sln /p:Configuration="Debug";Platform="x86"
    goto eof

:all
    @msbuild ./Phenix.sln /p:Configuration="Debug";Platform="x64"
    @msbuild ./Phenix.sln /p:Configuration="Debug";Platform="x86"
    @msbuild ./Phenix.sln /p:Configuration="Release";Platform="x64"
    @msbuild ./Phenix.sln /p:Configuration="Release";Platform="x86"
    goto eof

:eof

REM @call Boot.bat