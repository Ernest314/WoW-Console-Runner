Unicode true

!define VERSION "v2.0"
!define NAME_LOWER "wow-console-runner"
!define NAME_UPPER "WoW-Console-Runner"
!define NAME_SPACE "WoW Console Runner"
!define FILE_LICENSE "License.md"

!include "MUI2.nsh"


;====================;
;= General Settings =;
;====================;
; Some of these aren't necessarily MUI-specific.
Name "${NAME_SPACE}"
OutFile "${NAME_UPPER}-win32.exe"
RequestExecutionLevel admin ; For writing to start menu & Program Files
SetCompressor /SOLID lzma
SetCompressorDictSize 32 ; MB - default is 8

!define MUI_ICON	"install.ico"
!define MUI_UNICON	"uninstall.ico"
; TODO: Add the other settings for MUI2 (and NSIS in general?).

InstallDir "$PROGRAMFILES32\${NAME_SPACE}"
!define APPDATADIR "$APPDATA\${NAME_LOWER}"


;================;
;= MUI Settings =;
;================;
; These macros must be set before the page macros are called
; in order to take effect. Otherwise default values will be used.
!define MUI_WELCOMEPAGE_TITLE		$(STR_MUI_WELCOMEPAGE_TITLE)
!define MUI_WELCOMEPAGE_TEXT		$(STR_MUI_WELCOMEPAGE_TEXT)

!define MUI_PAGE_HEADER_TEXT		$(STR_MUI_PAGE_HEADER_TEXT)
!define MUI_PAGE_HEADER_SUBTEXT		$(STR_MUI_PAGE_HEADER_SUBTEXT)

!define MUI_LICENSEPAGE_TEXT_TOP	$(STR_MUI_LICENSEPAGE_TEXT_TOP)
!define MUI_LICENSEPAGE_TEXT_BOTTOM	$(STR_MUI_LICENSEPAGE_TEXT_BOTTOM)

!define MUI_DIRECTORYPAGE_TEXT_TOP	$(STR_MUI_DIRECTORYPAGE_TEXT_TOP)

!define MUI_INSTFILESPAGE_FINISHHEADER_TEXT		$(STR_MUI_INSTFILESPAGE_FINISHHEADER_TEXT)
!define MUI_INSTFILESPAGE_FINISHHEADER_SUBTEXT	$(STR_MUI_INSTFILESPAGE_FINISHHEADER_SUBTEXT)
!define MUI_INSTFILESPAGE_ABORTHEADER_TEXT		$(STR_MUI_INSTFILESPAGE_ABORTHEADER_TEXT)
!define MUI_INSTFILESPAGE_ABORTHEADER_SUBTEXT	$(STR_MUI_INSTFILESPAGE_ABORTHEADER_SUBTEXT)

!define MUI_FINISHPAGE_TITLE			$(STR_MUI_FINISHPAGE_TITLE)
!define MUI_FINISHPAGE_TEXT				$(STR_MUI_FINISHPAGE_TEXT)
!define MUI_FINISHPAGE_RUN				$INSTDIR\${NAME_LOWER}.exe
!define MUI_FINISHPAGE_RUN_TEXT			$(STR_MUI_FINISHPAGE_RUN_TEXT)
!define MUI_FINISHPAGE_LINK				$(STR_MUI_FINISHPAGE_LINK)
!define MUI_FINISHPAGE_LINK_LOCATION	$(STR_MUI_FINISHPAGE_LINK_LOCATION)
!define MUI_FINISHPAGE_NOREBOOTSUPPORT

; Default uninstall settings are good.


;====================;
;= Installer Layout =;
;====================;
; The sequential layout of the installer/uninstaller.
; MUI_LANGUAGE must be defined after these declarations.
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE ${FILE_LICENSE}
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"
; TODO: make this dynamic


;====================;
;= Language Strings =;
;====================;
; The language-specific translations of all the required strings.
; TODO: define strings for other languages.
LangString STR_MUI_WELCOMEPAGE_TITLE		${LANG_ENGLISH} "Install ${NAME_SPACE}"
LangString STR_MUI_WELCOMEPAGE_TEXT			${LANG_ENGLISH} "This program will install ${NAME_SPACE} ${VERSION} on your computer."

LangString STR_MUI_PAGE_HEADER_TEXT			${LANG_ENGLISH} "${NAME_SPACE}"
LangString STR_MUI_PAGE_HEADER_SUBTEXT		${LANG_ENGLISH} "${VERSION}"

LangString STR_MUI_LICENSEPAGE_TEXT_TOP		${LANG_ENGLISH} "License agreement:"
LangString STR_MUI_LICENSEPAGE_TEXT_BOTTOM	${LANG_ENGLISH} "You must agree to the license terms to continue."

LangString STR_MUI_DIRECTORYPAGE_TEXT_TOP	${LANG_ENGLISH} " Click $\"Browse$\" to select a different folder to install to."

LangString STR_MUI_INSTFILESPAGE_FINISHHEADER_TEXT		${LANG_ENGLISH} "Installation Finished"
LangString STR_MUI_INSTFILESPAGE_FINISHHEADER_SUBTEXT	${LANG_ENGLISH} "${NAME_SPACE} was installed. Click $\"Next$\" to continue."
LangString STR_MUI_INSTFILESPAGE_ABORTHEADER_TEXT		${LANG_ENGLISH} "Installation Cancelled"
LangString STR_MUI_INSTFILESPAGE_ABORTHEADER_SUBTEXT	${LANG_ENGLISH} "${NAME_SPACE} was not installed on your computer."

LangString STR_MUI_FINISHPAGE_TITLE			${LANG_ENGLISH} "Installation Complete"
LangString STR_MUI_FINISHPAGE_TEXT			${LANG_ENGLISH} "${NAME_SPACE} ${VERSION} is now installed on your computer."
LangString STR_MUI_FINISHPAGE_RUN_TEXT		${LANG_ENGLISH} "Run ${NAME_SPACE} (as admin) on exit."
LangString STR_MUI_FINISHPAGE_LINK			${LANG_ENGLISH} "erythro.space/go/${NAME_LOWER}"
LangString STR_MUI_FINISHPAGE_LINK_LOCATION	${LANG_ENGLISH} "http://erythro.space/go/${NAME_LOWER}/"


;====================;
;= Installer Script =;
;====================;
Section "Core Files" SectionCore

SetOutPath $INSTDIR

WriteUninstaller $INSTDIR\uninstall-${NAME_LOWER}.exe

File License.md

File ${NAME_LOWER}.exe

File msvcp140.dll
File vccorlib140.dll
File vcruntime140.dll

File Qt5Core.dll
File Qt5Gui.dll
File Qt5Widgets.dll

SetOutPath $INSTDIR\platforms
File platforms\qwindows.dll

CreateDirectory "$SMPROGRAMS\${NAME_SPACE}"
CreateShortCut "$SMPROGRAMS\${NAME_SPACE}\${NAME_SPACE}.lnk" "$INSTDIR\${NAME_LOWER}.exe"
CreateShortCut "$SMPROGRAMS\${NAME_SPACE}\Uninstall ${NAME_SPACE}.lnk" "$INSTDIR\uninstall-${NAME_LOWER}.exe"

; See: http://nsis.sourceforge.net/Add_uninstall_information_to_Add/Remove_Programs
!define UNINSTALL_REGISTRY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME_UPPER}"
WriteRegStr HKLM ${UNINSTALL_REGISTRY} "DisplayName" "${NAME_SPACE} ${VERSION}"
WriteRegStr HKLM ${UNINSTALL_REGISTRY} "UninstallString" "$\"$INSTDIR\uninstall-${NAME_LOWER}.exe$\""
WriteRegStr HKLM ${UNINSTALL_REGISTRY} "InstallLocation" "$INSTDIR"
WriteRegStr HKLM ${UNINSTALL_REGISTRY} "DisplayIcon" "$INSTDIR\${NAME_LOWER}.exe"
WriteRegStr HKLM ${UNINSTALL_REGISTRY} "Publisher" "Ernest Gu"
WriteRegStr HKLM ${UNINSTALL_REGISTRY} "URLInfoAbout" "$(STR_MUI_FINISHPAGE_LINK_LOCATION)"
WriteRegStr HKLM ${UNINSTALL_REGISTRY} "DisplayVersion" "${VERSION}"
WriteRegDWORD HKLM ${UNINSTALL_REGISTRY} "NoModify" 1
WriteRegDWORD HKLM ${UNINSTALL_REGISTRY} "NoRepair" 1

SectionEnd


;======================;
;= Uninstaller Script =;
;======================;
Section "Uninstall"

; Always delete uninstaller first.
Delete $INSTDIR\uninstall-${NAME_LOWER}.exe

Delete $INSTDIR\License.md

Delete ${APPDATADIR}\paths.txt
RMDir /r ${APPDATADIR}\Logs
RMDir ${APPDATADIR}

Delete $INSTDIR\${NAME_LOWER}.exe

Delete $INSTDIR\msvcp140.dll
Delete $INSTDIR\vccorlib140.dll
Delete $INSTDIR\vcruntime140.dll

Delete $INSTDIR\Qt5Core.dll
Delete $INSTDIR\Qt5Gui.dll
Delete $INSTDIR\Qt5Widgets.dll

Delete $INSTDIR\platforms\qwindows.dll
RMDir $INSTDIR\platforms

; As long as `/r` isn't specified, RMDir only removes empty directories.
; Using `/r` is dangerous since the user's files could be removed.
; If $INSTDIR is the root of /Program Files/, all of the user's programs
; would be (perma-?)deleted by the installer without any warning.
RMDir $INSTDIR

Delete "$SMPROGRAMS\${NAME_SPACE}\${NAME_SPACE}.lnk"
Delete "$SMPROGRAMS\${NAME_SPACE}\Uninstall ${NAME_SPACE}.lnk"

DeleteRegKey HKLM ${UNINSTALL_REGISTRY}

SectionEnd
