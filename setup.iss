; Example Inno Setup Script
#define MyAppName "GPShield"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "GPShield Technologies"
#define MyAppExeName "GPShieldServerApp.exe"
#define MyAppExeClient "GPShieldClientApp.exe"
#define MyAppIcon "E:\GPShield\app.ico"

[Setup]
AppId={{FA04BB14-6EF1-4859-AE56-5F495D3AF025}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\{#MyAppName}
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
DisableProgramGroupPage=yes
PrivilegesRequired=admin
OutputDir=E:\
OutputBaseFilename=GPShield
SetupIconFile={#MyAppIcon}
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "E:\GPShield\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
; Start Menu Shortcut with Icon
Name: "{autoprograms}\{#MyAppName} Server App"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{#MyAppIcon}"; IconIndex: 0
Name: "{autoprograms}\{#MyAppName} Client App"; Filename: "{app}\{#MyAppExeClient}"; IconFilename: "{#MyAppIcon}"; IconIndex: 0

; Desktop Shortcut with Icon
Name: "{autodesktop}\{#MyAppName} Server App"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon; IconFilename: "{#MyAppIcon}"; IconIndex: 0
Name: "{autodesktop}\{#MyAppName} Client App"; Filename: "{app}\{#MyAppExeClient}"; Tasks: desktopicon; IconFilename: "{#MyAppIcon}"; IconIndex: 0

; Startup Shortcut
Name: "{commonstartup}\{#MyAppName} Server App"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{#MyAppIcon}"; IconIndex: 0
Name: "{commonstartup}\{#MyAppName} Client App"; Filename: "{app}\{#MyAppExeClient}"; IconFilename: "{#MyAppIcon}"; IconIndex: 0