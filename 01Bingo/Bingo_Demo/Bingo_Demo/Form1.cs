using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Reflection.Emit;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Bingo_Demo
{
    public partial class Form1 : Form
    {
        //============================變數宣告===============================
        List<Button> play_Btns = new List<Button>();  //裝玩家賓果數字陣列
        List<Button> Ai_Btns = new List<Button>();    //裝電腦賓果數字陣列
        List<List<int>> Play_Random = new List<List<int>>();//儲存玩家亂數的陣列
        List<List<int>> Ai_Random = new List<List<int>>();  //儲存電腦還數的陣列
        int[,] temp = new int[5,5];//暫時儲存玩家的亂數
        int[,] ai_temp = new int[5,5];//暫時儲存電腦的亂數
        bool state = true;//true玩家 false電腦        
        bool winlink;//玩家連線
        bool ai_winlink;//電腦連線
        int[] w = new int[5];
        int[] H = new int[5];
        int slash_R = 0, slash_L = 0;//右邊斜線,左邊斜線

        public Form1()
        {
            InitializeComponent();
            initialize();
        }
        //============================自訂函式===============================
        private void initialize()//初始化
        {            
            label3.Text = "玩家選擇：0";
            label4.Text = "電腦選擇：0";
            label5.Text = "玩家連線：0";
            label6.Text = "電腦連線：0";
            Array.Clear(w,0,5);
            Array.Clear(H,0,5);

            for (int i = 0; i < 5; i++)
            {
                List<int> row = new List<int>();
                for (int j = 0; j < 5; j++)
                    row.Add(0);
                Play_Random.Add(row);
                Ai_Random.Add(row);
            }

            Play_Random = Random(5, 5, 1, 25, true);
            Ai_Random = Random(5, 5, 1, 25, false);

            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 5; j++)
                {
                    temp[i, j] = Play_Random[i][j];
                    ai_temp[i, j] = Ai_Random[i][j];
                }

            play_Btns = game_display(temp);
            Ai_Btns = ai_game_display(ai_temp);

            foreach (Button Btns in play_Btns)
            {
                Btns.Click += OnClick;
                this.Controls.Add(Btns);
            }
            foreach (Button ai_Btns in Ai_Btns)
                this.Controls.Add(ai_Btns);
        }
        private void OnClick(object sender, EventArgs e)//玩家按下數字後事件
        {
            Button btns = (Button)sender;
            int find_num = 0;
            int ai_find_num = 0;

            if (state && btns.Name == "0")
            {
                if(btns.Name != "1" )
                {
                    btns.ForeColor = Color.DarkOrange;
                    btns.Name = "1";
                    label3.Text = "玩家選擇：" + btns.Text;
                }                

                foreach (Button find in play_Btns) //尋找玩家按下的數字
                {
                    if (btns.Text != find.Text)
                        find_num++;
                    else                        
                        break;
                }
                foreach (Button find in Ai_Btns)
                {
                    if (btns.Text != find.Text)
                        ai_find_num++;
                    else
                    {
                        find.ForeColor = Color.DodgerBlue;
                        find.Name = "1";
                        break;
                    }
                }
                Click_num();
                AI();
                winlink = Judge_line();
                ai_winlink = Ai_Judge_line();
                if (winlink == ai_winlink && winlink == true)
                {
                    MessageBox.Show("玩家與電腦平手");
                    for (int i = 0; i < 25; i++)
                    {
                        this.Controls.Remove(play_Btns[i]);
                        this.Controls.Remove(Ai_Btns[i]);
                    }
                    button1.Enabled = true;
                    initialize();
                }
                else 
                    if (winlink)
                    {
                        MessageBox.Show("恭喜你！贏得本局");
                        for (int i = 0; i < 25; i++)
                        {
                            this.Controls.Remove(play_Btns[i]);
                            this.Controls.Remove(Ai_Btns[i]);
                        }
                        button1.Enabled = true;
                        initialize();
                    }                    
                    else
                        if (ai_winlink)
                        {
                            MessageBox.Show("真可惜！電腦贏得本局");
                            for (int i = 0; i < 25; i++)
                            {
                                this.Controls.Remove(play_Btns[i]);
                                this.Controls.Remove(Ai_Btns[i]);
                            }
                            button1.Enabled = true;
                            initialize();
                        }
            }
        }
        private bool Judge_line()//判斷玩家連線
        {
            int link_line = 0;
            bool winlink = false;

            //直的連線
            for (int i = 0; i < 5; i++)
                if (play_Btns[0 + i * 5].Name + play_Btns[1 + i * 5].Name + play_Btns[2 + i * 5].Name + play_Btns[3 + i * 5].Name + play_Btns[4 + i * 5].Name == "11111")
                {
                    for (int j = 0; j < 5; j++)
                        play_Btns[j + i * 5].BackColor = Color.Bisque;
                    link_line += 1;
                }
            //橫的連線
            for (int i = 0; i < 5; i++)
                if (play_Btns[i].Name + play_Btns[5 + i].Name + play_Btns[10 + i].Name + play_Btns[15 + i].Name + play_Btns[20 + i].Name == "11111")
                {
                    for (int j = 0; j < 5; j++)
                        play_Btns[j * 5 + i].BackColor = Color.Bisque;
                    link_line += 1;
                }
            if (play_Btns[0].Name + play_Btns[6].Name + play_Btns[12].Name + play_Btns[18].Name + play_Btns[24].Name == "11111")
            {
                for (int j = 0; j < 5; j++)
                    play_Btns[j * 6].BackColor = Color.Bisque;
                link_line += 1;
            }
            if (play_Btns[4].Name + play_Btns[8].Name + play_Btns[12].Name + play_Btns[16].Name + play_Btns[20].Name == "11111")
            {
                for (int j = 1; j < 6; j++)
                    play_Btns[j * 4].BackColor = Color.Bisque;
                link_line += 1;
            }

            label6.Text = "玩家連線：" + link_line;

            if (link_line >= 3)
                winlink = true;
            return winlink;
        }
        private bool Ai_Judge_line()//判斷電腦連線
        {
            int ai_link_line = 0;
            bool ai_winlink = false;

            //直的連線
            for (int i = 0; i < 5; i++)
            {
                if (Ai_Btns[0 + i * 5].Name + Ai_Btns[1 + i * 5].Name + Ai_Btns[2 + i * 5].Name + Ai_Btns[3 + i * 5].Name + Ai_Btns[4 + i * 5].Name == "11111")
                {
                    for (int j = 0; j < 5; j++)
                        Ai_Btns[j + i * 5].BackColor = Color.LightSkyBlue;
                    ai_link_line += 1;
                }
            }
            //橫的連線
            for (int i = 0; i < 5; i++)
            {
                if (Ai_Btns[i].Name + Ai_Btns[5 + i].Name + Ai_Btns[10 + i].Name + Ai_Btns[15 + i].Name + Ai_Btns[20 + i].Name == "11111")
                {
                    for (int j = 0; j < 5; j++)
                        Ai_Btns[j * 5 + i].BackColor = Color.LightSkyBlue;
                    ai_link_line += 1;
                }
            }
            if (Ai_Btns[0].Name + Ai_Btns[6].Name + Ai_Btns[12].Name + Ai_Btns[18].Name + Ai_Btns[24].Name == "11111")
            {
                for (int j = 0; j < 5; j++)
                    Ai_Btns[j * 6].BackColor = Color.LightSkyBlue;
                ai_link_line += 1;
            }
            if (Ai_Btns[4].Name + Ai_Btns[8].Name + Ai_Btns[12].Name + Ai_Btns[16].Name + Ai_Btns[20].Name == "11111")
            {
                for (int j = 1; j < 6; j++)
                    Ai_Btns[j * 4].BackColor = Color.LightSkyBlue;
                ai_link_line += 1;
            }

            label5.Text = "電腦連線：" + ai_link_line;

            if (ai_link_line >= 3)
                ai_winlink = true;
            return ai_winlink;
        }
        private void AI()//電腦選擇的數字
        {
            int Ai_choose = 0;
            int Ai_choose_find = 0;
            int find_num = 0;
            int ai_find_num = 0;
            int W_max = 0, H_max = 0;
            int find_slash = 0;
            int[] All_Max = new int[4];
            int Temp_Max = 0;

            Click_num();
            W_max = Array.IndexOf(w, w.Max());
            H_max = Array.IndexOf(H, H.Max());
            Slash();
            if (slash_R == 5)
                slash_R = 0;
            if (slash_L == 5)
                slash_L = 0;

            All_Max[0] = w.Max();
            All_Max[1] = H.Max();
            All_Max[2] = slash_R;
            All_Max[3] = slash_L;
            Temp_Max = Array.IndexOf(All_Max, All_Max.Max());
            Console.WriteLine("temp_max：" + Temp_Max);

            if (Temp_Max == 2 || Temp_Max == 3)
            {
                if (slash_R >= slash_L)
                {
                    for (int i = 0; i < 5; i++)
                        if (Ai_Btns[i * 6].Name == "0")
                            break;
                        else
                            find_slash++;
                    Console.WriteLine("find_slash：" + find_slash);
                    if (find_slash == 5)
                        find_slash = 0;
                    Ai_choose = Ai_Random[find_slash * 6 / 5][find_slash * 6 % 5];
                    Console.WriteLine("SRAi_choose" + Ai_choose);
                }
                else
                {
                    for (int i = 4; i < 5; i++)
                        if (Ai_Btns[i * 4].Name == "0")
                            break;
                        else
                            find_slash++;
                    Console.WriteLine("find_slash：" + find_slash);
                    if (find_slash == 5)
                        find_slash = 0;
                    Ai_choose = Ai_Random[find_slash * 4 / 5][find_slash * 4 % 5];
                    Console.WriteLine("SLAi_choose" + Ai_choose);
                }
            }
            else
            {
                if (w.Max() >= H.Max())// 垂直的比較大
                {
                    for (int i = 0; i < 5; i++)
                    {
                        if (Ai_Btns[i + W_max * 5].Name == "0")
                            break;
                        else
                            Ai_choose_find++;

                        if (Ai_choose_find == 5 || w[i] == 5)
                        {
                            Ai_choose_find = 0;
                            w[i] = 0;
                        }
                    }
                    Ai_choose = Ai_Random[W_max][Ai_choose_find];
                    Console.WriteLine("1Ai_choose：" + Ai_choose);
                }
                else
                {
                    Ai_choose_find = 0;
                    for (int i = 0; i < 5; i++)
                    {
                        if (Ai_Btns[H_max + i * 5].Name == "0")
                            break;
                        else
                            Ai_choose_find++;

                        if (Ai_choose_find == 5 || H[i] == 5)
                        {
                            Ai_choose_find = 0;
                            H[i] = 0;
                        }
                    }
                    Ai_choose = Ai_Random[Ai_choose_find][H_max];
                    Console.WriteLine("2Ai_choose：" + Ai_choose);
                }
            }

            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 5; j++)
                    if (Ai_choose == Ai_Random[i][j] && Ai_Btns[j + i * 5].Name == "0")
                    {
                        foreach (Button find in play_Btns)
                        {
                            if (Ai_Btns[j + i * 5].Text != find.Text)
                                find_num++;
                            else
                                break;
                        }
                        foreach (Button find in Ai_Btns)
                        {
                            if (Ai_Btns[j + i * 5].Text != find.Text)
                                ai_find_num++;
                            else
                            {
                                find.ForeColor = Color.DodgerBlue;
                                find.Name = "1";
                                find.Tag = 1;
                                break;
                            }
                        }
                        play_Btns[find_num].ForeColor = Color.DarkOrange;
                        play_Btns[find_num].Name = "1";
                        play_Btns[find_num].Tag = 1;
                        label4.Text = "電腦選擇：" + Ai_Btns[j + i * 5].Text;
                    }
            Click_num();
        }
        private void Click_num()//計算直橫列被按下的加總
        {
            int W_click = 0;
            int H_click = 0;
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (Ai_Btns[j + i * 5].Name == "1")//計算直的加總
                    {
                        W_click++;
                        w[i] = W_click;
                    }
                    if (Ai_Btns[i + j * 5].Name == "1")//計算橫的加總
                    {
                        H_click++;
                        H[i] = H_click;
                    }
                }
                W_click = 0;
                H_click = 0;

                if (w[i] == 5 || H[i] == 5)
                {
                    w[i] = 0;
                    H[i] = 0;
                }
            }
        }
        private void Slash()//計算斜線加總
        {
            slash_R = 0;
            slash_L = 0;

            for (int i = 0, j = 4; i < 5; i++,j--)
            {
                if (Ai_Btns[i * 6].Name == "1")
                    slash_R++;
                if (Ai_Btns[j * 4].Name == "1")
                    slash_L++;
            }
        }
        public static List<List<int>> Random(int Rows, int Cols, int min, int max, bool state)
        {
            List<List<int>> list = new List<List<int>>();
            int[] temp_list = new int[max];
            Random rd;
            int temp = 0;

            for (int i = 0; i < Rows; i++)
            {
                list.Add(new List<int>());
                for (int j = 0; j < Cols; j++)
                    list[i].Add(0);
            }

            if (state) // 解決 亂數會相同的問題
                rd = new Random();
            else
                rd = new Random(DateTime.Now.Millisecond);

            for (int i = 0; i < max; i++)    //不重複亂數
            {
                temp = rd.Next(min, max + 1);
                for (int j = 0; j < max; j++)
                    while (temp == temp_list[j])
                    {
                        j = 0;
                        temp = rd.Next(min, max + 1);
                    }
                temp_list[i] = temp;
            }

            for (int i = 0; i < Rows; i++)
                for (int j = 0; j < Cols; j++)
                    list[i][j] = temp_list[j + i * 5];
            return list;
        }
        public static List<Button> game_display(int[,] Play_Random) // 玩家遊戲畫面
        {
            List<Button> Buttons = new List<Button>();
            for (int j = 0; j < 5; j++)
                for (int i = 0; i < 5; i++)
                {
                    Button btns = new Button();
                    btns.Text = Play_Random[j, i].ToString();
                    btns.Tag = 0;
                    btns.Name = "0";
                    btns.Height = 60;
                    btns.Width = 60;
                    btns.Location = new Point(60 + j * 70, 90 + i * 70);
                    btns.Font = new Font("微軟正黑體", 14);
                    btns.Font = new Font(btns.Font, FontStyle.Bold);
                    Buttons.Add(btns);
                }
            return Buttons;
        }
        public static List<Button> ai_game_display(int[,] Ai_Random) // 電腦遊戲畫面
        {
            List<Button> ai_Buttons = new List<Button>();
            for (int j = 0; j < 5; j++)
                for (int i = 0; i < 5; i++)
                {
                    Button ai_btns = new Button();
                    ai_btns.Text = Ai_Random[j, i].ToString();
                    ai_btns.Name = "0";
                    ai_btns.Height = 60;
                    ai_btns.Width = 60;
                    ai_btns.Location = new Point(480 + j * 70, 90 + i * 70);
                    ai_btns.Font = new Font("微軟正黑體", 14);
                    ai_btns.Font = new Font(ai_btns.Font, FontStyle.Bold);
                    ai_Buttons.Add(ai_btns);
                }
            return ai_Buttons;
        }
        //=============================事件================================
        private void button1_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 25 ; i++)
            {
                this.Controls.Remove(play_Btns[i]);
                this.Controls.Remove(Ai_Btns[i]);
            }
            initialize();
            button1.Enabled = false;
        }
    }
}
