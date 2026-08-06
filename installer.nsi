Name "Mail Client"
OutFile "MailClientSetup.exe"

InstallDir "$PROGRAMFILES64\Mail Client"

RequestExecutionLevel admin

Page directory
Page instfiles

Section

SetOutPath "$INSTDIR"

File /r "release\*.*"

CreateShortcut "$DESKTOP\Mail Client.lnk" "$INSTDIR\appqtapptestmail.exe"

WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

Section "Uninstall"

Delete "$DESKTOP\Mail Client.lnk"

RMDir /r "$INSTDIR"

SectionEnd