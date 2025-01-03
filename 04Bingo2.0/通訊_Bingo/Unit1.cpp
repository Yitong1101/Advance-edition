//---------------------------------------------------------------------------
#include <vcl.h>
#include <cstdlib> // 包含 cstdlib 頭文件
#include <ctime>   // 包含 ctime 頭文件
#include <typeinfo>

#pragma hdrstop

#include "Unit1.h"
#include <iostream>
#include <winsock2.h>

#define CLIENT_MAX 5  //the maxinum of client
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

TStringList* lstLine;           //用來儲存功能碼的陣列
TStringList* Head;              //用來儲存標頭檔的陣列
String strHost;
String str;                     //封包內容串起來的字串
String ID = "Bingo";
String Header;
String whole_str;

bool blnserver;  				// 當作server or client
bool disconnect = false;  		// 判斷是否按"斷線"
bool connect_out = true;  		// 判斷是否斷線狀態
bool bingo;     				// 判斷是否賓果了
bool next_game_select = false;  // 是否要再來一局
bool no_response = false;  		// 已無回應
bool wait = false;              // 等待時間
bool arba = true;               // 能不能讀內容

int attack_side = -1;           // 判斷目前攻擊方是誰
int width = 5;              	// 遊戲的長
int height = 5;             	// 遊戲的寬
int temp1[5][5];    			// 暫時儲存的陣列
int temp2[5][5];    			// 暫時儲存的陣列
int wait_time = 0;  			// 等待對手的時間
int judge_side1;  				// 場地判斷
int judge_side2;            	// 場地判斷
int clicked1[5][5];  			// 判斷是否已被點擊
int clicked2[5][5];
int Number;                     // 選擇的數字
int client_num = 0;
int sort_client = -1;			// 排序用戶編號
int random_check_num;
int will_disconnect_client; 	// 即將斷線的用戶
int rand1, rand2;
int bingo_num1_temp = 0;    	// 賓果某一方暫時儲存的數字
int bingo_num2_temp = 0;    	// 賓果某一方暫時儲存的數字
int bingo_link = 0;  			// 紀錄連線數
int bingo_link2 = 0;        	// 紀錄連線數
int pick_num = -1;  			// 選取數值
int again_time = 10;        	// 等待的時間
int answer;  					// 再來一局判斷
int S_newgame = -1;  			// server再來一場
int C_newgame = -1;            	// client再來一場

TLabel *player_label[5][5]; 	// 玩家的label
TLabel *ai_label[5][5];     	// 對手的label
TShape *player_bingo[5][5];     // 玩家的shape
TShape *ai_bingo[5][5];         // 玩家的shape
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	New_shape(player_bingo, 25);
	New_shape(ai_bingo, 600);
	New_label(player_label, 35, temp1);
	New_label(ai_label, 610, temp2);
	Header = ID + "、" + 1 + "#";  //設定封包頭
}
//---------------------------------------------------------------------------
void __fastcall TForm1::New_shape(TShape* who_bingo[5][5], int left)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			who_bingo[i][j] = new TShape(this);
			who_bingo[i][j]->Parent = this;
			who_bingo[i][j]->Left = j * 105 + left;
			who_bingo[i][j]->Top = 110 + i * 105;
			who_bingo[i][j]->Width = 100;
			who_bingo[i][j]->Height = 100;
			who_bingo[i][j]->Brush->Color = clWhite;
			who_bingo[i][j]->Shape = stRoundRect;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::New_label(TLabel* who_label[5][5], int left, int temp[5][5])
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			who_label[i][j] = new TLabel(this);
			who_label[i][j]->Parent = this;
			who_label[i][j]->Left = j * 105 + left;
			who_label[i][j]->Top = 110 + i * 105;
			who_label[i][j]->Width = 100;
			who_label[i][j]->Height = 100;
			who_label[i][j]->Font->Size = 28;
			who_label[i][j]->OnClick = choose_num;

			if(temp[i][j] >= 10)
				player_label[i][j]->Caption = IntToStr(temp[i][j]);
			else
				player_label[i][j]->Caption = "";
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Random(int temp[5][5], int num, bool state)
{
	if (state)
		srand(time(0)); // 使用秒作為亂數
	else
		srand(time(0) * 1000); // 使用毫秒作為亂數

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			temp[i][j] = -1;

	while(num <= 25)
	{
		rand1 = rand() % width;
		rand2 = rand() % height;

		if(temp[rand1][rand2] == -1)
		{
			temp[rand1][rand2] = num;
			num++;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Reset()     //  重置遊戲
{
	initial();
	Form1->Caption = "Bingo";
	StatusBar1->SimpleText = "";
	Memo2->Clear();
	connect_out = true;
	ClientSocket1->Active = false;
	//ServerSocket1->Active = false;
	//Listen->Checked = false;
	Connect->Checked = false;
	Disconnect->Checked = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::initial()   // 初始化遊戲
{
    attack_side = 0;
	client_num = 0;
	bingo_link = 0;
	bingo_link2 = 0;
	S_newgame = -1;
	C_newgame = -1;
	Label3->Caption = "Score：0";
	Label4->Caption = "Score：0";
    Label5->Caption = "0";
	Connect->Checked = false;
	Disconnect->Checked = false;
	again_time = 10;
    Connect->Enabled = true;
    Listen->Enabled = true;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			player_label[i][j]->Caption = "";
			ai_label[i][j]->Caption = "";

			player_bingo[i][j]->Brush->Color = clWhite;
			ai_bingo[i][j]->Brush->Color = clWhite;

			clicked1[i][j] = 0;
			clicked2[i][j] = 0;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::start_game()   //開始遊戲
{
	srand(time(0));
	judge_side1 = 0;
	judge_side2 = 0;
	first_attack(0);  //server先攻
	attack_side = 0;
	again_time = 10;

	ServerSocket1->Socket->Connections[0]->SendText(Header + "3、" + IntToStr(1) + "、" + IntToStr(pick_num) + "、@");

	Random(temp1, 1, false);
	Random(temp2, 1, true);

	show_value(temp1, player_label);  //顯示value
	show_value(temp2, ai_label);

	//傳送封包"左邊value"
	str = Header + "2、0、";
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			str += IntToStr(temp1[i][j]) + "、";
	str += "@";
	ServerSocket1->Socket->Connections[0]->SendText(str);

	Sleep(100);

	//傳送封包"右邊value"
	str = Header + "2、1、";
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			str += IntToStr(temp2[i][j]) + "、";
	str += "@";
	ServerSocket1->Socket->Connections[0]->SendText(str);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::first_attack(int num)  //判斷誰先攻擊
{
	if(num == 0)
	{
		Label2->Caption = "玩家";
		Label1->Caption = "對手";
		Memo2->Lines->Add("由你先攻");
	}
	else
	{
		judge_side1 = 1;
		judge_side2 = 1;
        Label1->Caption = "玩家";
		Label2->Caption = "對手";
        Memo2->Lines->Add("對手先攻");
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::mark_num(int num)  //取得號碼標記
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if(temp1[i][j] == num)
			{
				clicked1[i][j] = 1;
				player_bingo[i][j]->Brush->Color = clSkyBlue;
			}

			if(temp2[i][j] == num)
			{
				clicked2[i][j] = 1;
				ai_bingo[i][j]->Brush->Color = clSkyBlue;
			}
		}
	}
}
//---------------------------------------------------------------------------  //show數值
void __fastcall TForm1::show_value(int temp[5][5], TLabel *label[5][5])
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if(temp[i][j] >= 10)
				label[i][j]->Caption = IntToStr(temp[i][j]);
			else
				label[i][j]->Caption = "0" + IntToStr(temp[i][j]);
		}
	}
}
//---------------------------------------------------------------------------  //取得數值
void __fastcall TForm1::get_value(int temp[5][5], int no)
{
	for (int i = 0, num = 2; i < width; i++)
		for (int j = 0; j < height; j++, num++)
			temp[i][j] = StrToInt(lstLine->Strings[num]);

	if(no == 0)
		show_value(temp1, player_label);
	else
		show_value(temp2, ai_label);
}
//---------------------------------------------------------------------------
int TForm1::bingo_V_H(int bingo_num, bool bingo, int VH, int click[5][5])  //水平
{
	bingo_num = 0;
	for(int i = 0; i < width; i++)
	{
		bingo = true;

		for(int j = 0; j < height; j++)
		{
			if ((VH == 0 && click[i][j] != 1) || (VH == 1 && click[j][i] != 1))
            {
                bingo = false;
                break;  // 一旦找到一個不是1的元素，就可以提前退出內部循環
			}
		}

		if(bingo == true)
			bingo_num++;
	}
	
	return bingo_num;
}
//---------------------------------------------------------------------------
int TForm1::bingo_slash(int bingo_num, bool bingo, int LR, int click[5][5])  //斜線判斷
{
    bingo_num = 0;
	bingo = true;

	if(LR == 0)
	{
		for (int i = 0; i < width; i++)
		if (click[i][i] != 1)
		{
			bingo = false;
			break;
		}
	}
	else
	{
		for (int i = 4, j = 0; j < width; i--, j++)
		if (click[i][j] != 1)
		{
			bingo = false;
			break;
		}
	}

	if(bingo == true)
		bingo_num++;

    return bingo_num;
}
//---------------------------------------------------------------------------  //取得選取數值
void __fastcall TForm1::Get_Select__Number()
{
    mark_num(Number);  //尋找數值
	attack_side = StrToInt(lstLine->Strings[1]);

	Label5->Caption = lstLine->Strings[2];
	Label5->Font->Color = clSkyBlue;
	Label5->Visible = true;
	Memo2->Lines->Add("對手選擇：" + lstLine->Strings[2]);
	Memo2->Lines->Add("輪到你了！");

	bingo_show();
}
//---------------------------------------------------------------------------  //取得用戶編號
void __fastcall TForm1::Send_Get_Id()
{
	if(random_check_num == StrToInt(lstLine->Strings[2]))
	{
		//取得用戶編號
		sort_client = StrToInt(lstLine->Strings[1]);

		//判斷server連線數量 是否 超過一人
		if(sort_client > 0)
		{
			//傳送斷線請求
			str = Header + "8、" + IntToStr(sort_client) + "、0、@";
			ClientSocket1->Socket->SendText(str);
			Timer4->Enabled = true;
			ClientSocket1->Active = false;
			//MessageDlg("伺服器已在遊戲中，無法進行連線！", mtInformation, TMsgDlgButtons() << mbYes, 0);
			Memo2->Clear();
			Memo2->Lines->Add("伺服器已在遊戲中，無法進行連線！");
		}
	}
}
//---------------------------------------------------------------------------  //確定再來一局
void __fastcall TForm1::sure_new_game()
{
    Timer2->Enabled = false;
	Memo2->Lines->Add("遊戲即將開始！");
}
//---------------------------------------------------------------------------  //取得對手回覆
void __fastcall TForm1::new_game_get_answer(int* newgame)
{
	if(lstLine->Strings[2] == "0")
	{
		*newgame = StrToInt(lstLine->Strings[2]);
		Memo2->Lines->Add("對手同意再來一局");
    }
    else
    {
        *newgame = StrToInt(lstLine->Strings[2]);

        if(no_response == true)
            Memo2->Lines->Add("對手已離開對戰");
        else
            Memo2->Lines->Add("對手拒絕再來一局");

        Timer2->Enabled = false;
        Memo2->Lines->Add("連線即將中斷");
        connect_out = true;
        Timer3->Enabled = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::new_game_judge() //再來一局判斷
{
	if(lstLine->Strings[1] == "0")
	{
		//調用next_game_get_answer函數，並傳遞了一個指向S_nextgame變數的指針
		new_game_get_answer(&S_newgame);
	}
	else
	{
		new_game_get_answer(&C_newgame);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::new_game_send_pocket(String yse_or_no, int event)  //傳送再來一局封包
{
	str = Header + "4、" + IntToStr(judge_side1)  + "、"  +  yse_or_no + "、@";

	if(judge_side1 == 0)
	{
		S_newgame = 0;
		ServerSocket1->Socket->Connections[0]->SendText(str);
	}
	else
	{
		C_newgame = 0;
		ClientSocket1->Socket->SendText(str);
	}

    //0:等待對手回應 1:執行斷線處理
	if(event == 0)
	{
		Timer2->Enabled = true;
        next_game_select = true;
	}
	else
	{
		next_game_select = true;
		Timer2->Enabled = false;
		Memo2->Lines->Add("連線即將中斷");
		if(judge_side1 != 0)
			MessageDlg("連線中斷", mtInformation, TMsgDlgButtons() << mbYes, 0);
			//Reset();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::show_result(String str)  //顯示結果
{
	Memo2->Lines->Add(str);
	MessageDlg(str, mtInformation, TMsgDlgButtons() << mbYes, 0);
	initial();

	answer = MessageDlg("要再玩一場嗎？", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0);
	if (answer == mrYes && connect_out == false)  //同意
	{
		new_game_send_pocket(0, 0);
	}
	else
	{
		if (answer == mrNo && connect_out == false)  //不同意
			new_game_send_pocket(1, 1);
		else if (connect_out == false)  //未選擇
			new_game_send_pocket(1, 1);
		else  //已被斷線 當作已選擇過
			next_game_select = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bingo_show()    // 計算連線數量
{
	bingo_link = 0;
	bingo_link2 = 0;

	bingo_link += bingo_V_H(bingo_num1_temp, bingo, 0, clicked1);
	bingo_link += bingo_V_H(bingo_num1_temp, bingo, 1, clicked1);
	bingo_link += bingo_slash(bingo_num1_temp, bingo, 0, clicked1);
	bingo_link += bingo_slash(bingo_num1_temp, bingo, 1, clicked1);
	Label3->Caption = "Score：" + IntToStr(bingo_link);

	bingo_link2 += bingo_V_H(bingo_num2_temp, bingo, 0, clicked2);
	bingo_link2 += bingo_V_H(bingo_num2_temp, bingo, 1, clicked2);
	bingo_link2 += bingo_slash(bingo_num2_temp, bingo, 0, clicked2);
	bingo_link2 += bingo_slash(bingo_num2_temp, bingo, 1, clicked2);
	Label4->Caption = "Score：" + IntToStr(bingo_link2);

	//勝負判斷
	if(bingo_link >= 3 && bingo_link2 >= 3)
		show_result("平手!!!");
	else if((bingo_link2 >= 3 && judge_side1 == 0) || (bingo_link >= 3 && judge_side2 == 1))
		show_result("你贏了!!!");
	else if((bingo_link2 < 3 && judge_side1 == 0 && bingo_link >= 3) || (bingo_link < 3 && judge_side2 == 1 && bingo_link2 >= 3))
		show_result("你輸了!!!");
}
//---------------------------------------------------------------------------  //字串分割
void __fastcall TForm1::read_packet(AnsiString asStr)
{
	//接字串 然後進行字串分割
	Head = new TStringList;
	str = StringReplace(asStr, "#", "\r\n", TReplaceFlags() << rfReplaceAll);   // 所有#字元替換為\r\n
	Head->Text = str;

	lstLine = new TStringList;
	str = StringReplace(Head->Strings[1], "、", "\r\n", TReplaceFlags() << rfReplaceAll);
	lstLine->Text = str;
	Number = StrToInt(lstLine->Strings[2]);

	switch (StrToInt(lstLine->Strings[0]))
	{
		case 1: // 取得號碼的位置並標記並判斷目前是server or client
			if (StrToInt(lstLine->Strings[2]) != -1)
				Get_Select__Number();
			break;
		case 2: // 取得選擇的號碼雙方陣列的哪裡
			if (StrToInt(lstLine->Strings[1]) == 0)
				get_value(temp1, 0);
			else if (StrToInt(lstLine->Strings[1]) == 1)
				get_value(temp2, 1);
			break;
		case 3: // 判斷目前是玩家還是對手
			if (StrToInt(lstLine->Strings[1]) == 1 && pick_num == -1)
				first_attack(1);
			break;
		case 4: // 是否要再來一局
			switch (StrToInt(lstLine->Strings[2]))
			{
				case 0: //再來一局
					new_game_judge();
					break;
				case 1: // 不玩了
					connect_out = true;
					new_game_judge();
					break;
				case 2: // 無回應
					no_response = true;
					new_game_judge();
					break;
			}
			break;
		case 5: // 確定再來一場
			sure_new_game();
			break;
		case 6: // 請求用戶編號
			str = Header + "7、" + IntToStr(client_num) + "、" + lstLine->Strings[1] + "、@";
			ServerSocket1->Socket->Connections[client_num]->SendText(str);
			client_num++;
			break;
		case 7:
			Send_Get_Id();
			break;
		case 8:
			will_disconnect_client = StrToInt(lstLine->Strings[1]);
			//Memo2->Lines->Add(StrToInt(lstLine->Strings[1]));
			break;
	}
}
//---------------------------------------------------------------------------  //傳送封包
void __fastcall TForm1::send_pocket(TShape *player_bingo[5][5], int temp1[5][5], int row, int col, int judge, int attack, int click[5][5])
{
	player_bingo[row][col]->Brush->Color = clSkyBlue;
	mark_num(temp1[row][col]);
	click[row][col] = 1;

	Label5->Caption = IntToStr(temp1[row][col]);
	Label5->Font->Color = clBlack;
	Label5->Visible = true;

	Memo2->Lines->Add("你選擇了：" + IntToStr(temp1[row][col]));

	//攻守交換
	if(attack_side == 0)
		attack_side = 1;
	else
		attack_side = 0;

	//傳送Server or Client 封包
	str = Header + "1、" + IntToStr(attack_side)  + "、" + IntToStr(temp1[row][col]) + "、@";

	if(judge == 0)
		ServerSocket1->Socket->Connections[0]->SendText(str);
	else
		ClientSocket1->Socket->SendText(str);

	bingo_show();  //計算賓果連線數量
}
//---------------------------------------------------------------------------
void __fastcall TForm1::choose_num(TObject *Sender)  //選取號碼
{
    TLabel *clickbtn;
	clickbtn = dynamic_cast<TLabel*>(Sender);

	if (clickbtn)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				if(player_label[i][j] == clickbtn && judge_side2 == 1 && attack_side == 1 && clicked1[i][j] == 0)   //Client left side send pocket
					send_pocket(player_bingo, temp1, i, j, 1, attack_side, clicked1);

				if(ai_label[i][j] == clickbtn && judge_side1 == 0 && attack_side == 0 && clicked2[i][j] == 0)  //server right side send pocket
					send_pocket(ai_bingo, temp2, i, j, 0, attack_side, clicked2);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListenClick(TObject *Sender)  //server
{
	Listen->Checked = !Listen->Checked;

	if(Listen->Checked)
	{
        try
		{
            ClientSocket1->Active = false;
			ServerSocket1->Active = true;
            Form1->Caption = "I am a sever";
			StatusBar1->SimpleText = "Status: Chat Server Listening...";
			blnserver = true;
            Connect->Enabled = false;
		}
		catch (Exception &e)
		{
			Listen->Checked = !Listen->Checked;
			MessageDlg("Port:13579已被占用，請重新確認端口使用情況。", mtInformation, TMsgDlgButtons() << mbYes, 0);
		}
	}
	else
	{
		if(ServerSocket1->Active)
			ServerSocket1->Active = false;

		StatusBar1->SimpleText = "Status: Chat Server Close";
		Form1->Caption = "Bingo";
		blnserver = false;
		initial();
        Memo2->Clear();
        Connect->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ServerSocket1Accept(TObject *Sender, TCustomWinSocket *Socket)  //伺服器同意連線
{
	try
	{
		connect_out = false;

		//連線數量判斷
		if(client_num > 0)
			return;

		//Memo2->Lines->Add("no~++！");
		StatusBar1->SimpleText = "Status: Connect from" + Socket->RemoteAddress;
		//Memo2->Lines->Add("no~==！");
		start_game();  //產生遊戲
	}
	catch(...)
	{
		//Memo2->Lines->Add("no~2");
		Memo2->Clear();
		Memo2->Lines->Add("對手已斷線！");
        StatusBar1->SimpleText = "";
		initial();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ServerSocket1ClientConnect(TObject *Sender, TCustomWinSocket *Socket)  //用戶端連線至伺服器
{
	try
	{
		if(client_num > 0)
			return;

		StatusBar1->SimpleText = "Status：Connect from" + Socket->RemoteAddress;
	}
	catch(...)
	{
		Memo2->Lines->Add("no~3");
    }
}
//---------------------------------------------------------------------------  //用戶端斷線
void __fastcall TForm1::ServerSocket1ClientDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
	try
	{
		if(will_disconnect_client == 0)
		{
			if(connect_out == false)
			{
				StatusBar1->SimpleText = "Status: Listening...";
				//MessageDlg("對手已斷線！", mtInformation, TMsgDlgButtons() << mbYes, 0);
				//Memo2->Clear();
				initial();
				Connect->Enabled = false;
				Memo2->Lines->Add("對手已斷線！");
			}
		}
		else
		{
			client_num--;
		}
	}
	catch(...)
	{
		Memo2->Lines->Add("no~4");
	}

}
//---------------------------------------------------------------------------  //用戶端錯誤
void __fastcall TForm1::ServerSocket1ClientError(TObject *Sender, TCustomWinSocket *Socket,
		  TErrorEvent ErrorEvent, int &ErrorCode)
{
	try
	{
		if(ErrorCode == 10053)
		{
			// 重新連線的處理程序

			// 停止Socket的使用或進行清理
			ServerSocket1->Active = false;

			// 等待一段時間後重新啟動Socket，這裡假設是0.5秒
			Sleep(300);

			ServerSocket1->Active = true; // 重新啟動伺服器Socket
		}
		if(ErrorCode > 0)
		{
			StatusBar1->SimpleText = "Error form Client socket.";
			arba=false;
			Memo2->Lines->Add("2！");
			return;
		}
		ErrorCode=0;
	}
	catch(...)
	{
		Memo2->Lines->Add("no~1");
    }


}
//---------------------------------------------------------------------------  //讀取用戶端 訊息
void __fastcall TForm1::ServerSocket1ClientRead(TObject *Sender, TCustomWinSocket *Socket)
{
	try
	{
		whole_str = Socket->ReceiveText();
		read_packet(whole_str);
	}
	catch(...)
	{
		Memo2->Lines->Add("no~5");
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ConnectClick(TObject *Sender)  //判斷是server or client send 訊息
{
	strHost = "127.0.0.1";

	if(InputQuery("Chat Connection", "Chat Server IP: ", strHost))
	{

		if(ClientSocket1->Active && strHost != "127.0.0.1")
			ClientSocket1->Active = false;

		Form1->Caption = "I am a Client";

		ClientSocket1->Host = strHost;
		ClientSocket1->Active = true;
		Listen->Checked = false;
		Listen->Enabled = false;
		Connect->Enabled = false;
		Connect->Checked = true;
		Disconnect->Checked = false;
		blnserver = false;
		//Timer1->Enabled = true;
		disconnect = false;
	}
}
//---------------------------------------------------------------------------  //用戶端成功連線
void __fastcall TForm1::ClientSocket1Connect(TObject *Sender, TCustomWinSocket *Socket)
{
	wait_time = 0;
	Timer1->Enabled = false;
	connect_out = false;
	StatusBar1->SimpleText = "Status: Connect to " + Socket->RemoteAddress;


	srand(time(0));
	random_check_num = rand() % 500;

	ClientSocket1->Active = true;

	//請求用戶編號
	str = Header + "6、" + IntToStr(random_check_num) + "、0" + "、@";
	ClientSocket1->Socket->SendText(str);
}
//---------------------------------------------------------------------------  //Client 斷線
void __fastcall TForm1::ClientSocket1Disconnect(TObject *Sender, TCustomWinSocket *Socket)
{
	if(connect_out == false)
	{
		if(Timer1->Enabled != true)
		{
			StatusBar1->SimpleText = "Server Disconnected";
            Memo2->Lines->Add("對手已斷線！");
		}

		if(judge_side1 == 0)
			//MessageDlg("對手已斷線！", mtInformation, TMsgDlgButtons() << mbYes, 0);
			Memo2->Lines->Add("對手已斷線！");
		else if(judge_side2 == 1 && disconnect == false)  //Client被Server斷線
		{
			disconnect = false;
			Form1->Caption = "Bingo";

            //判斷是否於遊戲中
			if(client_num != 0 || judge_side2 == 1)
			{
				//MessageDlg("對手已斷線！", mtInformation, TMsgDlgButtons() << mbYes, 0);
                Listen->Enabled = true;
				Memo2->Lines->Add("對手已斷線！");
			}

			Disconnect->Checked = false;
			Memo2->Clear();
		}

		initial();  //初始化
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DisconnectClick(TObject *Sender)  //server自行取消連線
{
	Form1->Caption = "";
	disconnect = true;

    //傳送斷線請求
	str = Header + "8、" + IntToStr(sort_client) + "、0" + "、@";
	ClientSocket1->Socket->SendText(str);

	if(blnserver)
		ServerSocket1->Active = false;
	else
		ClientSocket1->Active = false;

    Timer1->Enabled = false;
	Listen->Checked = false;
	Connect->Checked = false;
    Listen->Enabled = true;
    Connect->Enabled = true;
	Disconnect->Checked = false;
	Form1->Caption = "Bingo";
	Memo2->Clear();

	StatusBar1->SimpleText = "Status: Disconnect";
    Connect->Enabled = true;
    initial();  //初始化
}
//---------------------------------------------------------------------------  //連線錯誤
void __fastcall TForm1::ClientSocket1Error(TObject *Sender, TCustomWinSocket *Socket,
		  TErrorEvent ErrorEvent, int &ErrorCode)
{
	if(ErrorCode > 0)
	{
		Form1->Caption = "Bingo";
		Memo2->Lines->Add("連線錯誤");
		ClientSocket1->Active = false;
		StatusBar1->SimpleText = "";
		connect_out = true;
		Listen->Enabled = true;
		Connect->Enabled = true;
		Connect->Checked = false;
	}
	ErrorCode = 0;
}
//---------------------------------------------------------------------------  //用戶端讀取訊息
void __fastcall TForm1::ClientSocket1Read(TObject *Sender, TCustomWinSocket *Socket)
{
	whole_str = Socket->ReceiveText();

	read_packet(whole_str);
}
//---------------------------------------------------------------------------  //判斷連線狀態
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	wait_time++;
	StatusBar1->SimpleText = "Status: Connecting...";

    //連線超時 進行初始化
	if(!ClientSocket1->Active && wait_time == 5)
	{
		wait_time = 0;
        Connect->Checked = false;
		Timer1->Enabled = false;
		MessageDlg("連線超時，請重新確認Server IP再進行連線", mtInformation, TMsgDlgButtons() << mbYes, 0);
		Form1->Caption = "Bingo";
		StatusBar1->SimpleText = "";
		Listen->Enabled = true;
		Connect->Enabled = true;
        ClientSocket1->Active = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer2Timer(TObject *Sender)  //再來一局計時判斷
{
    again_time--;
	Memo2->Lines->Add("等待對手回應：" + IntToStr(again_time) + "秒");

	if(S_newgame == 0 && C_newgame == 0)
	{
		Timer2->Enabled = false;

		if(judge_side1 != 1 && judge_side2 != 1)
		{
			Memo2->Lines->Add("遊戲即將開始！");

            //傳送確定再來一局
			str = Header + "5、0、0" + "、@";
			ServerSocket1->Socket->Connections[0]->SendText(str);
            //ClientSocket1->Socket->SendText(str);

            start_game();  //產生遊戲內容
		}
	}
	else if(S_newgame == 1 || C_newgame == 1)  //有一方不同意
		Timer2->Enabled = false;

    //對手無回應
	if(again_time == 0)
	{
		Timer2->Enabled = false;
        again_time = 10;
		MessageDlg("對手已無回應，即將斷線", mtInformation, TMsgDlgButtons() << mbYes, 0);
        new_game_send_pocket(2, 1);
	}
}
//---------------------------------------------------------------------------  //判斷是否再來一局
void __fastcall TForm1::Timer3Timer(TObject *Sender)
{
	if(next_game_select == true)
	{
		Timer3->Enabled = false;

		if(no_response == true)
			MessageDlg("對手離開對戰，連線已中斷", mtInformation, TMsgDlgButtons() << mbYes, 0);
		else
			MessageDlg("對手拒絕再來一局，連線已中斷", mtInformation, TMsgDlgButtons() << mbYes, 0);

		Reset();
        next_game_select = false;
	}
}
//---------------------------------------------------------------------------  //執行斷線
void __fastcall TForm1::Timer4Timer(TObject *Sender)
{
    Timer4->Enabled = false;
	connect_out = true;
	ClientSocket1->Active = false;
	StatusBar1->SimpleText = "";
	Form1->Caption = "Bingo";
	Connect->Checked = false;
    initial();
}
//---------------------------------------------------------------------------

