@echo off
setlocal
set PATH_NSIS=C:\Program^ Files^ (x86)\NSIS
set PATH_7Z=C:\Program^ Files\7-Zip
set TITLE_LOWER=wow-console-runner
set TITLE_UPPER=WoW-Console-Runner
set DIR_RELEASE="Desktop Qt 5.15.0 MSVC2019 32bit release"\release
@echo on

@rem update .exe file
copy ..\%TITLE_LOWER%\%DIR_RELEASE%\%TITLE_LOWER%.exe %TITLE_LOWER%-win32\%TITLE_LOWER%.exe

@rem Win32 installer
@echo off
cd %TITLE_LOWER%-win32
@echo on
"%PATH_NSIS%\makensis" /nocd /v3 %TITLE_LOWER%-win32.nsi
del ..\%TITLE_UPPER%-win32.exe
move %TITLE_UPPER%-win32.exe ..
@echo off
cd..
@echo on

@rem Win32 zip file
del %TITLE_UPPER%-win32.zip
"%PATH_7Z%\7z" a -mx9 -mmt -tzip -x!%TITLE_LOWER%-win32.nsi -r %TITLE_UPPER%-win32.zip %TITLE_LOWER%-win32

@rem Win32 7z file (LZMA)
del %TITLE_UPPER%-win32.7z
"%PATH_7Z%\7z" a -mx9 -mmt -t7z -x!%TITLE_LOWER%-win32.nsi -r %TITLE_UPPER%-win32.7z %TITLE_LOWER%-win32
