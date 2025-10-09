!include "MUI2.nsh"
!include "logiclib.nsh"
!include "x64.nsh"

# The order of declarations in here is vital for things working as expected. The header defines must come before the page macros and they must come before the language macro.

Unicode True

!define MUI_ICON "barkout.ico"
!define MUI_HEADERIMAGE
!define MUI_WELCOMEFINISHPAGE_BITMAP "sidebanner.bmp"
!define MUI_HEADERIMAGE_BITMAP "topbanner.bmp"
!define MUI_HEADERIMAGE_RIGHT

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
#!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

!define NAME "Barkout"
!define PRODUCT_NAME "Barkout"
!define PRODUCT_VERSION "1.2.0.0"
!define VERSION "1.2.0.0"
!define PRODUCT_PUBLISHER "FatHorseGames; Stephen Branley"
!define OUTFILENAME "barkout-installer-${PRODUCT_VERSION}.exe"

Name "Barkout"

VIProductVersion "${VERSION}"
VIAddVersionKey  "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey  "CompanyName" "${PRODUCT_PUBLISHER}"
VIAddVersionKey  "ProductVersion" "${PRODUCT_VERSION}"
VIAddVersionKey  "FileVersion" "${PRODUCT_VERSION}"
VIAddVersionKey  "FileDescription" "${PRODUCT_NAME} ${VERSION}"
VIAddVersionKey  "LegalCopyright" "Copyright Stephen Branley 2024-2025"

OutFile "${OUTFILENAME}"

InstallDir $PROGRAMFILES64\FatHorseGames\Barkout\

#Page directory
#Page components
#Page instfiles

# define uninstaller name

Section "Game" game_id
	SetOutPath $INSTDIR
	File ../Barkout/barkout.exe
	File ../COPYING
	File ../README.md
  File ../Barkout/*.dll
	File /r ../Fonts
	File /r ../Sounds
	File /r ../Sprites
  CreateDirectory $INSTDIR\Levels
  SetOutPath $INSTDIR\Levels
  File ../Levels/levels.dat
  SetOutPath $INSTDIR
  WriteUninstaller $INSTDIR\uninstaller.exe
SectionEnd

Section "Start menu icon"
	CreateShortcut "$SMPROGRAMS\Barkout.lnk" "$INSTDIR\barkout.exe"
SectionEnd

Section "Desktop icon"
	CreateShortcut "$Desktop\Barkout.lnk" "$INSTDIR\barkout.exe"
SectionEnd


Section "Uninstall"
	# Delete loose files
	Delete $INSTDIR\uninstaller.exe
	Delete $INSTDIR\*.dll
	Delete $INSTDIR\COPYING
	Delete $INSTDIR\README.md
	Delete $INSTDIR\barkout.exe

	# Delete the directories
	RMDir /r $INSTDIR\Fonts
	RMDir /r $INSTDIR\Levels
	RMDir /r $INSTDIR\Sounds
	RMDir /r $INSTDIR\Sprites
  RMDir /r $INSTDIR\DLL
	RMDir $INSTDIR
SectionEnd

Function .oninit
	SectionSetFlags ${game_id} 17
FunctionEnd
