//
//  PassDuino
//  Version 1.0 (August 2020)
//
//  Made by ERDesigns - Ernst Reidinga
//  as an open-source project
//
//  If you change code, add functions, fix bugs or anything else
//  please share them - so i can update the GIT and update the code
//  on the website so others can use it too :)
//
//  I used some open-source / public domain - code from Torry, and SourceForce.
//
//  If you like my work, and you like to support by donating please send a mail
//  and ill send my paypal address where you can send the donation.
//
//

unit untAddPassword;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, dxGDIPlusClasses,
  Vcl.ExtCtrls, Vcl.ComCtrls;

type
  TfrmAddPassword = class(TForm)
    Panel1: TPanel;
    Image1: TImage;
    Label1: TLabel;
    Label2: TLabel;
    Panel2: TPanel;
    btnCancel: TButton;
    btnOK: TButton;
    Label3: TLabel;
    edtTitle: TEdit;
    Label4: TLabel;
    edtPassword: TEdit;
    Label5: TLabel;
    edtHotkey: THotKey;
  private
    { Private declarations }
  public
    { Public declarations }
    function Execute : TModalResult;
  end;

var
  frmAddPassword: TfrmAddPassword;

implementation

{$R *.dfm}

uses untSettings;

//
//  Execute Dialog
//
function TfrmAddPassword.Execute : TModalResult;
begin
  edtTitle.Text     := '';
  edtPassword.Text  := '';
  edtPassword.MaxLength := frmSettings.seStringLength.Value;
  edtHotkey.HotKey  := 0;
  Result := ShowModal;
end;

end.
