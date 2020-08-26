$PATH_NSIS = "C:/Program Files (x86)/NSIS"
$PATH_7Z = "C:/Program Files/7-Zip"
$TITLE_LOWER = "wow-console-runner"
$TITLE_UPPER = "WoW-Console-Runner"
$DIR_RELEASE = "Desktop Qt 5.15.0 MSVC2019 32bit release/release"

# Update to latest release build
Copy-Item "../$TITLE_LOWER/$DIR_RELEASE/$TITLE_LOWER.exe" -Destination "$TITLE_LOWER-win32/$TITLE_LOWER.exe"

# Win32 .exe installer
Set-Location "$TITLE_LOWER-win32"
&"$PATH_NSIS/makensis.exe" /nocd /v3 installer-win32.nsi
Remove-Item "../$TITLE_UPPER-win32.exe"
Move-Item "$TITLE_UPPER-win32.exe" -Destination "../$TITLE_UPPER-win32.exe"
Set-Location ".."

# Win32 .zip file
Remove-Item "$TITLE_UPPER-win32.zip"
&"$PATH_7Z/7z.exe" a -tzip -mmt -mx9 -r "$TITLE_UPPER-win32.zip" "$TITLE_LOWER-win32/" `
	-x!"installer-win32.nsi" -x!"install.ico" -x!"uninstall.ico"

# Win32 .7z file
Remove-Item "$TITLE_UPPER-win32.7z"
&"$PATH_7Z/7z.exe" a -t7z -mmt -mx9 -r "$TITLE_UPPER-win32.7z" "$TITLE_LOWER-win32/" `
	-x!"installer-win32.nsi" -x!"install.ico" -x!"uninstall.ico"
