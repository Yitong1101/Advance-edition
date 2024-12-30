using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.Common;
using System.Drawing;
using System.Linq;
using System.Reflection.Emit;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace candy
{
    public partial class Form1 : Form
    {
        //============================變數宣告===============================
        static int W = 10;              // 糖果的寬
        static int H = 10;              // 糖果的高
        int style = 0;                  // 糖果的類型有幾種
        int click = 0;                  // 計算第幾次按下圖片
        int [,] pic_rd;                 // 裝糖果亂數的陣列
        int [,] pic_temp;               // 暫存亂數糖果陣列
        int index1 = 0, index2 = 0;     // 糖果交換時的第一個位置和第二個位置
        PictureBox[,] pictureBox;       // 裝圖片的陣列
        Random rd = new Random();       // 產生隨機糖果
        bool match = false;             // 移動是否有效
        bool deadlock = true;           // 判斷是否有三消
        int temp1, temp2;               // 暫存假交換的糖果位置
        bool start = false;             // 開始遊戲
        bool deadlockShow = false;      // 死局訊息只出現一次
        int sec = 60;                   // 遊戲秒數
        int score = 0;                  // 遊戲分數

        public Form1()
        {
            InitializeComponent();
        }

        //============================自訂函式===============================
        private void initial()
        {
            score = 0;
            sec = 60;
            label1.Visible = true;
            label2.Visible = true;
            label3.Visible = true;
            textBox1.Visible = true;
            textBox2.Visible = true;
            textBox3.Visible = true;
            button1.Visible = true;
            start = false;
            label4.Text = "時間：" + sec;
            label5.Text = "分數：" + score;
            timer1.Stop();
            timer2.Stop();
        }
        private void delete()
        {
            for (int i = 0; i < W; i++)
                for (int j = 0; j < H; j++)
                    this.Controls.Remove(pictureBox[i, j]);
            Array.Clear(pictureBox, 0, pictureBox.Length);
            Array.Clear(pic_rd, 0, pic_rd.Length);
            Array.Clear(pic_temp, 0, pic_temp.Length);
        }
        private void Game_screen()  // 遊戲開始畫面
        {
            try
            {
                W = Convert.ToInt32(textBox1.Text);      // 取得使用者輸入的寬
                H = Convert.ToInt32(textBox2.Text);      // 取得使用者輸入的高
                style = Convert.ToInt32(textBox3.Text);  // 取得使用者輸入的類型

                if(6 < W || W < 3 || 6 < H || H < 3 || 6 < style || style < 2)
                    return;
                else
                    start = true;

                pictureBox = new PictureBox[W, H];      // 建立圖片陣列
                pic_rd = new int[W, H];                 // 建立圖片亂數
                pic_temp = new int[W, H];
                random();

                for (int i = 0; i < W; i++)
                    for (int j = 0; j < H; j++)
                    {
                        pictureBox[i, j] = new PictureBox();
                        pictureBox[i, j].Image = imageList1.Images[pic_rd[i, j]];
                        pictureBox[i, j].Tag = 0;
                        pictureBox[i, j].SizeMode = PictureBoxSizeMode.AutoSize;
                        pictureBox[i, j].Location = new Point(0 + j * 50, 0 + i * 50);
                        pictureBox[i, j].Click += OnClick;
                        this.Controls.Add(pictureBox[i, j]);
                    }
                label1.Visible = false;
                label2.Visible = false;
                label3.Visible = false;
                textBox1.Visible = false;
                textBox2.Visible = false;
                textBox3.Visible = false;
                button1.Visible = false;
                Match();
                timer2.Start();

            }
            catch
            {
                MessageBox.Show("長、寬與樣式不可為空值");
            }
        }
        private void random()           // 隨機產生糖果圖片
        {
            for (int i = 0; i < W; i++)
                for (int j = 0; j < H; j++)
                {
                    pic_rd[i, j] = rd.Next(0, style);
                    pic_temp[i, j] = pic_rd[i, j];
                }
        }
        private void OnClick(object sender, EventArgs e)
        {
            int index = 0;          // 圖片位置
            bool change = false;    // 判斷是否可以交換
            int temp;               // 暫存位置
            click++;
            match = false;

            PictureBox picture = sender as PictureBox;  // 取得按下的是哪個圖片
            picture.BorderStyle = BorderStyle.Fixed3D;

            
            foreach (PictureBox pic_index in pictureBox) // 尋找在圖片陣列的第幾個
            {
                if (picture != pic_index)
                    index++;
                else
                    break;
            }

            if (click == 1)
            {
                index1 = index;  // 將第一個按下的圖片陣列位置存在index1       
            }
            else
            {
                index2 = index;  // 將第二個按下的圖片陣列位置存在index2

                for (int i = 0; i < W; i++)
                    for (int j = 0; j < H; j++)
                        pictureBox[i, j].BorderStyle = BorderStyle.None;

                if (index2 == index1 + 1 || index2 == index1 - 1 || index2 == index1 + H || index2 == index1 - H)
                    change = true;
            }

            if (change)
            {
                temp = pic_rd[index1 / H, index1 % H];  // 交換index1和index2
                pic_rd[index1 / H, index1 % H] = pic_rd[index2 / H, index2 % H];
                pic_rd[index2 / H, index2 % H] = temp;

                for (int i = 0; i < W; i++)
                {
                    for (int j = 0; j < H; j++)
                    {
                        pic_temp[i, j] = pic_rd[i, j];
                    }
                }

                Match();
                if (!match)
                {
                    temp = pic_rd[index1 / H, index1 % H];
                    pic_rd[index1 / H, index1 % H] = pic_rd[index2 / H, index2 % H];
                    pic_rd[index2 / H, index2 % H] = temp; 
                    for (int i = 0; i < W; i++)
                    {
                        for (int j = 0; j < H; j++)
                        {
                            pic_temp[i, j] = pic_rd[i, j];
                        }
                    }
                }

                textBox4.Clear();
                for (int i = 0; i < W; i++) // 更新圖片
                {
                    for (int j = 0; j < H; j++)
                    {
                        pictureBox[i, j].Image = imageList1.Images[pic_rd[i, j]];
                        pictureBox[i, j].BorderStyle = BorderStyle.None;
                        textBox4.AppendText(pic_rd[i, j].ToString());
                    }
                    textBox4.AppendText("\r\n");
                }
            }

            if (click == 2)
            {
                click = 0;
                index1 = 0;
                index2 = 0;
                picture.BorderStyle = BorderStyle.None;
                Match();
                Newcandy();
            }
        }

        private void Match()
        {
            for (int row = 0; row < W; row++)   // 檢查水平
            {
                for (int column = 0; column < H - 2; column++)
                {
                    int NowCandy = pic_temp[row, column]; // 目前的糖果

                    if (NowCandy == pic_temp[row, column + 1] && NowCandy == pic_temp[row, column + 2])
                    {
                        timer1.Start();
                        match = true;
                        score += 30;
                        label5.Text = "分數：" + score;
                        pictureBox[row, column].Image = imageList1.Images[7];
                        pictureBox[row, column + 1].Image = imageList1.Images[7];
                        pictureBox[row, column + 2].Image = imageList1.Images[7];
                        pic_rd[row, column] = 7;
                        pic_rd[row, column + 1] = 7;
                        pic_rd[row, column + 2] = 7;
                        pictureBox[row, column].Tag = 1;
                        pictureBox[row, column + 1].Tag = 1;
                        pictureBox[row, column + 2].Tag = 1;
                    }
                }
            }

            for (int column = 0; column < H; column++)  // 檢查垂直
            {
                for (int row = 0; row < W - 2; row++)
                {
                    int NowCandy = pic_temp[row, column]; // 目前的糖果

                    if (NowCandy == pic_temp[row + 1, column] && NowCandy == pic_temp[row + 2, column])
                    {
                        timer1.Start();
                        match = true;
                        score += 30;
                        label5.Text = "分數：" + score;
                        pictureBox[row, column].Image = imageList1.Images[7];
                        pictureBox[row + 1, column].Image = imageList1.Images[7];
                        pictureBox[row + 2, column].Image = imageList1.Images[7];
                        pic_rd[row, column] = 7;
                        pic_rd[row + 1, column] = 7;
                        pic_rd[row + 2, column] = 7;
                        pictureBox[row, column].Tag = 1;
                        pictureBox[row + 1, column].Tag = 1;
                        pictureBox[row + 2, column].Tag = 1;
                    }
                }
            }
        }
        private bool IsFalling()
        {
            for (int row = 0; row < W; row++)
            {
                for (int col = 0; col < H; col++)
                {
                    if (pic_rd[row, col] == 7 && pictureBox[row,col].Tag.ToString() == "1")
                    {
                        return true; // 如果有糖果正在下降，返回 true
                    }
                }
            }
            return false; // 無糖果下降，返回 false
        }
        private void CheckDeadlock() // 判斷是否死局
        {
            deadlock = true;
            
            for (int row = 0; row < W; row++)
                for (int col = 0; col < H; col++)
                {
                    if (col > 0)                        
                        fakechange(row, col, 0, 0, 0, 1);   // 檢查左方是否有相同的糖果

                    if (col < H - 1) 
                        fakechange(row, col, 0, 0, 1, 0);   // 檢查右方是否有相同的糖果

                    if (row > 0)     
                        fakechange(row, col, 0, 1, 0, 0);   // 檢查上方是否有相同的糖果

                    if (row < W - 1) 
                        fakechange(row, col, 1, 0, 0, 0);   // 檢查下方是否有相同的糖果
                }

            if (deadlock)   // 判斷到死局
            {
                timer2.Stop();
                if (!deadlockShow)
                {
                    deadlockShow = true;
                    timer2.Stop();
                    MessageBox.Show("死局，將重新開始！");

                    for (int i = 0; i < W; i++)
                        for (int j = 0; j < H; j++)
                            this.Controls.Remove(pictureBox[i, j]);

                    Game_screen();
                    Match();
                    deadlockShow = false;
                    CheckDeadlock();
                }
            }

        }
        void fakechange(int x, int y, int x1, int x2, int y1, int y2)   // 假交換        
        {
            temp1 = pic_rd[x, y];
            temp2 = pic_rd[x + x1 - x2, y + y1 - y2];

            pic_rd[x, y] = temp2;
            pic_rd[x + x1 - x2, y + y1 - y2] = temp1;

            isdead();

            pic_rd[x, y] = temp1;
            pic_rd[x + x1 - x2, y + y1 - y2] = temp2;
        }
        void isdead()  //判斷下一步是否可達成連線
        {
            for (int i = 0; i < W; i++)
                for (int j = 0; j < H - 2; j++)
                    if (pic_rd[i, j] == pic_rd[i, j + 1] && pic_rd[i, j] == pic_rd[i, j + 2])
                        deadlock = false;

            for (int j = 0; j < H; j++)
                for (int i = 0; i < W - 2; i++)
                    if (pic_rd[i, j] == pic_rd[i + 1, j] && pic_rd[i, j] == pic_rd[i + 2, j])
                        deadlock = false;
        }
        private void Newcandy() // 產生新的糖果
        {
            for (int i = 0; i < W; i++)
            {
                for (int j = 0; j < H; j++)
                {
                    pictureBox[i, j].Image = imageList1.Images[pic_rd[i, j]];
                }
            }
        }
        //=============================事件================================
        private void timer1_Tick(object sender, EventArgs e)
        {
            Newcandy();
            for (int col = 0; col < H; col++)
            {
                for (int row = W - 1; row >= 0; row--)
                {
                    if (pic_rd[row, col] == 7 && row != 0)                    
                    {
                        pic_rd[row, col] = pic_rd[row - 1, col];
                        pictureBox[row, col].Image = pictureBox[row - 1, col].Image;
                        pic_rd[row - 1, col] = 7;
                        pictureBox[row, col].Tag = pictureBox[row - 1, col].Tag;
                    }
                    else if (pic_rd[row, col] == 7 && row == 0)
                    {
                        pic_rd[row, col] = rd.Next(0, style);
                        pictureBox[row, col].Image = imageList1.Images[pic_rd[row, col]];
                        pictureBox[row, col].Tag = 0;
                    }
                    for (int i = 0; i < W; i++)
                    {
                        for (int j = 0; j < H; j++)
                        {
                            pic_temp[i, j] = pic_rd[i, j];
                        }
                    }
                }
            }

            if (!IsFalling()) // 判斷糖果是否還在下降
            {
                timer1.Stop();      // 糖果下降結束，停止計時器
                CheckDeadlock();    // 判斷是否陷入死局
                Match();
            }
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            DialogResult result;    // 玩家的選擇
            DialogResult end_result;
            sec--;
            label4.Text = "時間：" + sec.ToString();
            if(sec == 0)
            {
                start = false;
                timer1.Stop();
                timer2.Stop();
                result = MessageBox.Show("時間到，遊戲結束！再玩一場？", "", MessageBoxButtons.YesNo);
                if (result == DialogResult.Yes)
                {
                    delete();
                    initial();
                }
                else
                {
                    end_result = MessageBox.Show("確定要離開遊戲嗎？", "", MessageBoxButtons.YesNo);
                    if (end_result == DialogResult.No)
                    {
                        delete();
                        initial();
                    }
                    else
                        this.Close();
                }
                    
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Game_screen();
            if (start)
            {
                timer1.Start();
                Match();
                CheckDeadlock();
            }
            else
                MessageBox.Show("請重新輸入長、寬、樣式");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            textBox4.Clear();
            for (int i = 0; i < W; i++)
            {
                for (int j = 0; j < H; j++)
                {
                    textBox4.AppendText(pictureBox[i, j].Tag.ToString());
                }
                textBox4.AppendText("\r\n");
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            textBox4.Clear();
            for (int i = 0; i < W; i++)
            {
                for (int j = 0; j < H; j++)
                {
                    Console.WriteLine(pictureBox[i, j].Location);
                    textBox4.AppendText(pic_rd[i, j].ToString());
                }
                textBox4.AppendText("\r\n");
            }
        }
        private void button4_Click(object sender, EventArgs e)
        {
            DialogResult result;
            
            timer2.Stop();
            result = MessageBox.Show("確定要離開遊戲嗎？", "", MessageBoxButtons.YesNo);
            if (result == DialogResult.No)
            {
                timer2.Start();
                if(start)
                    delete();
                initial();
            }
            else
                this.Close();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (start)
                delete();
            initial();
        }
    }
}
