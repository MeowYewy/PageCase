; PageCase — Windows installer (Inno Setup 6)
; Build: scripts\package-installer.bat
; Or: ISCC /DAppVersion=0.2.1 /DSourceDir=dist\PageCase_0.2.1_win64 /DOutputDir=dist\artifacts packaging\windows\PageCase.iss

#ifndef AppVersion
  #define AppVersion "0.2.1"
#endif

#ifndef SourceDir
  #define SourceDir "..\..\dist\PageCase_" + AppVersion + "_win64"
#endif

#ifndef OutputDir
  #define OutputDir "..\..\dist\artifacts"
#endif

#ifndef AppIconFile
  #define AppIconFile "..\..\resources\app-icon.ico"
#endif

[Setup]
; Same AppId as PDF Studio 0.1.0 — required for in-place upgrade.
AppId={{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}
AppName=PageCase
AppVersion={#AppVersion}
AppPublisher=MeowYewy
AppPublisherURL=https://github.com/MeowYewy/PageCase
AppSupportURL=https://github.com/MeowYewy/PageCase/issues
AppUpdatesURL=https://github.com/MeowYewy/PageCase/releases
DefaultDirName={autopf}\PageCase
DefaultGroupName=PageCase
DisableProgramGroupPage=yes
UsePreviousAppDir=yes
UsePreviousGroup=yes
OutputDir={#OutputDir}
OutputBaseFilename=PageCase_{#AppVersion}_win64_Setup
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
UninstallDisplayName=PageCase {#AppVersion}
SetupIconFile={#AppIconFile}
UninstallDisplayIcon={#AppIconFile}
LicenseFile={#SourceDir}\LICENSE.txt
; Force-close both new and legacy process names during upgrade from v0.1.0.
CloseApplications=force
CloseApplicationsFilter=PageCase.exe,PDFStudio.exe,ProjectP.exe
RestartApplications=yes
AppMutex=PageCaseAppMutex

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "chinesesimplified"; MessagesFile: "languages\ChineseSimplified.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#SourceDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\PageCase"; Filename: "{app}\PageCase.exe"; IconFilename: "{app}\PageCase.exe"
Name: "{group}\{cm:UninstallProgram,PageCase}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\PageCase"; Filename: "{app}\PageCase.exe"; IconFilename: "{app}\PageCase.exe"; Tasks: desktopicon

[Run]
; postinstall + nowait: relaunch after /SILENT in-app update (do not use skipifsilent).
Filename: "{app}\PageCase.exe"; Description: "{cm:LaunchProgram,PageCase}"; Flags: nowait postinstall

[InstallDelete]
; Drop legacy Start Menu / desktop entries left from PDF Studio branding.
Type: filesandordirs; Name: "{autoprograms}\PDF Studio"
Type: files; Name: "{autodesktop}\PDF Studio.lnk"
Type: files; Name: "{userdesktop}\PDF Studio.lnk"
Type: files; Name: "{commondesktop}\PDF Studio.lnk"

[UninstallDelete]
Type: filesandordirs; Name: "{app}"

[Code]
const
  LegacyAppDirName = 'PDF Studio';
  LegacyDisplayPrefix = 'PDF Studio';
  UninstallRootPath = 'Software\Microsoft\Windows\CurrentVersion\Uninstall';

function IsLegacyDisplayName(const DisplayName: String): Boolean;
begin
  Result := (Pos(LegacyDisplayPrefix, DisplayName) = 1);
end;

procedure DeleteUninstallKeyIfLegacy(RootKey: Integer; const SubKeyPath: String);
var
  DisplayName, Publisher: String;
begin
  if not RegQueryStringValue(RootKey, SubKeyPath, 'DisplayName', DisplayName) then
    Exit;
  if not IsLegacyDisplayName(DisplayName) then
    Exit;
  if RegQueryStringValue(RootKey, SubKeyPath, 'Publisher', Publisher) then
  begin
    if CompareText(Publisher, 'MeowYewy') <> 0 then
      Exit;
  end;
  RegDeleteKeyIncludingSubkeys(RootKey, SubKeyPath);
end;

procedure SweepUninstallRoot(RootKey: Integer; const UninstallRoot: String);
var
  Names: TArrayOfString;
  I: Integer;
begin
  if not RegGetSubkeyNames(RootKey, UninstallRoot, Names) then
    Exit;
  for I := 0 to GetArrayLength(Names) - 1 do
    DeleteUninstallKeyIfLegacy(RootKey, UninstallRoot + '\' + Names[I]);
end;

procedure RemoveLegacyUninstallEntries;
begin
  { After AppId entry is PageCase, leftover "PDF Studio" keys are orphans. }
  SweepUninstallRoot(HKLM, UninstallRootPath);
  SweepUninstallRoot(HKCU, UninstallRootPath);
  SweepUninstallRoot(HKLM32, UninstallRootPath);
  SweepUninstallRoot(HKCU32, UninstallRootPath);
end;

procedure RemoveLegacyInstallDir;
var
  LegacyDir, CurrentApp, Unins: String;
  ResultCode: Integer;
begin
  LegacyDir := ExpandConstant('{autopf}\' + LegacyAppDirName);
  CurrentApp := ExpandConstant('{app}');
  if CompareText(RemoveBackslashUnlessRoot(LegacyDir), RemoveBackslashUnlessRoot(CurrentApp)) = 0 then
    Exit;
  if not DirExists(LegacyDir) then
    Exit;

  Unins := LegacyDir + '\unins000.exe';
  if FileExists(Unins) then
    Exec(Unins, '/VERYSILENT /SUPPRESSMSGBOXES /NORESTART', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);

  if DirExists(LegacyDir) then
    DelTree(LegacyDir, True, True, True);
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    RemoveLegacyInstallDir;
    RemoveLegacyUninstallEntries;
  end;
end;
