//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TButton *Button1;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TTimer *Timer1;
	TMenuItem *N6;
	TMenuItem *N7;
	TPanel *Panel1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall N2Click(TObject *Sender);
	void __fastcall BitBtnMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall Win();
	void __fastcall N3Click(TObject *Sender);
	void __fastcall N6Click(TObject *Sender);
private:	// User declarations
	int Boom_num[10][10];
    int flag_index[10][10];
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	void _fastcall initial();
	void _fastcall Start_Game();
    void __fastcall Random();
	void __fastcall OnClick(TObject *Sender);
	void __fastcall connectivity(int x, int y);
	void __fastcall diffusion(int x, int y);
    void __fastcall Delete_Map();
	void __fastcall count_boom();

	int Width = 0;
	int Height = 0;
	int boom = 0;
	int boomnum = 0;
	int Flag_num = 0;
    int btn_x = 0;
	int btn_y = 0;
	int sec = 0;
	bool start = false;
	bool openboom = false;
	bool win = true;

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
