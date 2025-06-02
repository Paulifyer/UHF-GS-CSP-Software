@echo off
set INCLUDE=
set LIB=
call "D:\Program Files (x86)\Microsoft Visual Studio\VC\Auxiliary\Build\vcvarsall.bat" x64
echo PATH=%PATH%
echo INCLUDE=%INCLUDE%
echo LIB=%LIB%;%LIBPATH%
