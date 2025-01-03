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
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdIPWatch.hpp>
#include <Vcl.ExtCtrls.hpp>

typedef struct _PACKET
{
  String ID;  //4 bytes
  String Host; // unfixed length
  String separation;  // 1 byte, "#"
  String MainFunc;  // 1 byte
  String SubFunc;  // 1 byte
  int MsgLength; // 3 byte
  String Msg; //unfixed length
  String EndSymbol; // 1 byte, @
}PACKET, *pPACKET;

typedef struct _CLIENT_SOCKET
{
  bool active;
  TCustomWinSocket *pSocket;
  String ClientID;
}CLIENTSOCKET, *pCLIENTSOCKET;

enum Suit { CLUBS, SPADES, HEARTS, DIAMONDS };

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *Tools;
	TMenuItem *Listen;
	TMenuItem *Connect;
	TMenuItem *Disconnect;
	TServerSocket *ServerSocket1;
	TClientSocket *ClientSocket1;
	TStatusBar *StatusBar1;
	TIdIPWatch *IdIPWatch1;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TTimer *Timer1;
	TTimer *Timer2;
	TMenuItem *Debug;
	TMenuItem *Opendebugwindow;
	TComboBox *ComboBox1;
	TMemo *Memo1;
	TButton *Button1;
	TMenuItem *N1;
	TMenuItem *Start_Game;
	TButton *Button5;
	TButton *Button4;
	TButton *Button3;
	TButton *Button2;
	TTimer *Timer3;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TTimer *Timer4;
	TTimer *Timer5;
	void __fastcall ListenClick(TObject *Sender);
	void __fastcall ConnectClick(TObject *Sender);
	void __fastcall ClientSocket1Connect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall DisconnectClick(TObject *Sender);
	void __fastcall ClientSocket1Error(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
	void __fastcall ClientSocket1Read(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocket1ClientConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocket1ClientDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocket1ClientError(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode);
	void __fastcall ServerSocket1ClientRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall OpendebugwindowClick(TObject *Sender);
	void __fastcall ServerSocket1Accept(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Timer2Timer(TObject *Sender);
	void __fastcall ClientSocket1Disconnect(TObject *Sender, TCustomWinSocket *Socket);
    void __fastcall SendMsg(TObject *Sender);
	void __fastcall Start_GameClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Timer3Timer(TObject *Sender);
	void __fastcall Timer4Timer(TObject *Sender);
	void __fastcall Timer5Timer(TObject *Sender);








private:	// User declarations
// 定義撲克牌結構
	struct TCard
	{
        Suit suit;
		int value;
	};


	void __fastcall TForm1:: Disconnect_interface(int Num);
	// 顯示玩家的撲克牌在對應的 Panel 上
	void displayPlayerCards(TPanel* playerPanel, const TCard hand[5], int player_num);
	void __fastcall TForm1::Licensing(); // 洗牌顯示牌況
	void __fastcall TForm1::bet(String bet);   // 下注
    int TForm1::getSuitIndex(int card);
	void __fastcall TForm1::who_win(int clientNum); // 判斷誰贏
	void __fastcall TForm1::initialize();   //初始化
	void __fastcall TForm1::reset();   		//重置
	void __fastcall TForm1::play_Game();   //開始遊戲
	int TForm1::get_card_num(int cardIndex);
    int TForm1::To_simple(int card);
	String TForm1::compareCards(int player_num); // 五張牌比大小

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);

    bool CheckClients(TCustomWinSocket *);
	String AssemblePacket(PACKET );
	pPACKET DisassemblePacket(String );
	void ShowPacket(PACKET);
	void ShowPacket(pPACKET);
	void ParsePacket(pPACKET pPkg);
	void ReflashClientList();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
