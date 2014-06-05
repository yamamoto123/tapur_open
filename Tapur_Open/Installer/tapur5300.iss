[Setup]
PrivilegesRequired=none
VersionInfoVersion=5.3.0.111
VersionInfoTextVersion=5.3.0.111
VersionInfoCompany=tapur.com
UsePreviousTasks=No
OutputDir=C:\Tapur\Installer\Setup
DisableProgramGroupPage=Yes
OnlyBelowVersion=4.9.3000,6.9
MinVersion=4.9.3000,5.0.2195
ShowTasksTreeLines=Yes
AppName=Tapur
AppCopyright=Copyright (C) 2004-2011 tapur.com
AppVerName=Tapur 5.3.0.111
AppPublisher=tapur.com
AppPublisherURL=http://www.tapur.com
AppSupportURL=http://www.tapur.com
AppUpdatesURL=http://www.tapur.com
DefaultDirName={pf}\Tapur
DefaultGroupName=Tapur
LicenseFile=C:\Tapur\Installer\New\license.txt
OutputBaseFilename=tp5300
SetupIconFile=C:\Tapur\Installer\New\tapur32.ico
SolidCompression=Yes
ShowUndisplayableLanguages=Yes

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"; InfoAfterFile: "C:\Tapur\Installer\New\install_en.txt"
Name: "jp"; MessagesFile: "compiler:Languages\Japanese.isl"; InfoAfterFile: "C:\Tapur\Installer\New\install_jp.txt"
Name: "de"; MessagesFile: "compiler:Languages\German.isl"; InfoAfterFile: "C:\Tapur\Installer\New\install_de.txt"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"
Name: "startupicon"; Description: "スタートアップに登録する"; Languages: jp
Name: "answering"; Description: "起動時の留守番応答機能を無効にする"; Languages: jp; Flags: unchecked;
Name: "ClearData"; Description: "作成済みのユーザデータを完全に削除する"; Languages: jp; Flags: unchecked;
Name: "startupicon"; Description: "Start Tapur when the computer starts"; Languages: en
Name: "answering"; Description: "Automatically invalidate the function of the answering machine when starting Tapur"; Languages: en; Flags: unchecked
Name: "ClearData"; Description: "Delete all the data made by old version Tapur"; Languages: en; Flags: unchecked
Name: "startupicon"; Description: "Start Tapur, wenn der Computer startet"; Languages: de
Name: "answering"; Description: "Automatisch zur Ungultigkeit der Funktion der Anrufbeantworter beim Start Tapur"; Languages: de; Flags: unchecked
Name: "ClearData"; Description: "Loschen Sie alle Daten, die alte Version Tapur"; Languages: de; Flags: unchecked

[Files]
Source: "New\instchk.exe"; DestDir: "{app}"; Flags: ignoreversion;
Source: "New\tapmgr.exe"; DestDir: "{app}"; Flags: ignoreversion;
Source: "New\tapmgr.exe.local"; DestDir: "{app}"; Flags: ignoreversion;
Source: "New\Tapur.exe"; DestDir: "{app}"; Flags: ignoreversion; BeforeInstall: MyBeforeInstall
Source: "New\Tapur.exe.local"; DestDir: "{app}"; Flags: ignoreversion;
Source: "New\ogg.dll"; DestDir: "{app}"; Flags: comparetimestamp replacesameversion;
Source: "New\vorbisfile.dll"; DestDir: "{app}"; Flags: comparetimestamp replacesameversion;
Source: "New\vorbisenc.dll"; DestDir: "{app}"; Flags: comparetimestamp replacesameversion;
Source: "New\vorbis.dll"; DestDir: "{app}"; Flags: comparetimestamp replacesameversion;
Source: "New\sqlite3.dll"; DestDir: "{app}"; Flags: comparetimestamp replacesameversion;
Source: "New\vidgrab_VC6.ocx"; DestDir: "{app}"; Flags: comparetimestamp replacesameversion regserver noregerror;
Source: "New\ActiveST.dll"; DestDir: "{app}"; Flags: comparetimestamp replacesameversion regserver noregerror;
Source: "New\init.db"; DestDir: "{app}"; Flags: ignoreversion;
Source: "New\license.txt"; DestDir: "{app}"; Flags: ignoreversion;
Source: "New\beep.wav"; DestDir: "{app}"; Flags: ignoreversion;
Source: "New\wav\beep_dog.wav"; DestDir: "{app}\wav";
Source: "New\wav\beep_cat.wav"; DestDir: "{app}\wav";
Source: "New\greeting_jp.wav"; DestDir: "{app}\wav";
Source: "New\greeting_en.wav"; DestDir: "{app}\wav";
Source: "New\bin\1.csv"; DestDir: "{app}\bin"; Flags: ignoreversion;
Source: "New\bin\tapmail.bat"; DestDir: "{app}\bin"; Flags: ignoreversion;
Source: "New\bin\tapmail.vbs"; DestDir: "{app}\bin"; Flags: ignoreversion;
Source: "New\bin\11.csv"; DestDir: "{app}\bin"; Flags: ignoreversion;
Source: "New\bin\tapftp.bat"; DestDir: "{app}\bin"; Flags: ignoreversion;
Source: "New\res\jp.dll"; DestDir: "{app}"; DestName: "lang.dll"; Languages: jp; Flags: comparetimestamp replacesameversion;
Source: "New\manual_jp.pdf"; DestDir: "{app}"; DestName: "manual.pdf"; Languages: jp; Flags: ignoreversion;
Source: "New\install_jp.txt"; DestDir: "{app}"; DestName: "install.txt"; Languages: jp; Flags: ignoreversion;
Source: "New\install_jp.pdf"; DestDir: "{app}"; DestName: "install.pdf"; Languages: jp; Flags: ignoreversion;
Source: "New\greeting_jp.wav"; DestDir: "{app}"; DestName: "greeting.wav"; Languages: jp; Flags: ignoreversion;
Source: "New\res\en.dll"; DestDir: "{app}"; DestName: "lang.dll"; Languages: en; Flags: comparetimestamp replacesameversion;
Source: "New\manual_en.pdf"; DestDir: "{app}"; DestName: "manual.pdf"; Languages: en; Flags: ignoreversion;
Source: "New\install_en.txt"; DestDir: "{app}"; DestName: "install.txt"; Languages: en; Flags: ignoreversion;
Source: "New\install_en.pdf"; DestDir: "{app}"; DestName: "install.pdf"; Languages: en; Flags: ignoreversion;
Source: "New\greeting_en.wav"; DestDir: "{app}"; DestName: "greeting.wav"; Languages: en; Flags: ignoreversion;
Source: "New\res\en.dll"; DestDir: "{app}"; DestName: "lang.dll"; Languages: de; Flags: comparetimestamp replacesameversion;
Source: "New\manual_de.pdf"; DestDir: "{app}"; DestName: "manual.pdf"; Languages: de; Flags: ignoreversion;
Source: "New\install_de.txt"; DestDir: "{app}"; DestName: "install.txt"; Languages: de; Flags: ignoreversion;
Source: "New\install_de.pdf"; DestDir: "{app}"; DestName: "install.pdf"; Languages: de; Flags: ignoreversion;
Source: "New\greeting_en.wav"; DestDir: "{app}"; DestName: "greeting.wav"; Languages: de; Flags: ignoreversion;
Source: "New\utvideo-8.5.2-x86.msi"; DestDir: "{app}"; Flags: ignoreversion;
Source: "New\LagarithSetup_1324.exe"; DestDir: "{app}"; Flags: ignoreversion;
Source: "New\utvideo-8.5.2-src.zip"; DestDir: "{app}"; Flags: ignoreversion;
Source: "New\Lagarith_1324_Source.zip"; DestDir: "{app}"; Flags: ignoreversion;
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[INI]
Filename: "{app}\Tapur.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.tapur.com/"

[Icons]
Name: "{group}\Tapur"; Filename: "{app}\tapmgr.exe"
Name: "{group}\{cm:ProgramOnTheWeb,Tapur}"; Filename: "{app}\Tapur.url"
Name: "{group}\{cm:UninstallProgram,Tapur}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Tapur"; Filename: "{app}\tapmgr.exe"; Tasks: desktopicon
Name: "{group}\Tapurの使い方"; Filename: "{app}\manual.pdf"; Languages: jp
Name: "{group}\インストールについて"; Filename: "{app}\install.pdf"; Languages: jp
Name: "{group}\ライセンスについて"; Filename: "{app}\license.txt"; Languages: jp
Name: "{group}\First step of Tapur"; Filename: "{app}\manual.pdf"; Languages: en
Name: "{group}\About Install"; Filename: "{app}\install.pdf"; Languages: en
Name: "{group}\About License"; Filename: "{app}\license.txt"; Languages: en
Name: "{group}\Kurzanleitung"; Filename: "{app}\manual.pdf"; Languages: de
Name: "{group}\Installanduninstall"; Filename: "{app}\install.pdf"; Languages: de
Name: "{group}\Uber License"; Filename: "{app}\license.txt"; Languages: de

[Run]
Filename: "{app}\LagarithSetup_1324.exe"; Description: "Install [Lagarith Lossless Video Codec 1.3.24]"; Flags: postinstall skipifsilent; WorkingDir: "{app}";
Filename: "{app}\utvideo-8.5.2-x86.msi"; Description: "Install [Ut Video Codec Suite 8.5.2]"; Flags: postinstall shellexec skipifsilent; WorkingDir: "{app}";
Filename: "{app}\tapmgr.exe"; Description: "{cm:LaunchProgram,Tapur}"; Flags: postinstall nowait skipifsilent; WorkingDir: "{app}";
Filename: "{app}\Tapur.url"; Description: "{cm:ProgramOnTheWeb,Tapur}"; Flags: postinstall nowait shellexec skipifsilent;

[UninstallDelete]
Type: files; Name: "{app}\Tapur.url"

[UninstallRun]
Filename: "{app}\instchk.exe"; Flags: waituntilterminated;

[InstallDelete]
Type: filesandordirs; Name: "{userappdata}\Tapur"; Tasks: ClearData;

[Messages]
jp.WelcomeLabel2=このプログラムはご使用のコンピュータへ [name/ver] をインストールします。%n%n続行する前に他のアプリケーションをすべて終了してください。%n%n旧バージョンの[name]がインストールされている場合は、先にアンインストールを行い、コンピュータを再起動してからインストールを行ってください。
en.WelcomeLabel2=This program install [name/ver] into your computer.%n%nBefore you install, quit all applications.%n%nIf you already have a previous version of [name] installed, you had better remove it and reboot your computer first.
de.WelcomeLabel2=Dieses Programm installieren [name / ver] auf Ihren Computer.%n%nBefore Installation, beenden Sie alle Anwendungen.%n%nWenn Sie bereits eine altere Version von [name] installiert ist, Sie hatten besser entfernen Sie es und starten Sie Ihren Computer neu zuerst.

[Registry]
Root: HKCU; Subkey: "Software\Tapur\Tapur\Settings"; ValueType: dword; ValueName: "EnableVoiceRecordConfirm"; ValueData: "1"
Root: HKCU; Subkey: "Software\Tapur\Tapur\Settings"; ValueType: dword; ValueName: "EnableVideoRecordConfirm"; ValueData: "1"
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "Tapur"; ValueData: "{app}\tapmgr.exe"; Tasks: startupicon ; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Tapur"; Tasks: ClearData; Flags: deletekey
Root: HKCU; Subkey: "Software\Tapur\Tapur\Settings"; ValueType: dword; ValueName: "InitDisable"; ValueData: "1"; Tasks: answering

[Code]
procedure MyBeforeInstall();
var
  ResultCode: Integer;
begin
  Exec(ExpandConstant('{app}\instchk.exe'), '', '', SW_HIDE,
     ewWaitUntilTerminated, ResultCode);
end;
