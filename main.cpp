#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <graphics.h>

#define SIZE 40		//图片宽度，即每个格子的宽度，
#define N 5			//显示的记录条数
using namespace std;
int row = 10;		//格子行数
int col = 10;		//格子列数
int number = 10;	//地雷数量

IMAGE Img[20];//存储图片
int cnt = 0;
time_t TimeStart;//存储开始时间
time_t TimeEnd;//存储结束时间
int** NearBy = NULL;
char** Map = NULL;

//函数声明
void Menu();		//菜单
void StartGame();	//开始游戏

void InitGame();					//游戏初始化
void MouseClick();					//点击
void Islandmine(int x, int y);		//输入后数据处理
void Show();						//每次输入后要更新图片
void Judge(int x, int y);			//胜利或失败判断

void Expand(int x, int y);					//打开0后炸开一片的递归
void Protect(int x, int y);					//第一次不能点到雷处理
void Mark(int x, int y);					//右键标记

void LoadImages();//加载图片
void PrintNearBy();//打印下面的数字，测试专用
void Exit();//安全退出函数

void Setting();
void ShowRank();
void TextButton();
//文件部分
void ReadFile();
void SortRank();
void WriteFile();

typedef struct User
{
	int Level;	        	             
	char name[20];		                 
	int time;	                                         
}User;                                    

User st[N];
User CurrentUser;
//加载地图，包括随机埋地雷，并初始化NearBy，初始化map
void InitGame()
{
	initgraph(col * SIZE, row * SIZE, 1);
	//再始化()
	cnt = 0;
	//初始化
	NearBy = new int* [row + 2];
	for (int i = 0; i < row + 2; i++)
	{
		NearBy[i] = new int[col + 2];
	}

	Map = new char* [row + 2];
	for (int i = 0; i < row + 2; i++)
	{
		Map[i] = new char[col + 2];
	}
	//初始化
	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++)
		{
			NearBy[i][j] = 0;
		}
	}
	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++)
		{
			Map[i][j] = '*';
		}
	}

	//随机埋地雷
	srand((unsigned)time(NULL));	//注意该行代码不能放到循环内
	int x, y;						//存储雷的坐标
	for (int i = 0; i < number;)
	{
		x = rand() % row + 1;	//随机数x范围1~row
		y = rand() % col + 1;	//随机数y范围1~col
		if (NearBy[x][y] == 0)	//要考虑到重复埋雷的问题
		{
			NearBy[x][y] = -1;
			i++;
		}
	}

	//计算每个位置周围地雷数量
	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++) {
			if (NearBy[i][j] == -1)
			{
				for (int p = i - 1; p <= i + 1; p++)
				{
					for (int q = j - 1; q <= j + 1; q++)
					{
						if (NearBy[p][q] != -1)//不是地雷就++，(不用排除自己，因为自己就是地雷，不会通过这条语句)
						{
							NearBy[p][q]++;
						}
					}
				}
			}
		}
	}
	PrintNearBy();
	Show();
}

//开始游戏
void StartGame()
{
	InitGame();
	time(&TimeStart);
	MouseClick();
}


//判定是否是地雷，是否不是地雷
void Islandmine(int x, int y)
{
	//遇到已经打开的或者已经标记了的
	if (Map[x][y] == '@' || Map[x][y] != '*')
	{
		return;
	}
	else if (NearBy[x][y] == -1)
	{
		if (cnt == 0)
		{
			Protect(x, y);
			Map[x][y] = NearBy[x][y] + 48;//先打开
			Expand(x, y);//如果是0，就要特殊操作了
			PrintNearBy();
			Judge(x, y);//每次输入后都要判断是否胜利
		}
		else
		{
			Map[x][y] = NearBy[x][y] + 48;//先打开
			PrintNearBy();
			Judge(x, y);
		}

	}
	//不是地雷
	else
	{
		Map[x][y] = NearBy[x][y] + 48;//先打开
		Expand(x, y);//如果是0，就要特殊操作了
		PrintNearBy();
		Judge(x, y);//每次输入后都要判断是否胜利
	}
}

//右键标记
void Mark(int x, int y)
{
	if (Map[x][y] == '@')
	{
		Map[x][y] = '*';
		cout << "取消标记成功" << endl;
		Show();
	}
	else
	{
		Map[x][y] = '@';
		cout << "标记成功" << endl;
		Show();
	}
}

//递归点开格子
void Expand(int x, int y)
{
	//首先检验坐标是否在地图内
	if (x >= 1 && x <= row && y >= 1 && y <= col)
	{
		//如果为0，要递归
		if (NearBy[x][y] == 0)
		{
			//递归检查周围8个
			for (int i = x - 1; i <= x + 1; i++)
			{
				for (int j = y - 1; j <= y + 1; j++)
				{
					//没有被打开，并且不是自己
					if (Map[i][j] == '*' && (i != x || j != y))
					{
						cnt++;
						Map[i][j] = NearBy[i][j] + 48;//0旁边8个一定会被打开
						Expand(i, j);//递归调用旁边8个，旁边8个如果是0就和刚才一样，不是0,的判断就过不去，不会再递归调用，仅仅打开了自己
					}
				}
			}
		}
	}
}

//用户点击
void MouseClick()
{
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg))
		{
			int x = msg.y / SIZE + 1;
			int y = msg.x / SIZE + 1;
			switch (msg.message)
			{
			case WM_LBUTTONDOWN: Islandmine(x, y); break;
			case WM_RBUTTONDOWN: Mark(x, y); break;
			}
		}
	}
}



//第一次不能点到雷
void Protect(int x, int y)
{
	int a, b;
	srand((unsigned)time(NULL));	//注意该行代码不能放到循环内
	do
	{
		a = rand() % row + 1;	//随机数x范围1~row
		b = rand() % col + 1;	//随机数y范围1~col

	} while (NearBy[a][b] == -1);//找到一个没雷的位置把雷偷偷转移走

	NearBy[a][b] = -1;
	NearBy[x][y] = 0;
	//先把下面原来炸弹周围的点--
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if ((i != x || j != y) && NearBy[i][j] != -1)
			{
				NearBy[i][j]--;
			}
			if (NearBy[i][j] == -1)
			{
				NearBy[x][y]++;
			}
		}
	}

	//新位置加一个雷，周围++
	for (int i = a - 1; i <= a + 1; i++)
	{
		for (int j = b - 1; j <= b + 1; j++)
		{
			if ((i != x || j != y) && NearBy[i][j] != -1)
			{
				NearBy[i][j]++;
			}
		}
	}
}

//打印NearBy，这是测试代码！！！！！
void PrintNearBy()
{
	//system("cls");
	cout << "下图仅用于测试" << endl;
	for (int k = 0; k <= col; k++)
	{
		printf("%3d", k);
	}
	cout << endl;
	cout << "---------------------" << endl;
	for (int i = 1; i <= row; i++)
	{
		printf("%d |", i);
		for (int j = 1; j <= col; j++)
		{
			printf("%3d", NearBy[i][j]);
		}
		cout << endl;
	}
	cout << "---------------------" << endl;
	cout << cnt << endl;
}

//*****************************主函数***********************************
int main()
{
	LoadImages();
	initgraph(640, 640, 1);
	Menu();
	return 0;
}

//*****************************下面是涉及到图形化的代码***********************************



//显示图片1-8,空白,炸弹，棋子等图片！(根据map中的数字/符号显示对应图片)
void Show()
{
	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++)
		{
			//如果没有被翻开就放置图片10
			if (Map[i][j] == '*')
			{
				putimage(j * SIZE - SIZE, i * SIZE - SIZE, &Img[10]);
			}
			//如果数字被翻开了就显示对应图片
			else if (Map[i][j] >= 48 && Map[i][j] <= 56)
			{
				for (int k = 0; k < 9; k++)
				{
					if (Map[i][j] == k + 48)
					{
						putimage(j * SIZE - SIZE, i * SIZE - SIZE, &Img[k]);
					}
				}

			}
			//如果翻开了地雷就显示地雷图片
			else if (Map[i][j] != '*' && Map[i][j] != '@' && NearBy[i][j] == -1)
			{
				putimage(j * SIZE - SIZE, i * SIZE - SIZE, &Img[9]);
			}
			//如果被标记了就显示标记图片
			else if (Map[i][j] == '@')
			{
				putimage(j * SIZE - SIZE, i * SIZE - SIZE, &Img[11]);
			}
		}
	}
}

//加载图片
void LoadImages()
{
	//设置顶部文字
	HWND hnd = GetHWnd();
	SetWindowText(hnd, "不会玩扫雷");
	char path[50];
	for (int i = 0; i < 12; i++)
	{
		sprintf(path, "./images/%d.jpg", i);
		loadimage(&Img[i], path, SIZE, SIZE);
	}
}

//胜利或失败判断，每输入一次后判断一下
//目前的判断胜利逻辑是检测地图上有几个没有被翻开，没有被翻开的数目=雷数才能获胜，会不会遇到bug？
void Judge(int x, int y)
{
	Show();
	int Count = 0;
	if (NearBy[x][y] == -1)
	{
		HWND choice = GetHWnd();
		int ch = MessageBox(choice, "蹦蹦炸弹！是否再来一局？", "失败", MB_OKCANCEL);
		if (ch == IDOK)
		{
			StartGame();
		}
		else
		{
			initgraph(640, 640, 1);
			Menu();
		}
	}
	else
	{
		for (int i = 1; i <= row; i++)
		{
			for (int j = 1; j <= col; j++)
			{
				if (Map[i][j] == '@' || Map[i][j] == '*')
				{
					Count++;
				}
			}
		}
	}
	cout << Count << endl;
	if (Count == number)
	{
		HWND choice = GetHWnd();
		time(&TimeEnd);
		int time = TimeEnd - TimeStart;
		CurrentUser.time = time;
		char str[50];
		sprintf(str, "您获胜了，耗时:%ds，是否再玩一次", time);
		ReadFile();
		SortRank();
		WriteFile();
		int ch = MessageBox(choice, str, "很好", MB_OKCANCEL);
		if (ch == IDOK)
		{
			StartGame();
		}
		else
		{
			initgraph(640, 640, 1);
			Menu();
		}
	}
}

void Exit()
{
	//释放new出来的
	for (int i = 0; i < row; i++)
	{
		delete[] NearBy[i];
	}
	delete[] NearBy;

	for (int i = 0; i < row; i++)
	{
		delete[] Map[i];
	}
	delete[] Map;
	//关闭窗口和程序
	closegraph();
	exit(0);
}

void Menu()
{
	setbkcolor(WHITE);
	cleardevice();
	TextButton();
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg, EM_MOUSE))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				if (msg.x >= 40 && msg.x <= 200 && msg.y >= 250 && msg.y <= 290)
				{
					cleardevice();
					StartGame();
				}
				if (msg.x >= 40 && msg.x <= 120 && msg.y >= 325 && msg.y <= 365)
				{
					int isok = InputBox(CurrentUser.name, 20, "请输入您的用户名，至多20字符", "登录", NULL, 0, 0, false);
					if (isok == 1)
					{
						HWND hwnd = GetHWnd();
						MessageBox(hwnd, "登录成功", "提示", MB_OK);
						outtextxy(100, 20, CurrentUser.name);
					}

				}
				if (msg.x >= 40 && msg.x <= 120 && msg.y >= 400 && msg.y <= 440)
				{
					Setting();
				}
				if (msg.x >= 40 && msg.x <= 120 && msg.y >= 475 && msg.y <= 515)
				{
					ShowRank();
				}
				if (msg.x >= 40 && msg.x <= 120 && msg.y >= 550 && msg.y <= 590)
				{
					Exit();
				}
			}
		}
	}

}
void TextButton()
{
	settextcolor(BLACK);
	settextstyle(40, 0, "VonwaonBitmap 16px");
	outtextxy(40, 250, "开始游戏");
	outtextxy(40, 325, "登录");
	outtextxy(40, 400, "设置");
	outtextxy(40, 475, "排行");
	outtextxy(40, 550, "退出");
}
void Setting()
{
	cleardevice();
	settextcolor(BLACK);
	settextstyle(40, 0, "VonwaonBitmap 16px");
	outtextxy(40, 50, "难度:");
	outtextxy(180, 50, "简单");
	outtextxy(300, 50, "普通");
	outtextxy(420, 50, "困难");
	//outtextxy(40, 125, "皮肤:");
	outtextxy(500, 540, "返回");
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg, EM_MOUSE))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				if (msg.x >= 180 && msg.x <= 260 && msg.y >= 50 && msg.y <= 90)
				{
					CurrentUser.Level = 1;
					row = 8;
					col = 8;
					number = 5;
					MessageBox(NULL, "简单模式设置成功", "提示", MB_OK);
				}
				if (msg.x >= 300 && msg.x <= 380 && msg.y >= 50 && msg.y <= 90)
				{
					CurrentUser.Level = 2;
					row = 12;
					col = 12;
					number = 15;
					MessageBox(NULL, "普通模式设置成功", "提示", MB_OK);
				}
				if (msg.x >= 420 && msg.x <= 500 && msg.y >= 50 && msg.y <= 90)
				{
					CurrentUser.Level = 3;
					row = 20;
					col = 20;
					number = 60;
					MessageBox(NULL, "困难模式设置成功", "提示", MB_OK);
				}
				if (msg.x >= 500 && msg.x <= 580 && msg.y >= 540 && msg.y <= 580)
				{
					Menu();
				}
			}
		}
	}
}
void ShowRank()
{
	cleardevice();
	outtextxy(500, 540, "返回");
	settextstyle(20, 0, "VonwaonBitmap 12px");
	ReadFile();
	for (int i = 0; i < N; i++)
	{
		char time[20];
		sprintf(time, "%d", st[i].time);
		outtextxy(30, 50 + 30 * i, st[i].name);
		outtextxy(100, 50 + 30 * i, time);
	}
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg,EX_MOUSE))
		{
			if (msg.message==WM_LBUTTONDOWN)
			{
				if (msg.x >= 500 && msg.x <= 580 && msg.y >= 540 && msg.y <= 580)
				{
					Menu();
				}
			}
		}
	}
}

//*************************文件部分*************************

//读入排行榜文件中的数据，以进行随后的数据比较
void ReadFile()
{
	int i = 0;                       /*此处i应初始化为0*/
	FILE* fp;                        /*定义文件指针*/
	fp = fopen("./rank.txt", "r");     /*打开文本文件准备读入数据*/
	if (fp == 0)                     /*如果文件打开失败 */
	{
		printf("source file error\n");
		exit(1);
	}
	while (!feof(fp))               /*当文件未结束时进入循环*/
	{
		/*结构体数组的下标加1然后继续读入*/
		fscanf(fp, "%d%s%d", &st[i].Level, st[i].name, &st[i].time);
		i++;
	}
	for (int i = 0; i < N; i++)
	{
		printf("%d%s%d\n", st[i].Level, st[i].name, st[i].time);
	}
	fclose(fp);                     /*关闭文件*/
}
//数据比较
void SortRank()
{	
	int i = N - 1;
	for ( i = N - 1; i >= 0;)
	{
		if (CurrentUser.time < st[i].time)
		{
			i--;
		}
	}
	i++;
	//替换数据
	if (i < N)
	{
		strcpy(st[i].name, CurrentUser.name);
		st[i].time = CurrentUser.time;
		cout << "替换数据成goon" << endl;
	}
	
}
//数据重新写入
void WriteFile()
{
	int i;
	FILE* fp;
	fp = fopen("./rank.txt", "w");     /*以w方式打开文本文件准备写入内容*/
	if (fp == 0)	      	    /* 如果文件打开失败 */
	{
		printf("create new file error\n");
		exit(1);
	}
	for (i = 0; i < N; i++)           /*记录条数n确知时可以这样控制循环*/
	{  /*下面两条语句以相同的格式将记录分别写入文件和屏幕显示*/
		fprintf(fp, "%d %s %d\n", st[i].Level, st[i].name, st[i].time);
	}
	fclose(fp);              /*关闭文件*/
}


