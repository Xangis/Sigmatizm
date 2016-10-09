echo This program runs the desktop app packager command to create a Windows 10 store package from an existing installer.
pause
DesktopAppConverter.exe -Installer "C:\Users\Xangis\code\Sigmatizm\installer\Sigmatizm1.1Setup.exe" -PackageName "Sigmatizm" -Version 1.1.0.0 -Makeappx -Destination "C:\Users\Xangis\code\Sigmatizm\installer" -InstallerArguments "/VERYSILENT" -Publisher "CN=Jason Champion" -Sign
Certutil -addStore TrustedPeople Sigmatizm\auto-generated.cer



