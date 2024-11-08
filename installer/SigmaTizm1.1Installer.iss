; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{49A17013-5369-4F54-83D5-443427DBC164}
AppName=SigmaTizm
AppVerName=SigmaTizm 1.1
AppPublisher=Zeta Centauri
AppPublisherURL=http://zetacentauri.com
AppSupportURL=http://zetacentauri.com
AppUpdatesURL=http://zetacentauri.com
DefaultDirName={pf}\Zeta Centauri\SigmaTizm
DefaultGroupName=Zeta Centauri\SigmaTizm
LicenseFile=C:\Users\Xangis\code\Sigmatizm\Release\License.txt
OutputDir=C:\Users\Xangis\code\Sigmatizm\installer
OutputBaseFilename=SigmaTizm1.1Setup
SetupIconFile=C:\Users\Xangis\code\Sigmatizm\Release\sigmatizm.ico
UninstallDisplayIcon={app}\sigmatizm.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "C:\Users\Xangis\code\Sigmatizm\Release\SigmaTizm.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\Sigmatizm\Release\hsliderbk.bmp"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\Sigmatizm\Release\hsliderind.bmp"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\Sigmatizm\Release\octave.bmp"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\Sigmatizm\Release\portaudio_x86.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\Sigmatizm\Release\sigmatizm.htb"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\Sigmatizm\Release\SigmaTizm.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\Sigmatizm\Release\sigmatizm.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\Sigmatizm\Release\sliderbk.bmp"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\Sigmatizm\Release\sliderind.bmp"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\Sigmatizm\Release\spin.bmp"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\Sigmatizm\Release\Patches\*"; DestDir: "{localappdata}\Sigmatizm\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\Xangis\code\Sigmatizm\Release\Patches\*"; DestDir: "{app}\Patches\"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "C:\Users\Xangis\code\Sigmatizm\Release\vcredist2010_x86.exe"; DestDir: "{app}"; Flags: ignoreversion deleteafterinstall

[Icons]
Name: "{group}\SigmaTizm"; Filename: "{app}\SigmaTizm.exe"; WorkingDir: "{app}"
Name: "{group}\{cm:ProgramOnTheWeb,SigmaTizm}"; Filename: "http://zetacentauri.com/software_sigmatizm.htm"
Name: "{commondesktop}\SigmaTizm"; Filename: "{app}\SigmaTizm.exe"; Tasks: desktopicon; WorkingDir: "{app}"
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\SigmaTizm"; Filename: "{app}\SigmaTizm.exe"; Tasks: quicklaunchicon; WorkingDir: "{app}"

[Run]
Filename: "{app}\vcredist2010_x86.exe"; Parameters: "/q"; WorkingDir: "{app}";  StatusMsg: "Installing Visual C++ 2010 Redistributable..."; Flags: waituntilterminated
Filename: "{app}\SigmaTizm.exe"; Description: "{cm:LaunchProgram,SigmaTizm}"; Flags: nowait postinstall skipifsilent

