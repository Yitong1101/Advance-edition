//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <System.Win.ScktComp.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TStatusBar *StatusBar1;
	TServerSocket *ServerSocket1;
	TClientSocket *ClientSocket1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TTimer *Timer1;
	TMemo *Memo2;
	TTimer *Timer2;
	TTimer *Timer3;
	TMenuItem *Tool;
	TMenuItem *Listen;
	TMenuItem *Connect;
	TMenuItem *Disconnect;
	TTimer *Timer4;
	void __fastcall ListenClick(TObject *Sender);
	void __fastcall ServerSocket1Accept(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocket1ClientConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocket1ClientDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocket1ClientError(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode);
	void __fastcall ServerSocket1ClientRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ConnectClick(TObject *Sender);
	void __fastcall ClientSocket1Connect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket1Disconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall DisconnectClick(TObject *Sender);
	void __fastcall ClientSocket1Error(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
	void __fastcall ClientSocket1Read(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Timer2Timer(TObject *Sender);
	void __fastcall Timer3Timer(TObject *Sender);
	void __fastcall Timer4Timer(TObject *Sender);




private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	void __fastcall New_shape(TShape* who_bingo[5][5], int left);
	void __fastcall New_label(TLabel* who_label[5][5], int left, int temp[5][5]);
	void __fastcall Random(int temp[5][5], int num, bool state);
	void __fastcall Reset();
	void __fastcall initial();
	void __fastcall start_game();
	void __fastcall first_attack(int num);
	void __fastcall mark_num(int num);
	void __fastcall show_value(int temp[5][5], TLabel *label[5][5]);
	void __fastcall get_value(int temp[5][5], int no);
	int bingo_V_H(int bingo_num, bool bingo, int VH, int click[5][5]);
	int bingo_slash(int bingo_num, bool bingo, int LR, int click[5][5]);
	void __fastcall Get_Select__Number();
	void __fastcall Send_Get_Id();
	void __fastcall sure_new_game();
	void __fastcall new_game_get_answer(int* newgame);
	void __fastcall new_game_judge();
	void __fastcall new_game_send_pocket(String yse_or_no, int event);
	void __fastcall show_result(String str);
	void __fastcall bingo_show();
	void __fastcall read_packet(AnsiString asStr);
	void __fastcall send_pocket(TShape *player_bingo[5][5], int temp1[5][5], int row, int col, int judge, int attack, int click[5][5]);
	void __fastcall choose_num(TObject *Sender);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
