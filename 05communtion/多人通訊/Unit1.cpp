﻿
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"

#define  CLIENT_MAX 4 // the maximum of client
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

String strHost;
bool blnServer;  //當成server or client
String newClient="";
String ClientHostName="";
String ServerName="";
bool fgConnectState;  //是否在連線狀態
String msg;
PACKET pkg;
String strPkg;
String srtClient;
String SendCard = "";
String AllCard = "";
int clientNum;
int chips = 100;
int sec = 10;
int player_num = 0;
int people_num;
int player_cards[4][5];
int player_disconnet[4];
int bets[4];
int max;
int min;
String player_host_name[4];
bool start_game = false;
bool All_bet = false;
bool bet_TF = false;
int bet1 = 100;
int bet2 = 100;
int bet3 = 100;
int bet4 = 100;
int money1 = 0;
int money2 = 0;
int money3 = 0;
int money4 = 0;
int points1;
int points2;
int points3;
int points4;
String state1;
String state2;
String state3;
String state4;

/*String suitToString(Suit s)  // 函數用於將花色轉換成字串表示
{
	switch (s)
	{
		case CLUBS:
			return L"club";
		case SPADES:
			return L"spade";
		case HEARTS:
			return L"heart";
		case DIAMONDS:
			return L"diamond";
        default:
            return L"";
	}
}*/
//---------------------------------------------------------------------------
void DeleteAllImagesFromPanel(TPanel* panel)
{
	try
	{
	for (int i = 0; i < panel->ControlCount; ++i)
	{
		TControl *control = panel->Controls[i];
		if (dynamic_cast<TImage*>(control)) // 檢查控件是否是 TImage
		{
			delete control; // 刪除 TImage 控件
			i--; // 由於刪除了一個控件，需要調整迴圈索引
		}
	}
	}
	catch(...)
	{
		Form2->Memo1->Lines->Add("oh on1");
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1:: Disconnect_interface(int Num)
{
	//ShowMessage(Num);
	if(Num == 0)
	{
		Panel1->Caption = "已斷線！";
		Label1->Visible = false;
		Label2->Visible = false;
		Label3->Visible = false;
		player_disconnet[0] = 0;
		DeleteAllImagesFromPanel(Panel1);
	}
	else if(Num == 1)
	{
		Panel2->Caption = "已斷線！";
		Label4->Visible = false;
		Label5->Visible = false;
		Label6->Visible = false;
		player_disconnet[1] = 0;
		DeleteAllImagesFromPanel(Panel2);
	}
	else if(Num == 2)
	{
		Panel3->Caption = "已斷線！";
		Label7->Visible = false;
		Label8->Visible = false;
		Label9->Visible = false;
		player_disconnet[2] = 0;
		DeleteAllImagesFromPanel(Panel3);
	}
	else if(Num == 3)
	{
		Panel4->Caption = "已斷線！";
		Label10->Visible = false;
		Label11->Visible = false;
		Label12->Visible = false;
		player_disconnet[3] = 0;
		DeleteAllImagesFromPanel(Panel4);
	}
}
//---------------------------------------------------------------------------
void TForm1::displayPlayerCards(TPanel* playerPanel, const TCard hand[5], int player_num)// 顯示玩家的撲克牌在對應的 Panel 上
{
	int cardIndex = 0;
	String cardImageName;

	for (int card = 0; card < 5; ++card)
	{
		TImage* cardImage = new TImage(playerPanel);
		cardImage->Parent = playerPanel;
		cardImage->Left = card * 50;

		/*String suitImage = suitToString(hand[card].suit);
		String valueImage = IntToStr(hand[card].value);
		cardImage->Picture->LoadFromFile("pic\\"+suitImage+"_"+valueImage+".jpg");*/

		//cardIndex = (hand[card].value - 1) * 4 + static_cast<int>(hand[card].suit) + 1;
		//String cardImageName = "pic\\" + IntToStr(cardIndex) + ".bmp";
		//cardImage->Picture->LoadFromFile(cardImageName);
		//SendCard += IntToStr(cardIndex) + "、";

		if(player_cards[player_num][card] < 10)
			cardImageName = "pic\\0" + IntToStr(player_cards[player_num][card]) + ".bmp";
		else
			cardImageName = "pic\\" + IntToStr(player_cards[player_num][card]) + ".bmp";
		//Memo1->Lines->Add(player_cards[player_num][card]);
		cardImage->Picture->LoadFromFile(cardImageName);
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Licensing() // 洗牌顯示牌況
{
	// 分發給四個玩家
	TCard playerHands[4][5];
	int cardIndex = 0;
	TStringList *cards = new TStringList();

	srand(time(0));
	Memo1->Clear();
	clientNum=ServerSocket1->Socket->ActiveConnections;

	// 生成所有撲克牌，每種花色有13張牌
	TCard deck[52];
	int index = 0;
	for (int i = 1; i <= 13; ++i)
	{
		deck[index++] = {CLUBS, i};
		deck[index++] = {SPADES, i};
		deck[index++] = {HEARTS, i};
		deck[index++] = {DIAMONDS, i};
	}

	// 洗牌
	for (int i = 51; i > 0; --i) {
		int j = rand() % (i + 1);
		TCard temp = deck[i];
		deck[i] = deck[j];
		deck[j] = temp;
	}

	index = 0;
	for (int card = 0; card < 5; ++card)
	{
		for (int player = 0; player < 4; ++player)
		{
			playerHands[player][card] = deck[index++];
			cardIndex = (playerHands[player][card].value - 1) * 4 + static_cast<int>(playerHands[player][card].suit) + 1;

			if(cardIndex < 10)
				SendCard += + "0" + IntToStr(cardIndex) + "、";
			else
				SendCard += IntToStr(cardIndex) + "、";
		}
	}

	pkg.ID = "CHAT";
	pkg.Host=IdIPWatch1->LocalIP();
	pkg.separation = "#";
	pkg.MainFunc = 2; // 傳送資料
	pkg.SubFunc = 3; // 傳送全體玩家手牌
	pkg.Msg = SendCard;
	pkg.MsgLength = pkg.Msg.Length();
	pkg.EndSymbol = "@";
	strPkg = AssemblePacket(pkg);
	for (int i = 0; i < ServerSocket1->Socket->ActiveConnections; i++)
		ServerSocket1->Socket->Connections[i]->SendText(strPkg);

	cards->Delimiter = L'、';
	cards->DelimitedText = SendCard;

	int index_sort = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 5; j++)
			player_cards[i][j] = cards->Strings[index_sort++].ToInt();

	for (int k = 0; k < 4; k++)	// 泡沫排序
		for (int i = 0; i < 5 - 1; i++)
			for (int j = 0; j < 5 - i - 1; j++)
			{
				if(get_card_num(player_cards[k][j]) > get_card_num(player_cards[k][j + 1]))
				{
					int temp = player_cards[k][j];
					player_cards[k][j] = player_cards[k][j + 1];
					player_cards[k][j + 1] = temp;
				}
			}
	delete cards;

	/*if(clientNum == 1)
	{
		displayPlayerCards(Panel1, playerHands[0], 0);
	}
	else if(clientNum == 2)
	{
		displayPlayerCards(Panel1, playerHands[0], 0);
		displayPlayerCards(Panel2, playerHands[1], 1);
	}
	else if(clientNum == 3)
	{
        displayPlayerCards(Panel1, playerHands[0], 0);
		displayPlayerCards(Panel2, playerHands[1], 1);
		displayPlayerCards(Panel3, playerHands[2], 2);
	}
	else
	{
		displayPlayerCards(Panel1, playerHands[0], 0);
		displayPlayerCards(Panel2, playerHands[1], 1);
		displayPlayerCards(Panel3, playerHands[2], 2);
		displayPlayerCards(Panel4, playerHands[3], 3);
	}*/
	/*for (int i = 0; i < 4; i++)
	{
		if(player_disconnet[i])
		{
			displayPlayerCards(Panel1, playerHands[i], i);
		}
	}*/
	for (int i = 0; i < 4; i++)
	{
		//Form2->Memo1->Lines->Add("!!!!!!!!!!!!!!");
		//Form2->Memo1->Lines->Add(player_disconnet[i]);
		if (player_disconnet[i] == 1)
		{
			switch (i)
			{
				case 0:
					displayPlayerCards(Panel1, playerHands[i], i);
					break;
				case 1:
					displayPlayerCards(Panel2, playerHands[i], i);
					break;
				case 2:
					displayPlayerCards(Panel3, playerHands[i], i);
					break;
				case 3:
					displayPlayerCards(Panel4, playerHands[i], i);
					break;
				default:
					// Handle additional players
					break;
			}
		}
	}

	who_win(4);

	Button2->Enabled = false;
	Button3->Enabled = false;
	Button4->Enabled = false;
	Button5->Enabled = false;
}
//---------------------------------------------------------------------------
int TForm1::get_card_num(int cardIndex)
{
	int suitIndex = cardIndex / 13;
	int value = cardIndex % 13;

	if(value == 0)
		value = 13;

	return value;
}
//---------------------------------------------------------------------------
int TForm1::To_simple(int card)
{
	int value = card % 13;

	if(value > 10)
		value = 10;

	return value;
}
//---------------------------------------------------------------------------
String TForm1::compareCards(int player_num) // 五張牌比大小
{
	int All_total = 0;
	int total = 0;
	String state;

	for(int i = 0; i < 5; i++)
		All_total += To_simple(player_cards[player_num][i]);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = i + 1; j < 4; ++j)
		{
			for (int k = j + 1; k < 5; ++k)
			{
				total = To_simple(player_cards[player_num][i]) +
						To_simple(player_cards[player_num][j]) +
						To_simple(player_cards[player_num][k]);

				//Memo1->Lines->Add(To_simple(player_cards[player_num][i]));
				//Memo1->Lines->Add(To_simple(player_cards[player_num][j]));
				//Memo1->Lines->Add(To_simple(player_cards[player_num][k]));
				//ShowMessage(total);
				if(All_total == 50)
				{
					state = "五公";
                    return state;
				}
				if(total % 10 == 0)
				{
					//ShowMessage("All_total" + IntToStr(All_total));
					if((All_total - total) % 10 == 0)
					{
						state = "妞妞";
						return state;
					}
					else
					{
						//ShowMessage("All_total - total" + IntToStr(All_total - total));
						if(All_total - total > 10)
						{
							state = IntToStr((All_total - total) % 10);
							//ShowMessage("147" + state);
							return state;
						}
						else
						{
							state = IntToStr(All_total - total);
							//ShowMessage("258" + state);
							return state;
						}
					}
				}
				else
					state = "無妞";
			}
		}
	}
	return state;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bet(String bet) // 下注
{
	pkg.ID="CHAT";

	if(blnServer)
		pkg.Host=IdIPWatch1->LocalIP();
	else
		pkg.Host=ClientHostName;

	 pkg.separation="#";
	 pkg.MainFunc=2; //傳送資料
	 pkg.SubFunc=2;
	 pkg.Msg=IntToStr(player_num) + bet;
	 pkg.MsgLength=pkg.Msg.Length();
	 pkg.EndSymbol="@";
	 strPkg=AssemblePacket(pkg);

	ClientSocket1->Socket->SendText(strPkg);
	bet_TF = true;

	Button2->Enabled = false;
	Button3->Enabled = false;
	Button4->Enabled = false;
	Button5->Enabled = false;
}
//---------------------------------------------------------------------------
int TForm1::getSuitIndex(int card)
{
	int cardValue = To_simple(card);
    if (cardValue >= 1 && cardValue <= 13) {
        return 0; // 梅花
    } else if (cardValue >= 14 && cardValue <= 26) {
        return 1; // 菱形
    } else if (cardValue >= 27 && cardValue <= 39) {
        return 2; // 愛心
    } else if (cardValue >= 40 && cardValue <= 52) {
        return 3; // 黑桃
    }
    return -1; // 無法識別的花色
}
//---------------------------------------------------------------------------
void __fastcall TForm1::who_win(int clientNum) // 判斷誰贏
{
	int temp_point;
	int maxPointsIndex  = 0;
	int maxTempPoint = 0;
	int maxPoints = 0;
	int temp[clientNum];
	String temp_str;
    // 儲存每個連接的下注狀態
	int bets[4] = {bet1, bet2, bet3, bet4};

	//Form2->Memo1->Lines->Add("-+-+-+-" + clientNum);
    //Timer3->Enabled = false;

	for(int i = 0; i < clientNum; i++)
	{
		temp_str = compareCards(i);
		Form2->Memo1->Lines->Add("temp_str" + temp_str);
		if(!player_disconnet[i])
		{
			temp_point = -1;
		}
		else
		{
			if(temp_str == L"五公")
				temp_point = 4;
			else if(temp_str == L"妞妞")
				temp_point = 3;
			else if(temp_str == L"無妞")
				temp_point = 1;
			else
				temp_point = 2;
		}
		if(i == 0)
		{
			Label3->Caption = "點數：" +  compareCards(i);
			points1 = temp_point;
			state1 = compareCards(i);
			if(points1 == 2)
				temp[i] = StrToInt(compareCards(i));
			Form2->Memo1->Lines->Add("***4***" + IntToStr(points1));
		}
		else if(i == 1)
		{
			Label6->Caption = "點數：" +  compareCards(i);
			points2 = temp_point;
			state2 = compareCards(i);
			if(points2 == 2)
				temp[i] = StrToInt(compareCards(i));
			Form2->Memo1->Lines->Add("***4***" + IntToStr(points2));
		}
		else if(i == 2)
		{
			Label9->Caption =  "點數：" +  compareCards(i);
			points3 = temp_point;
			state3 = compareCards(i);
			if(points3 == 2)
				temp[i] = StrToInt(compareCards(i));
			Form2->Memo1->Lines->Add("***4***" + IntToStr(points3));
		}
		else if(i == 3)
		{
			Label12->Caption = "點數：" +  compareCards(i);
			points4 = temp_point;
			state4 = compareCards(i);
			if(points4 == 2)
				temp[i] = StrToInt(compareCards(i));
			Form2->Memo1->Lines->Add("***4***" + IntToStr(points4));
		}


		/*if(temp_point > maxPoints || (temp_point == maxPoints && temp[i] > maxTempPoint))
		{
			maxPoints = temp_point;
			maxTempPoint = temp[i];
			maxPointsIndex = i;
		}*/
		if (temp_point > maxPoints || (temp_point == maxPoints && temp[i] > maxTempPoint) ||
		 (temp_point == maxPoints && temp[i] == maxTempPoint &&
		  getSuitIndex(player_cards[i][4]) > getSuitIndex(player_cards[maxPointsIndex][4]))) {
			maxPoints = temp_point;
			maxTempPoint = temp[i];
			maxPointsIndex = i;
		}
	}

	// 更新對應的籌碼
	switch (maxPointsIndex) {
		case 0:
			bet1 += money2 + money3 + money4;
			bets[0] = bet1;
			Label1->Caption = L"籌碼：" + IntToStr(bet1);
			Memo1->Lines->Add("本局玩家為Palyer1！");
			//Form2->Memo1->Lines->Add("bet1：" + IntToStr(bet1));

			break;
		case 1:
			bet2 += money1 + money3 + money4;
			bets[1] = bet2;
			Label4->Caption = L"籌碼：" + IntToStr(bet2);
			Memo1->Lines->Add("本局玩家為Palyer2！");
			//Form2->Memo1->Lines->Add("bet2：" + IntToStr(bet2));
			break;
		case 2:
			bet3 += money1 + money2 + money4;
			bets[2] = bet3;
			Label7->Caption = L"籌碼：" + IntToStr(bet3);
			Memo1->Lines->Add("本局玩家為Palyer3！");
			//Form2->Memo1->Lines->Add("bet3：" + IntToStr(bet3));
			break;
		case 3:
			bet4 += money1 + money2 + money3;
			bets[3] = bet4;
			Label10->Caption = L"籌碼：" + IntToStr(bet4);
			Memo1->Lines->Add("本局玩家為Palyer4！");
			//Form2->Memo1->Lines->Add("bet4：" + IntToStr(bet4));
			break;
		default:
			break;
	}
	Form2->Memo1->Lines->Add("---1---" + state1);
	Form2->Memo1->Lines->Add("---2---" + state2);
	Form2->Memo1->Lines->Add("---3---" + state3);
	Form2->Memo1->Lines->Add("---4---" + state4);

	Timer4->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::initialize()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 5; ++j)
			player_cards[i][j] = 0;

	DeleteAllImagesFromPanel(Panel1);
	DeleteAllImagesFromPanel(Panel2);
	DeleteAllImagesFromPanel(Panel3);
	DeleteAllImagesFromPanel(Panel4);
    SendCard = "";

	money1 = 0;
	money2 = 0;
	money3 = 0;
	money4 = 0;

	Label3->Caption = "點數：";
	Label6->Caption = "點數：";
	Label9->Caption = "點數：";
	Label12->Caption = "點數：";

	Button2->Visible = true;
	Button3->Visible = true;
	Button4->Visible = true;
	Button5->Visible = true;

	Timer3->Enabled = false;
	Timer4->Enabled = false;
	Memo1->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::reset()
{
	initialize();
	Label1->Caption = "籌碼：100";
	Label4->Caption = "籌碼：100";
	Label7->Caption = "籌碼：100";
	Label10->Caption = "籌碼：100";
	Label2->Caption = "賭注：";
	Label5->Caption = "賭注：";
	Label8->Caption = "賭注：";
	Label11->Caption = "賭注：";
	Label1->Visible = true;
	Label2->Visible = true;
	Label3->Visible = true;
	Label4->Visible = true;
	Label5->Visible = true;
	Label6->Visible = true;
	Label7->Visible = true;
	Label8->Visible = true;
	Label9->Visible = true;
	Label10->Visible = true;
	Label11->Visible = true;
	Label12->Visible = true;
    Start_Game->Enabled = false;
	Panel1->Caption = "Player1";
	Panel2->Caption = "Player2";
	Panel3->Caption = "Player3";
	Panel4->Caption = "Player4";
    sec = 10;
	bet1 = 100;
	bet2 = 100;
	bet3 = 100;
	bet4 = 100;
	state1 = "";
	state2 = "";
	state3 = "";
	state4 = "";
	start_game = false;
    Button2->Enabled = false;
	Button3->Enabled = false;
	Button4->Enabled = false;
	Button5->Enabled = false;
    //last_one = false;
	//Form2->Memo1->Clear();
	Form1->Caption="Local host: "+IdIPWatch1->LocalIP();
	for (int i = 0; i < 4; i++)
		player_disconnet[i] = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::play_Game()
{
	pkg.ID="CHAT";

	if(blnServer)
	 pkg.Host=IdIPWatch1->LocalIP();
	else
	 pkg.Host=ClientHostName;

	start_game = true;
	Timer3->Enabled = true;

	for(int i=0;i<ServerSocket1->Socket->ActiveConnections;i++)
	{
		pkg.separation="#";
		pkg.MainFunc=2; //傳送資料
		pkg.SubFunc=1;
		pkg.Msg=i;
		pkg.MsgLength=pkg.Msg.Length();
		pkg.EndSymbol="@";
		strPkg=AssemblePacket(pkg);
		ServerSocket1->Socket->Connections[i]->SendText(strPkg);
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::SendMsg(TObject *Sender)
{
   pkg.ID="CHAT";

   if(blnServer)
	 pkg.Host=IdIPWatch1->LocalIP();
   else
	 pkg.Host=ClientHostName;

   pkg.separation="#";
   pkg.MainFunc=2; //傳送資料
   pkg.SubFunc=0;
   pkg.MsgLength=Memo1->Lines->Strings[Memo1->Lines->Count-1].Length();
   pkg.Msg=Memo1->Lines->Strings[Memo1->Lines->Count-1];
   pkg.EndSymbol="@";
   strPkg=AssemblePacket(pkg);

	if (blnServer)
	{
		int index;

		if(ComboBox1->Text=="All") //brocaste
		{
			for(int i=0;i<ServerSocket1->Socket->ActiveConnections;i++)
			  ServerSocket1->Socket->Connections[i]->SendText(strPkg);
		}
		else // specific client
		{
			for(index=0;index<ServerSocket1->Socket->ActiveConnections;index++)
			{
				srtClient=ServerSocket1->Socket->Connections[index]->RemoteAddress +
						 "-"+IntToStr(ServerSocket1->Socket->Connections[index]->SocketHandle);
				if(ComboBox1->Text==srtClient)
				  break;
			}

			ServerSocket1->Socket->Connections[index]->SendText(strPkg);
		}
	}
	else // Client
		ClientSocket1->Socket->SendText(strPkg) ;

}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListenClick(TObject *Sender)
{
	PACKET pkg;
	String strPkg;

	Form1->Caption="I am a server: "+IdIPWatch1->LocalIP();

	Listen->Checked = !Listen->Checked;


	if (Listen->Checked)
	{
		ClientSocket1->Active = false;

		try
		{
			ServerSocket1->Active = true;
		}
		catch(...)
		{
			Listen->Checked=false;
			ShowMessage("Be a server failure.");
		return;
		}

		ServerName=IdIPWatch1->LocalIP();
		StatusBar1->SimpleText="Status: Chat Server Listening...";
		Connect->Enabled=false;
		Button2->Visible = false;
		Button3->Visible = false;
		Button4->Visible = false;
		Button5->Visible = false;
		Form2->Memo1->Lines->Add(ServerName);
	}
	else
	{
		if (ServerSocket1->Active)
		{
			// build packet
			pkg.ID="CHAT";
			pkg.MainFunc=1;
			pkg.SubFunc=2;
			pkg.MsgLength=0;
			pkg.Msg="";
			pkg.separation="#";
			pkg.EndSymbol="@";
			pkg.Host=ServerName;

			strPkg=AssemblePacket(pkg);
			for(int i=0;i<ServerSocket1->Socket->ActiveConnections;i++)
			   ServerSocket1->Socket->Connections[i]->SendText(strPkg);

			ServerSocket1->Active = false;
			reset();
		}
		Connect->Enabled=true;

		StatusBar1->SimpleText="Status: Chat Server Close";
	}

	blnServer = true;
	Form1->Tag=0; //表示為server
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ConnectClick(TObject *Sender)
{
	Form1->Caption="I am a client";

	if (ClientSocket1->Active)
	ClientSocket1->Active = false;

	strHost="127.0.0.1";
	if (InputQuery("Chat Connection", "Chat Server IP: ", strHost))
	{
		if (strHost.Length() > 0)
		{
			ClientSocket1->Host = strHost;

			ClientSocket1->Active = true;
			Timer1->Enabled=true;

			Listen->Checked = false;
			Disconnect->Enabled = false;
			blnServer = false ;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DisconnectClick(TObject *Sender)
{
	PACKET pkg;
	String strPkg;

	if(blnServer) //此功能不是給server用的
	return;
	player_num = StrToInt(Form1->Caption.SubString(Form1->Caption.Length(),1));

	// build packet
	pkg.ID="CHAT";
	pkg.MainFunc=1;
	pkg.SubFunc=2;
	pkg.Msg=player_num;
	pkg.MsgLength=pkg.Msg.Length();
	pkg.separation="#";
	pkg.EndSymbol="@";
	pkg.Host=ClientHostName;

	strPkg=AssemblePacket(pkg);

	ClientSocket1->Socket->SendText(strPkg);
	ClientSocket1->Active = false;

	//Memo2->Lines->Add("["+Time()+"] Disconnect.");
	Listen->Checked = false;
	Connect->Checked = false;
	Disconnect->Checked = false;

	fgConnectState=false;
	//Memo2->Visible=false;

	StatusBar1->SimpleText="Status: Disconnect" ;
	Form1->Caption="";
	reset();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Start_GameClick(TObject *Sender)
{
	play_Game();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ClientSocket1Connect(TObject *Sender, TCustomWinSocket *Socket)

{
	Timer1->Enabled=false;

	StatusBar1->SimpleText== "Status: Connect to " + Socket->RemoteHost ;
	fgConnectState=true;
	//Memo2->Clear();
	//Memo2->Visible=true;

	Listen->Enabled=false;
	Connect->Enabled=false;
	Sleep(1000);
	Disconnect->Enabled=true;

	ClientSocket1->ClientType=ctNonBlocking;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ClientSocket1Disconnect(TObject *Sender, TCustomWinSocket *Socket)

{
	Timer1->Enabled=false;
	StatusBar1->SimpleText="server disconnected";
	fgConnectState=false;
	reset();
	//Memo2->Visible=false;

	Listen->Enabled=true;
	Disconnect->Enabled=false;
	Connect->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ClientSocket1Error(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode)
{
	//Memo2->Lines->Add("Error: " + strHost);
	ErrorCode = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClientSocket1Read(TObject *Sender, TCustomWinSocket *Socket)

{
    pPACKET pPkg;
	String strPacket;

	if(!fgConnectState)
	return;

	strPacket=Socket->ReceiveText();
	Form2->Memo1->Lines->Add(strPacket);

	TStringList *sList = new TStringList();
	sList->Delimiter = '@'; // 設定要分割的字元
	sList->DelimitedText = strPacket;

	if(sList->Count-1 > 1)
	{
        for(int i = 0; i < sList->Count-1; i++)
		{
			AnsiString subStr = sList->Strings[i] + "@"; // 去除空白字符

			Form2->Memo1->Lines->Add(subStr);
			pPkg=DisassemblePacket(subStr);
			ParsePacket(pPkg);
		}
		delete sList;
	}
	else
	{
        pPkg=DisassemblePacket(strPacket);
		if(pPkg==NULL)
		{
			Memo1->Lines->Add("Packet error");
			return;
		}
		ParsePacket(pPkg);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ServerSocket1Accept(TObject *Sender, TCustomWinSocket *Socket)

{
	fgConnectState=true;
	//Memo2->Visible=true;

	ReflashClientList();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ServerSocket1ClientConnect(TObject *Sender, TCustomWinSocket *Socket)

{
    PACKET pkg;
	String strPkg;

	clientNum=ServerSocket1->Socket->ActiveConnections;

	if(clientNum>CLIENT_MAX)
	{
		pkg.ID="CHAT";
		pkg.Host=ServerName;
		pkg.separation="#";
		pkg.MainFunc=1;
		pkg.SubFunc=3;
		pkg.Msg=newClient;
		pkg.MsgLength=pkg.Msg.Length();
		pkg.EndSymbol="@";
		strPkg=AssemblePacket(pkg);

		ServerSocket1->Socket->Connections[4]->SendText(strPkg);

		Form2->Memo1->Lines->Add("已達連線最大數量");
		Socket->Close();
		return;
	}

	StatusBar1->SimpleText= "Status: Connect from " + Socket->RemoteHost+
						  " socket ID: "+ IntToStr(Socket->SocketHandle);

	if(start_game)  // 如果遊戲正在進行中
	{
		pkg.ID="CHAT";
		pkg.Host=ServerName;
		pkg.separation="#";
		pkg.MainFunc=1; //傳送資料
		pkg.SubFunc=4;
		pkg.Msg=newClient;
		pkg.MsgLength=newClient.Length();
		pkg.EndSymbol="@";
		strPkg=AssemblePacket(pkg);

		ServerSocket1->Socket->Connections[clientNum-1]->SendText(strPkg);

		Socket->Close();
		return;
	}

	if(!CheckClients(Socket)) //new connection
	{
		pkg.ID="CHAT";
		pkg.Host=ServerName;
		pkg.separation="#";
		pkg.MainFunc=1; //新連線
		pkg.SubFunc=1;  //設定client的名稱
		pkg.Msg=newClient;
		pkg.MsgLength=newClient.Length();
		pkg.EndSymbol="@";
		strPkg=AssemblePacket(pkg);

		ServerSocket1->Socket->Connections[clientNum-1]->SendText(strPkg) ;
		player_disconnet[clientNum-1] = true;
		player_host_name[clientNum-1] = newClient.SubString(newClient.Length()-1,3);
		//Memo1->Lines->Add(player_host_name[clientNum-1]);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServerSocket1ClientDisconnect(TObject *Sender, TCustomWinSocket *Socket)

{
	String Host = IntToStr(Socket->SocketHandle);
	int temp_num = -1;

	Form2->Memo1->Lines->Add("'"+Socket->RemoteAddress+"-"+IntToStr(Socket->SocketHandle)+"' disconnection comfirmed");

	if(start_game)
	{
		for (int i = 0; i < 4; i++)
			if(Host.SubString(Host.Length()-1,3) == player_host_name[i])
				temp_num = i;

		pkg.ID="CHAT";
		pkg.Host=IdIPWatch1->LocalIP();
		pkg.separation="#";
		pkg.MainFunc=1; //傳送資料
		pkg.SubFunc=5;
		pkg.Msg=temp_num;
		pkg.MsgLength=pkg.Msg.Length();
		pkg.EndSymbol="@";
		strPkg=AssemblePacket(pkg);

		for(int i=0;i<ServerSocket1->Socket->ActiveConnections;i++)
			ServerSocket1->Socket->Connections[i]->SendText(strPkg);

		Disconnect_interface(temp_num);
		Start_Game->Enabled = false;
	}

	StatusBar1->SimpleText = "Status: Listening...";

	if(ServerSocket1->Socket->ActiveConnections==1)
	{
		fgConnectState=false;
		//Memo2->Visible=false;
	}

	ComboBox1->Enabled=false;
	Timer2->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServerSocket1ClientError(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode)
{
	/*if(ErrorCode == 10053 || ErrorCode == 10054)
	{
		Sleep(1000);
		//ShowMessage(ErrorCode);
		//ShowMessage("!!1!!");
	}*/

	StatusBar1->SimpleText="Error from Client socket.";
	ReflashClientList();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServerSocket1ClientRead(TObject *Sender, TCustomWinSocket *Socket)

{
    pPACKET pPkg;
	String strPacket;

	strPacket=Socket->ReceiveText();
	Form2->Memo1->Lines->Add(strPacket);

	pPkg=DisassemblePacket(strPacket);
	if(pPkg==NULL)
	{
		Memo1->Lines->Add("Packet error");
		return;
	}

	ParsePacket(pPkg);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    randomize();

	fgConnectState=false;

	Disconnect->Enabled=false;

	Form1->Caption="Local host: "+IdIPWatch1->LocalIP();
}
//---------------------------------------------------------------------------
bool TForm1::CheckClients(TCustomWinSocket *Socket)
{
	bool fg;
	int sHnd;
	String hostIP;
	int clientNum;

	clientNum=ServerSocket1->Socket->ActiveConnections;

	hostIP=Socket->RemoteAddress;
	sHnd=Socket->SocketHandle;

	newClient= hostIP+"-"+IntToStr(sHnd);
	Form2->Memo1->Lines->Add("New connection: "+newClient);
	Form2->Memo1->Lines->Add("Connection number:"+
						   IntToStr(ServerSocket1->Socket->ActiveConnections));
	if(clientNum > 3)
		Start_Game->Enabled = true;

	fg=false;
	return fg;
}

//-------------------------------------------------------
String TForm1::AssemblePacket(PACKET pkg)
{
	String str="",tmp;

	//------------- 檔頭 ----------
	if(pkg.ID.Length()!=4)
	 {
		 ShowMessage("檔頭錯誤");
		 return NULL;
	 }
	else
	 str+=pkg.ID;

	//------------ host ----------
	if(pkg.Host.Length()==0)
	{
		 ShowMessage("host錯誤");
		 return NULL;
	 }
	else
	{
		 str+=pkg.Host;
		 str+=pkg.separation;
	}

	//------------ main function ----------
	str+=pkg.MainFunc;

	//------------ sub function ----------
	str+=pkg.SubFunc;

	//---------- mdg length ------------
	tmp=IntToStr(pkg.MsgLength);
	if(tmp.Length()>3)
	{
		ShowMessage("msg長度紀錄錯誤");
		return NULL;
	}
	else
	{
	   for(int i=0;i<3- tmp.Length(); i++)
		 str+="0";

		str+=tmp;
	}

	//---------------- message ------------
	if(pkg.MsgLength != pkg.Msg.Length())
	{
		ShowMessage("msg長度錯誤");
		return NULL;
	}
	else
	{
		str+=pkg.Msg;
		str+=pkg.EndSymbol;
	}
	return str;
}

//----------------------------------------------------
pPACKET TForm1::DisassemblePacket(String strPkg)
{
	PACKET tmpPkg;
	pPACKET pPkg;
	int separator;
	String strTmp;

	Form2->Memo1->Lines->Add("===== Diassemble packet ====");

	if(strPkg[strPkg.Length()]!='@')
	{
		Form2->Memo1->Lines->Add("Packet error 1");
		return NULL;
	}

	if(strPkg.Pos("#")<=0)
	{
		Form2->Memo1->Lines->Add("Packet error 2");
		return NULL;
	}

	if(strPkg.SubString(1,4)!="CHAT")
	{
		Form2->Memo1->Lines->Add("Packet error 3");
		return NULL;
	}

	//---------------------------------------
	tmpPkg.ID=strPkg.SubString(1,4);

	separator= strPkg.Pos("#");
	tmpPkg.Host=strPkg.SubString(5,separator-5);
	tmpPkg.separation=strPkg.SubString(separator,1);

	tmpPkg.MainFunc=strPkg.SubString(separator+1,1);
	tmpPkg.SubFunc=strPkg.SubString(separator+2,1);

	//-----------------------------------------
	strTmp=strPkg.SubString(separator+3,3);
	try
	{
		tmpPkg.MsgLength=StrToInt(strTmp);
	}
	catch(...)
	{
		Form2->Memo1->Lines->Add("Packet error 4");
		return NULL;
	}

	if(tmpPkg.MsgLength<0)
	{
		Form2->Memo1->Lines->Add("Packet error 5");
		return NULL;
	}

	strTmp=strPkg.SubString(separator+6,strPkg.Length()-separator-6);
	if(strTmp.Length()!=tmpPkg.MsgLength)
	{
		Form2->Memo1->Lines->Add("Packet error 6");
		return NULL;
	}
	else
		tmpPkg.Msg=strTmp;

	pPkg=new PACKET;
	pPkg->ID=tmpPkg.ID;
	pPkg->Host=tmpPkg.Host;
	pPkg->separation=tmpPkg.separation;
	pPkg->MainFunc=tmpPkg.MainFunc;
	pPkg->SubFunc=tmpPkg.SubFunc;
	pPkg->MsgLength=tmpPkg.MsgLength;
	pPkg->Msg=tmpPkg.Msg;
	pPkg->EndSymbol=tmpPkg.EndSymbol;

	return pPkg;

}
//---------------------------------------------------------------------------
void TForm1::ParsePacket(pPACKET pPkg)
{
	int mFunc,sFunc;
	int money, num;
	TStringList *cards = new TStringList();
	TCard playerHands[4][5];
	int people_num;
	int count = 4;
	//int player_disconncet[4] = {0, 0, 0, 0};

	mFunc=StrToInt(pPkg->MainFunc);
	sFunc=StrToInt(pPkg->SubFunc);

	switch(mFunc)
	{
		case 1: //get a new name
			 switch(sFunc)
			   {

					case 1: //new connection
						 ClientHostName=pPkg->Msg;
						 Form2->Memo1->Lines->Add("New name: "+ClientHostName);
						 Form1->Caption=ClientHostName;
						 break;
					case 2: // Disconnect
						 if(blnServer)
						 {
							Form2->Memo1->Lines->Add(Time()+" Client'"+
							pPkg->Host+"' requests to disconnect.");
							if(start_game)
							{

								player_num = StrToInt(pPkg->Msg.SubString(pPkg->Msg.Length(),1));
								ShowMessage("玩家" + IntToStr(player_num+1) + "已斷線");
								//Memo1->Lines->Add("玩家" + IntToStr(player_num+1) + "已斷線");
								Disconnect_interface(player_num);

								pkg.ID="CHAT";
								pkg.Host=IdIPWatch1->LocalIP();
								pkg.separation="#";
								pkg.MainFunc=1; //傳送資料
								pkg.SubFunc=5;
								pkg.Msg=player_num;
								pkg.MsgLength=pkg.Msg.Length();
								pkg.EndSymbol="@";
								strPkg=AssemblePacket(pkg);
								//ClientSocket1->Socket->SendText(strPkg);

								for(int i=0;i<ServerSocket1->Socket->ActiveConnections;i++)
									ServerSocket1->Socket->Connections[i]->SendText(strPkg);
							}
						 }
						 else
						 {
						   Form2->Memo1->Lines->Add(Time()+" Server'"+
						   pPkg->Host+"' disconnected.");
						   initialize();
						 }
						 break;
					case 3: // 已達連線上限
						MessageDlg("已達連線最大數量！", mtInformation, TMsgDlgButtons() << mbYes, 0);
						break;
					case 4: // 伺服器遊戲中
						MessageDlg("遊戲正在進行中，請稍後在試！", mtInformation, TMsgDlgButtons() << mbYes, 0);
						break;
					case 5: // 玩家斷線通知
						//ShowMessage("玩家" + IntToStr(StrToInt(pPkg->Msg)+1) + "已斷線");
						//Memo1->Lines->Add("玩家" + IntToStr(StrToInt(pPkg->Msg)+1) + "已斷線");
						player_disconnet[StrToInt(pPkg->Msg.SubString(pPkg->Msg.Length(),1))] = 0;
						for(int i = 0; i < 4;i++)
							if(!player_disconnet[i])
								count--;
						if(count == 1)
						{
							//ShowMessage("恭喜獲得第一名！");
							Memo1->Lines->Add("恭喜獲得第一名！");
                            Timer3->Enabled = false;
							return;
						}
						Disconnect_interface(StrToInt(pPkg->Msg));
						//ShowMessage(player_disconnet[StrToInt(pPkg->Msg.SubString(pPkg->Msg.Length(),1))]);
						break;
			   }
			 break;

		case 2:
			switch(sFunc)
			   {
					case 1: // 遊戲開始
						Timer3->Enabled = true;
						Button2->Enabled = true;
						Button3->Enabled = true;
						Button4->Enabled = true;
						Button5->Enabled = true;

						if(!start_game)
						{
                            for (int i = 0; i < 4; i++)
								player_disconnet[i] = true;

							player_num = StrToInt(pPkg->Msg.SubString(pPkg->Msg.Length(),1));
							if(player_num == 0)
							{
								Panel1->Caption = "yourself";
								Form1->Caption=ClientHostName + IntToStr(player_num);

							}
							else if(player_num == 1)
							{
								Panel2->Caption = "yourself";
								Form1->Caption=ClientHostName + IntToStr(player_num);
							}
							else if(player_num == 2)
							{
								Panel3->Caption = "yourself";
								Form1->Caption=ClientHostName + IntToStr(player_num);
							}
							else
							{
								Panel4->Caption = "yourself";
								Form1->Caption=ClientHostName + IntToStr(player_num);
							}
						}
                        start_game = true;

						 break;
					case 2: // 傳送下注金額
						if(pPkg->MsgLength == 4)
							money = 100;
						else
                            money = StrToInt(pPkg->Msg.SubString(pPkg->Msg.Length()-1,2));
						chips -= money;
						//Memo1->Lines->Add(pPkg->Host + "下注：" + IntToStr(money));

						pkg.ID="CHAT";

						if(blnServer)
						 pkg.Host=IdIPWatch1->LocalIP();
						else
						 pkg.Host=ClientHostName;

						 pkg.separation="#";
						 pkg.MainFunc=2; //傳送資料
						 pkg.SubFunc=2;
						 pkg.Msg=pPkg->Msg;
						 pkg.MsgLength=pkg.Msg.Length();
						 pkg.EndSymbol="@";
						 strPkg=AssemblePacket(pkg);

						for(int i=0;i<ServerSocket1->Socket->ActiveConnections;i++)
							ServerSocket1->Socket->Connections[i]->SendText(strPkg);


						num = StrToInt(pPkg->Msg.SubString(pPkg->Msg.Length()-3,1));
						//Memo1->Lines->Add(player_num);

						try
						{
							//if(blnServer)
							{
								//ShowMessage(ServerSocket1->Socket->Connections[1]);
								if(num == 0)
								{
									money1 = money;
									bet1 -= money1;
									bets[0] = bet1;
									if(bet1 < 0 && player_disconnet[0])
									{
										Disconnect_interface(0);
										ServerSocket1->Socket->Connections[0]->Close();
										Sleep(500);
										Form2->Memo1->Lines->Add("*斷1");
									}
								}
								else if(num == 1)
								{
									money2 = money;
									bet2 -= money2;
									bets[1] = bet2;
									if(bet2 < 0 && player_disconnet[1])
									{
										Disconnect_interface(1);
										if(!player_disconnet[0])
										{
											ServerSocket1->Socket->Connections[0]->Close();
											Sleep(500);
											Form2->Memo1->Lines->Add("*斷2_1");
										}
										else
										{
											if(player_disconnet[3] && player_disconnet[2])
												ServerSocket1->Socket->Connections[ServerSocket1->Socket->ActiveConnections-3]->Close();
											else if(!player_disconnet[3] || !player_disconnet[2])
												ServerSocket1->Socket->Connections[ServerSocket1->Socket->ActiveConnections-2]->Close();

											Sleep(500);
											Form2->Memo1->Lines->Add("*斷2_2");
										}
									}
								}
								else if(num == 2)
								{
									money3 = money;
									bet3 -= money3;
									bets[2] = bet3;
									if(bet3 < 0 && player_disconnet[2])
									{
										Disconnect_interface(2);

										if(!player_disconnet[0] && !player_disconnet[1])
										{
											ServerSocket1->Socket->Connections[0]->Close();
											Sleep(500);
											Form2->Memo1->Lines->Add("*斷3_1");
										}
										else if(!player_disconnet[1])
										{
											ServerSocket1->Socket->Connections[1]->Close();
											Sleep(500);
											Form2->Memo1->Lines->Add("*斷3_2");
										}
										else
										{
											if(player_disconnet[3])
												ServerSocket1->Socket->Connections[ServerSocket1->Socket->ActiveConnections-2]->Close();
											else
												ServerSocket1->Socket->Connections[ServerSocket1->Socket->ActiveConnections-1]->Close();
											Sleep(500);
											Form2->Memo1->Lines->Add("*斷3_3");
										}
									}
								}
								else if(num == 3)
								{
									money4 = money;
									bet4 -= money4;
									bets[3] = bet4;
									if(bet4 < 0 && player_disconnet[3])
									{
										Disconnect_interface(3);
										ServerSocket1->Socket->Connections[ServerSocket1->Socket->ActiveConnections-1]->Close();
											Sleep(500);
										Form2->Memo1->Lines->Add("*斷4");
									}
								}
							}
							//else
							{
								/*for(int i = 0; i < 4;i++)
									if(!player_disconnet[i])
										count--;
								if(count == 1)
								{
									//Memo1->Lines->Add("恭喜獲得第一名！");
									return;
								}*/

								/*if(num == 0)
								{
									money1 = money;
									bet1 -= money1;
									if(bet1 < 0)
										Disconnect_interface(0);

								}
								else if(num == 1)
								{
									money2 = money;
									bet2 -= money2;
									if(bet2 < 0)
										Disconnect_interface(1);

								}
								else if(num == 2)
								{
									money3 = money;
									bet3 -= money3;
									if(bet3 < 0)
										Disconnect_interface(2);
								}
								else if(num == 3)
								{
									money4 = money;
									bet4 -= money4;
									if(bet4 < 0)
										Disconnect_interface(3);
								}*/
							}
						}
						catch(...)
						{
							Form2->Memo1->Lines->Add("oh on5");
							//ShowMessage("oh on5");
						}
						//player_bet[0] = bet1;
						Label1->Caption = L"籌碼：" + IntToStr(bet1);
						Label2->Caption = L"賭注：" + IntToStr(money1);
						//player_bet[1] = bet2;
						Label4->Caption = L"籌碼：" + IntToStr(bet2);
						Label5->Caption = L"賭注：" + IntToStr(money2);
						//player_bet[2] = bet3;
						Label7->Caption = L"籌碼：" + IntToStr(bet3);
						Label8->Caption = L"賭注：" + IntToStr(money3);
						//player_bet[3] = bet4;
						Label10->Caption = L"籌碼：" + IntToStr(bet4);
						Label11->Caption = L"賭注：" + IntToStr(money4);
						 break;
					case 3: // 傳送全體玩家手牌
						AllCard = pPkg->Msg;
						people_num = pPkg->MsgLength/15;
						cards->Delimiter = L'、';
						cards->DelimitedText = AllCard;

						int index_sort = 0;
						for (int i = 0; i < 4; i++)
							for (int j = 0; j < 5; j++)
								player_cards[i][j] = cards->Strings[index_sort++].ToInt();

						for (int k = 0; k < 4; k++)	// 泡沫排序
							for (int i = 0; i < 5 - 1; i++)
								for (int j = 0; j < 5 - i - 1; j++)
								{
									if(get_card_num(player_cards[k][j]) > get_card_num(player_cards[k][j + 1]))
									{
										int temp = player_cards[k][j];
										player_cards[k][j] = player_cards[k][j + 1];
										player_cards[k][j + 1] = temp;
									}
								}
						delete cards;

						// 顯示每個玩家的撲克牌在對應的 Panel 上

						/*if(people_num == 1)
						{
							displayPlayerCards(Panel1, playerHands[0], 0);
						}
						else if(people_num == 2)
						{
							displayPlayerCards(Panel1, playerHands[0], 0);
							displayPlayerCards(Panel2, playerHands[1], 1);
						}
						else if(people_num == 3)
						{
							displayPlayerCards(Panel1, playerHands[0], 0);
							displayPlayerCards(Panel2, playerHands[1], 1);
							displayPlayerCards(Panel3, playerHands[2], 2);
						}
						else
						{
							displayPlayerCards(Panel1, playerHands[0], 0);
							displayPlayerCards(Panel2, playerHands[1], 1);
							displayPlayerCards(Panel3, playerHands[2], 2);
							displayPlayerCards(Panel4, playerHands[3], 3);
						}*/
						for (int i = 0; i < 4; i++)
						{
							//Form2->Memo1->Lines->Add("++++++++++");
							//Form2->Memo1->Lines->Add(player_disconnet[i]);
							if (player_disconnet[i])
							{
								switch (i)
								{
									case 0:
										displayPlayerCards(Panel1, playerHands[i], i);
										break;
									case 1:
										displayPlayerCards(Panel2, playerHands[i], i);
										break;
									case 2:
										displayPlayerCards(Panel3, playerHands[i], i);
										break;
									case 3:
										displayPlayerCards(Panel4, playerHands[i], i);
										break;
									default:
										// Handle additional players
										break;
								}
							}
						}

						who_win(4);
						break;
			   }
			 break;
		 break;

	default:
		 break;
	}
}
//---------------------------------------------------------------------------
void TForm1::ReflashClientList()
{

	ComboBox1->Clear();

	if(ServerSocket1->Socket->ActiveConnections>1)
		ComboBox1->Items->Add("All");

	for(int i=0;i<ServerSocket1->Socket->ActiveConnections;i++)
	ComboBox1->Items->Add(ServerSocket1->Socket->Connections[i]->RemoteAddress+"-"+
						  IntToStr(ServerSocket1->Socket->Connections[i]->SocketHandle));

	ComboBox1->ItemIndex=0;

	ComboBox1->Enabled=true;

	if(ServerSocket1->Socket->ActiveConnections < 4)
		Start_Game->Enabled = false;
	else
		Start_Game->Enabled = true;


	if(ServerSocket1->Socket->ActiveConnections == 1 && start_game)
	{
		for (int i = 0; i < ServerSocket1->Socket->ActiveConnections; ++i)
			ServerSocket1->Socket->Connections[i]->Close();
		reset();
		ComboBox1->Items->Add("All");
	}


}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpendebugwindowClick(TObject *Sender)
{
    Form2->Top=Form1->Top;
	Form2->Left=Form1->Left+Form1->Width+10;
	Form2->Show();
}
//---------------------------------------------------------------------------
void TForm1::ShowPacket(pPACKET pPkg)
{
	Form2->Memo1->Lines->Add("====================");
	Form2->Memo1->Lines->Add("ID= "+pPkg->ID);
	Form2->Memo1->Lines->Add("Host= "+pPkg->Host);
	Form2->Memo1->Lines->Add("MainFunc= "+pPkg->MainFunc);
	Form2->Memo1->Lines->Add("SubFunc= "+pPkg->SubFunc);
	Form2->Memo1->Lines->Add("Msg length= "+IntToStr(pPkg->MsgLength));
	Form2->Memo1->Lines->Add("Msg= "+pPkg->Msg);

}
//---------------------------------------------------------------------------
void TForm1::ShowPacket(PACKET Pkg)
{
	Form2->Memo1->Lines->Add("====================");
	Form2->Memo1->Lines->Add("ID= "+Pkg.ID);
	Form2->Memo1->Lines->Add("Host= "+Pkg.Host);
	Form2->Memo1->Lines->Add("MainFunc= "+Pkg.MainFunc);
	Form2->Memo1->Lines->Add("SubFunc= "+Pkg.SubFunc);
	Form2->Memo1->Lines->Add("Msg length= "+IntToStr(Pkg.MsgLength));
	Form2->Memo1->Lines->Add("Msg= "+Pkg.Msg);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    // TServerSocket和TClientSocket沒有TimeOut，
	//所以使用Timer1模擬。
	Timer1->Enabled=false;
	ClientSocket1->Active=false;
	Memo1->Lines->Add("Time Out");
    //Memo1->Lines->Add(ClientSocket1->Active);
	//ShowMessage("Time Out");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
    //client斷線後，需要幾秒的時間去更新資訊，
	//所以使用Timer2去延遲3秒，這樣抓到的資料才會正確
	Timer2->Enabled=false;
	ReflashClientList();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	String msg;
	PACKET pkg;
	String strPkg;
	String srtClient;

   pkg.ID="CHAT";

   if(blnServer)
	 pkg.Host=IdIPWatch1->LocalIP();
   else
	 pkg.Host=ClientHostName;

   start_game = true;
   Timer3->Enabled = True;

   for(int i=0;i<ServerSocket1->Socket->ActiveConnections;i++)
   {
		pkg.separation="#";
		pkg.MainFunc=2; //傳送資料
		pkg.SubFunc=1;
		pkg.Msg=i;
		pkg.MsgLength=pkg.Msg.Length();
		pkg.EndSymbol="@";
		strPkg=AssemblePacket(pkg);
		ServerSocket1->Socket->Connections[i]->SendText(strPkg);
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	bet(Button2->Caption);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	bet(Button3->Caption);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	bet(Button4->Caption);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
	bet(Button5->Caption);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer3Timer(TObject *Sender)
{
	if(sec >= 0)
	{
		Memo1->Lines->Add("下注時間剩餘：" + IntToStr(sec) + "秒");
		sec--;

		if(sec == 0 && !bet_TF)
			bet(10);

	}
	else if(sec == 10)
	{
		Button2->Enabled = true;
		Button3->Enabled = true;
		Button4->Enabled = true;
		Button5->Enabled = true;
    }
	else
	{
		sec = 10;
		All_bet = true;
		bet_TF = false;
		Timer3->Enabled = false;

		if(blnServer)
			Licensing();
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer4Timer(TObject *Sender)
{
	initialize();
	play_Game();
	Timer4->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer5Timer(TObject *Sender)
{
	int count = 4;

	for (int i = 0; i < 4; i++)
	{
		//Form2->Memo1->Lines->Add(player_disconnet[i]);
		if(!player_disconnet[i])
			count--;
	}

	/*if(count == 2)
	{
		if (player_disconnet[0])
		{
			max = bet1;
			Form2->Memo1->Lines->Add("--1--");
		}
		else if (player_disconnet[1] && bet2 > max)
		{
			max = bet2;
			Form2->Memo1->Lines->Add("--2--");
		}
		else if (player_disconnet[2] && bet3 > max)
		{
			max = bet3;
			Form2->Memo1->Lines->Add("--3--");
		}
		else if (player_disconnet[3] && bet4 > max)
		{
			max = bet4;
			Form2->Memo1->Lines->Add("--4--");
		}

		if (player_disconnet[0])
		{
			min = bet1;
			Form2->Memo1->Lines->Add("--5--");
		}
		else if (player_disconnet[1] && bet2 < min)
		{
			min = bet2;
			Form2->Memo1->Lines->Add("--6--");
		}
		else if (player_disconnet[2] && bet3 < min)
		{
			min = bet3;
			Form2->Memo1->Lines->Add("--7--");
		}
		else if (player_disconnet[3] && bet4 < min)
		{
			min = bet4;
			Form2->Memo1->Lines->Add("--8--");
		}

		//if(max -min > 200)
		Memo1->Lines->Add(max);
			Memo1->Lines->Add(min);

			Memo1->Lines->Add("bet1"+IntToStr(bet1));
			Memo1->Lines->Add("bet2"+IntToStr(bet2));
			Memo1->Lines->Add("bet3"+IntToStr(bet3));
			Memo1->Lines->Add("bet4"+IntToStr(bet4));
	}*/
}
//---------------------------------------------------------------------------

