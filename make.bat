:: Lucid Demo - Wizard Windows Makefile
:: alpha 0.1 (oct-2013)
@echo off
echo Lucid Demo:

set buildtype=release
set cmake=true

:parameters
if "%1" == "" goto main
if /I "%1" == "debug" set buildtype=debug
if /I "%1" == "release" set buildtype=release
if "%1" == "/C" set cmake=false
if "%1" == "/U" goto updateshaders
shift
goto parameters

:main
if "%cmake%" == "false" goto compile

:build
echo Making a %buildtype% build...
cmake . -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE:STRING=%buildtype%

:compile
echo Compiling and updating...
mingw32-make
if %ERRORLEVEL% gtr 0 goto fail
if not exist lucid-build\%buildtype%\shaders mkdir lucid-build\%buildtype%\shaders
xcopy libraries\*.dll lucid-build\%buildtype% /Y
:updateshaders
xcopy lucid\shaders\* lucid-build\%buildtype%\shaders /Y
xcopy assets\* lucid-build\%buildtype%\assets /Y
echo Building done.
goto end

:fail
echo Building failed.

:end