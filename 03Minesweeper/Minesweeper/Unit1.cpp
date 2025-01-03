﻿//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.cpp"
#include <cstdlib>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TBitBtn*** BitBtnMap;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	initial();
		//ShowMessage(1);
	Start_Game();
		//ShowMessage(2);

	if(start)
	{
		//ShowMessage(3);
		Random();
		//ShowMessage(4);
		count_boom();
		//ShowMessage(5);

	}
	else
	{
        MessageDlg("請重新輸入長、寬、炸彈數\r\n並介於5~10的數值", mtWarning , TMsgDlgButtons() << mbOK, 0);
	}
}
//---------------------------------------------------------------------------
void _fastcall TForm1::initial()    // 初始化程式
{
	Width = 0;
	Height = 0;
	boom = 0;
	Flag_num = 0;
	sec = 0;
}
//---------------------------------------------------------------------------
void _fastcall TForm1::Start_Game()     // 遊戲開始
{
	try
	{
		Width = StrToInt(Edit1->Text);
		Height = StrToInt(Edit2->Text);
		boom = StrToInt(Edit3->Text);

		if(10 < Width || Width < 5 || 10 < Height || Height < 5 || 10 < boom || boom < 5)
			return;
		else
			start = true;

		Flag_num = boom;
		int Boom_num[Width][Height];
		Label1->Visible = false;
		Label2->Visible = false;
		Label3->Visible = false;
		Edit1->Visible = false;
		Edit2->Visible = false;
		Edit3->Visible = false;
		Button1->Visible = false;
		Timer1->Enabled = true;
		start = true;
		N5->Caption = "Boom：" + IntToStr(boom);
		N7->Caption = "Flag：" + IntToStr(Flag_num);
		srand(time(NULL));
		BitBtnMap = new TBitBtn**[Width];
		for (int i = 0; i < Width; ++i)
		{
			BitBtnMap[i] = new TBitBtn*[Height];
			for (int j = 0; j < Height; ++j)
			{
				BitBtnMap[i][j] = new TBitBtn(this);
				BitBtnMap[i][j]->Parent = Panel1;
				BitBtnMap[i][j]->Left = i * 80;//(ClientWidth / Width)
				BitBtnMap[i][j]->Top = j * 80;
				BitBtnMap[i][j]->Width = 80;
				BitBtnMap[i][j]->Height = 80;
				BitBtnMap[i][j]->Tag = 0;
				BitBtnMap[i][j]->OnClick = OnClick;
				BitBtnMap[i][j]->OnMouseDown = BitBtnMouseDown;
				BitBtnMap[i][j]->Caption = " ";

                flag_index[i][j] = 0;
			}
		}
		Panel1->Visible = true;
		ClientWidth = 80 * Width;
		ClientHeight = 80 * Height;
	}
	catch(Exception & e)
	{
		MessageDlg("長、寬與炸彈數不可為空值", mtWarning , TMsgDlgButtons() << mbOK, 0);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Random()    // 亂數給炸彈
{
	int x = 0;
	int y = 0;
	int temp = 0;
	int tempxy = 0;
	int rd[boom];

	srand(time(NULL));
		//ShowMessage(1);
	for(int i = 0; i < boom; i++)
	{
		//ShowMessage(2);
		rd[i] = rand() % (Width * Height);
		//ShowMessage(3);

		for(int j = 0; j< i;j++)
		{
		//ShowMessage(4);
			 if(rd[i] == rd[j])
			 {
		//ShowMessage(5);
				 i--;
				 break;
			 }
		}
		//ShowMessage(6);
        x = rd[i] / Width;
		y = rd[i] % Height;
		//ShowMessage("rd" + IntToStr(rd[i]));
		//ShowMessage("X" + IntToStr(x));
		//ShowMessage("Y" + IntToStr(y));
		BitBtnMap[x][y]->Tag = 1;
		//ShowMessage(7);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::connectivity(int x, int y)  // 計算炸彈數八連通
{
	int x_index[] = {-1, 0, 1, -1, 1, -1, 0, 1};
	int y_index[] = {-1, -1, -1, 0, 0, 1, 1, 1};
	boomnum = 0;

	for (int i = 0; i < 8; i++)
	{
		int newX = x + x_index[i];
		int newY = y + y_index[i];

		if (0 <= newX && newX < Width && 0 <= newY && newY < Height)
		{
			if(BitBtnMap[newX][newY]->Tag == 1)
			{
				boomnum++;
				Boom_num[x][y] = boomnum;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::count_boom()    // 計算四周炸彈數
{
	for(int i = 0; i < Width; i++)
	{
		for(int j = 0; j < Height; j++)
		{
			connectivity(i,j);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::diffusion(int x, int y)  // 擴散
{
	int x_index[] = {-1, 0, 1, -1, 1, -1, 0, 1};
	int y_index[] = {-1, -1, -1, 0, 0, 1, 1, 1};

    if (BitBtnMap[x][y]->Tag == 1 || BitBtnMap[x][y]->Enabled == false)
		return;

	BitBtnMap[x][y]->Enabled = false;

	for (int i = 0; i < 8; i++)
	{
		int newX = x + x_index[i];
		int newY = y + y_index[i];

		if (0 <= newX && newX < Width && 0 <= newY && newY < Height)
		{
			if(BitBtnMap[newX][newY]->Caption == L"🚩")
			{
				Flag_num++;
				BitBtnMap[newX][newY]->Caption = " ";
				flag_index[newX][newY] = 0;
                N7->Caption = "Flag：" + IntToStr(Flag_num);
			}

			if (Boom_num[newX][newY] == 0)
			{
				diffusion(newX, newY);
			}
			else
			{
				BitBtnMap[newX][newY]->Caption = IntToStr(Boom_num[newX][newY]);
                BitBtnMap[newX][newY]->Enabled = false;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Delete_Map()    //  刪除地圖
{
	Timer1->Enabled = false;
    start = false;

	for (int i = 0; i < Width; ++i)
	{
		for (int j = 0; j < Height; ++j)
		{
			BitBtnMap[i][j]->Free(); // 釋放資源
			Boom_num[i][j] = 0;
		}
		delete[] BitBtnMap[i];
	}
	delete[] BitBtnMap;

	Label1->Visible = true;
    Label2->Visible = true;
	Label3->Visible = true;
    Edit1->Visible = true;
	Edit2->Visible = true;
	Edit3->Visible = true;
	Button1->Visible = true;
    Panel1->Visible = false;
	N4->Caption = "Time：0";
	N5->Caption = "Boom：0";
	N7->Caption = "Flag：0";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
	  int X, int Y)
{
	if (Button == mbRight)
	{
		TBitBtn *flagButton = dynamic_cast<TBitBtn*>(Sender);
		btn_x = flagButton->Left / 80;
		btn_y = flagButton->Top / 80;

		if (flagButton && !openboom)
		{
			if (flagButton->Caption == L"🚩")
			{

				flagButton->Caption = " "; // 取消插旗幟
				flag_index[btn_x][btn_y] = 0;
				Flag_num++;
			}
			else
			{
				if (Flag_num > 0 && flagButton->Caption != Boom_num[btn_x][btn_y])
				{
					flagButton->Caption = L"🚩"; // 插上旗幟
					flag_index[btn_x][btn_y] = 1;
					Flag_num--;
				}

				if (Flag_num == 0)
				{
					win = true;
					for (int i = 0; i < Width && win; i++)
					{
						for (int j = 0; j < Height; j++)
						{
							if (BitBtnMap[i][j]->Tag == 1 && BitBtnMap[i][j]->Caption != L"🚩")
							{
								win = false;
								break;
							}
						}
					}

					if (win)
					{
						Timer1->Enabled = false;
						for (int i = 0; i < Width && win; i++)
						{
							for (int j = 0; j < Height; j++)
							{
								if (BitBtnMap[i][j]->Tag == 0 && Boom_num[i][j] > 0)
									BitBtnMap[i][j]->Caption = Boom_num[i][j];
							}
						}
						//ShowMessage("恭喜獲勝！總花費時間：" + IntToStr(sec) + "秒");
						MessageDlg("恭喜獲勝！總花費時間：" + IntToStr(sec) + "秒", mtInformation , TMsgDlgButtons() << mbOK, 0);
						Delete_Map();
						ClientWidth = 800;
						ClientHeight = 800;
					}
				}
			}
		}
		else
		{
			MessageDlg("炸彈顯示中，所以不能插旗幟", mtInformation , TMsgDlgButtons() << mbOK, 0);
		}
		N7->Caption = "Flag：" + IntToStr(Flag_num);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Win()   // 判斷玩家是否已採完全部安全格
{
    for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			if (Boom_num[i][j] != 0 && BitBtnMap[i][j]->Tag != 1 && BitBtnMap[i][j]->Caption != Boom_num[i][j] && BitBtnMap[i][j]->Visible)
			{
				win = false;
			}
		}
	}

	if (win)
	{
		Timer1->Enabled = false;
		for (int i = 0; i < Width && win; i++)
		{
			for (int j = 0; j < Height; j++)
			{
				if (BitBtnMap[i][j]->Tag == 0 && Boom_num[i][j] > 0)
					BitBtnMap[i][j]->Caption = Boom_num[i][j];

				if(BitBtnMap[i][j]->Tag == 1)
					BitBtnMap[i][j]->Caption = L"💣";
			}
		}
		MessageDlg("恭喜獲勝！總花費時間：" + IntToStr(sec) + "秒", mtInformation , TMsgDlgButtons() << mbOK, 0);
		Delete_Map();
		ClientWidth = 800;
		ClientHeight = 800;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OnClick(TObject *Sender)    // 按鍵事件
{
	TBitBtn *clickedButton = dynamic_cast<TBitBtn*>(Sender);

	btn_x = clickedButton->Left / 80;
	btn_y = clickedButton->Top / 80;
	win = true;
	if(clickedButton->Caption != L"🚩")
	{
		if(clickedButton->Tag == 1 && clickedButton->Caption != L"🚩")
		{
			for (int i = 0; i < Width; i++)
			{
				for (int j = 0; j < Height; j++)
				{
					if(BitBtnMap[i][j]->Tag == 1)
					{
						BitBtnMap[i][j]->Caption = L"💣";
					}
				}
			}
			//ShowMessage("Boom！");
			MessageDlg("Boom！", mtInformation , TMsgDlgButtons() << mbOK, 0);
			Delete_Map();
			ClientWidth = 800;
			ClientHeight = 800;
		}
		else
		{
			if(Boom_num[btn_x][btn_y] == 0)
			{
				diffusion(btn_x,btn_y);
			}
			else
			{
				clickedButton->Caption = Boom_num[btn_x][btn_y];
                clickedButton->Enabled = false;
			}
			Win();
		}
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)    // 遊戲時間計時
{
    sec++;
    N4->Caption = "Time：" + IntToStr(sec);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N2Click(TObject *Sender)    // 透視炸彈
{
	if(start)
	{
		if(!openboom)
		{
			openboom = true;   // 開炸彈
			for (int i = 0; i < Width; i++)
			{
				for (int j = 0; j < Height; j++)
				{
					if(BitBtnMap[i][j]->Caption == L"🚩" && BitBtnMap[i][j]->Tag == 1)
					{
						Flag_num++;
						N7->Caption = "Flag：" + IntToStr(Flag_num);
					}
					if (BitBtnMap[i][j]->Tag == 1)
					{
						BitBtnMap[i][j]->Caption = L"💣";
					}
				}
			}
		}
		else
		{
			openboom = false;   // 關炸彈

			for (int i = 0; i < Width; i++)
			{
				for (int j = 0; j < Height; j++)
				{
					if(BitBtnMap[i][j]->Caption == L"🚩" && BitBtnMap[i][j]->Tag == 1)
					{
						Flag_num++;
						N7->Caption = "Flag：" + IntToStr(Flag_num);

					}
					if(BitBtnMap[i][j]->Tag == 1)
					{
						BitBtnMap[i][j]->Caption = " ";
					}
					if(flag_index[i][j] == 1 && BitBtnMap[i][j]->Tag == 1)
					{
						BitBtnMap[i][j]->Caption = L"🚩";
                        Flag_num--;
						N7->Caption = "Flag：" + IntToStr(Flag_num);
					}
				}
			}
        }
	}
	else
		MessageDlg("遊戲尚未開始，不能透視炸彈！", mtInformation , TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N3Click(TObject *Sender)    // 重新開始
{
	if(start)
	{
		Delete_Map();
		sec = 0;
		boom = 0;
		Flag_num = 0;
		openboom = false;
		Start_Game();
		Random();
		count_boom();
	}
	else
	{
		MessageDlg("遊戲尚未開始！", mtInformation , TMsgDlgButtons() << mbOK, 0);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N6Click(TObject *Sender)    // 設定遊戲
{
	if(start)
	{
		Delete_Map();

		sec = 0;
		boom = 0;
		Flag_num = 0;
		openboom = false;
		ClientWidth = 800;
		ClientHeight = 800;
	}
	else
		MessageDlg("已在設定遊戲！", mtInformation , TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------
