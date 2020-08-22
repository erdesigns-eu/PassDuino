program PassDuino;

uses
  Vcl.Forms,
  WinApi.Windows,
  System.SysUtils,
  untMain in '..\Units\untMain.pas' {frmMain},
  untFunctions in '..\Units\untFunctions.pas' {dmFunctions: TDataModule},
  untUSB in '..\Units\untUSB.pas',
  untSettings in '..\Units\untSettings.pas' {frmSettings},
  untAddPassword in '..\Units\untAddPassword.pas' {frmAddPassword},
  untEditPassword in '..\Units\untEditPassword.pas' {frmEditPassword},
  untAddProfile in '..\Units\untAddProfile.pas' {frmAddProfile},
  untSelectProfile in '..\Units\untSelectProfile.pas' {frmSelectProfile},
  SetupApi in '..\Units\SetupApi.pas',
  ModuleLoader in '..\Units\ModuleLoader.pas';

{$R *.res}

const
  Mutex = 'PassDuino';

begin
  // Check for running instance - we only want this to run once..
  if CreateMutex(nil, True, Mutex) = 0 then RaiseLastOSError;
  if GetLastError = ERROR_ALREADY_EXISTS then Exit;

  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.Title := 'PassDuino';
  Application.CreateForm(TfrmMain, frmMain);
  Application.CreateForm(TfrmAddPassword, frmAddPassword);
  Application.CreateForm(TfrmEditPassword, frmEditPassword);
  Application.CreateForm(TfrmAddProfile, frmAddProfile);
  Application.CreateForm(TfrmSelectProfile, frmSelectProfile);
  Application.CreateForm(TfrmSettings, frmSettings);
  Application.CreateForm(TdmFunctions, dmFunctions);
  Application.Run;
end.
