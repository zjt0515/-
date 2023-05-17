#include <iostream>
#include <ctime>
#include <cstdio>
#include <easyx.h>
#include <cstdlib>
#define LANDMINE_NUM 15	//地雷数量
#define LENGTH 40		//图片长度，即每个格子的长度
#define WIDTH 40		//图片宽度，即每个格子的宽度
using namespace std;
int ROW = 10;
int COL = 10;
int difficulty = 1; //难度默认1，一共1，2，3三个难度

IMAGE Img[20];//保存图片
int cnt = 0;
time_t start;//存储开始时间
time_t end1;//存储结束时间
int** NearBy = NULL;
char** Map = NULL;


//未实现（写着玩的）
struct user
{
	string name;
	int difficulty;
	int seconds;
};


//函数声明
void Menu();		//菜单
void Login();		//登录
void StartGame();	//开始游戏

void LoadMap();						//加载地图
void MouseClick();					//点击
void Islandmine(int x, int y);		//输入后数据处理
void Show();						//每次输入后要更新图片
void Judge(int x, int y);			//胜利或失败判断

void Expand(int x, int y);					//打开0后炸开一片的递归
void Protect(int x, int y);					//第一次不能点到雷处理
void Mark(int x, int y);					//右键标记

void LoadImages();//加载图片
void PrintNearBy();//打印下面的数字，测试专用

//加载地图，包括随机埋地雷，并初始化NearBy，初始化map
void LoadMap()
{
	//初始化
	NearBy = new int* [ROW+2];
	for (int i = 0; i < ROW+2; i++)
	{
		NearBy[i] = new int[COL+2];
	}

	Map = new char* [ROW+2];
	for (int i = 0; i < ROW+2; i++)
	{
		Map[i] = new char[COL+2];
	}
	//初始化
	for (int i = 1; i <= ROW; i++)
	{
		for (int j = 1; j <= COL; j++)
		{
			NearBy[i][j] = 0;
		}
	}
	for (int i = 1; i <= ROW; i++)
	{
		for (int j = 1; j <= COL; j++)
		{
			Map[i][j] = '*';
		}
	}

	//随机埋地雷
	srand((unsigned)time(NULL));	//注意该行代码不能放到循环内
	int x, y;						//存储雷的坐标
	for (int i = 0; i < LANDMINE_NUM;)
	{
		x = rand() % ROW + 1;	//随机数x范围1~row
		y = rand() % COL + 1;	//随机数y范围1~col
		if (NearBy[x][y] == 0)	//要考虑到重复埋雷的问题
		{
			NearBy[x][y] = -1;
			i++;
		}
	}


	//计算每个位置周围地雷数量
	for (int i = 1; i <= ROW; i++)
	{
		for (int j = 1; j <= COL; j++) {
			if (NearBy[i][j] == -1)
			{
				for (int p = i-1; p <=i+1; p++)
				{
					for (int q = j-1; q <=j+1; q++)
					{
						if (NearBy[p][q] != -1)
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
	//重玩游戏时需要对全局里的一些变量重置
	cnt = 0;
	LoadMap();
	time(&start);
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
	if (x >= 1 && x <= ROW && y >= 1 && y <= COL)
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
			int x = msg.y / WIDTH + 1;
			int y = msg.x / LENGTH + 1;
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
		a = rand() % ROW + 1;	//随机数x范围1~row
		b = rand() % COL + 1;	//随机数y范围1~col

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
	for (int k = 0; k <= COL; k++)
	{
		printf("%3d", k);
	}
	cout << endl;
	cout << "---------------------" << endl;
	for (int i = 1; i <= ROW; i++)
	{
		printf("%d |", i);
		for (int j = 1; j <= COL; j++)
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
	initgraph(LENGTH * COL, WIDTH * ROW,EX_SHOWCONSOLE);
	LoadImages();
	//Menu();
	StartGame();
	closegraph();
	delete[] Map, NearBy;
	return 0;
}

//*****************************下面是涉及到图形化的代码***********************************


//游戏菜单
void Menu()
{
	int choice;
	cout << "	请输入下面选项的对应序号" << endl;
	cout << "           1.登录           " << endl;
	cout << "           2.开始游戏           " << endl;
	cout << "           3.退出           " << endl;
	cin >> choice;
	switch (choice)
	{
	case 1:Login(); break;
	case 2:StartGame(); break;
	case 3:exit(1); break;
	default:
		break;
	}
}



//登录（未完成,写着玩的）
void Login()
{
	cout << "请输入用户名" << endl;
	string n;
	cin >> n;
	struct user user1;
	user1.name = n;
	cout << "登录成功" << endl;
	Menu();
}

//排行榜相关(未完成)
void rank() {

}

//显示图片1-8,空白,炸弹，棋子等图片！(根据map中的数字/符号显示对应图片)
void Show()
{
	for (int i = 1; i <= ROW; i++)
	{
		for (int j = 1; j <= COL; j++)
		{
			//如果没有被翻开就放置图片10
			if (Map[i][j] == '*')
			{
				putimage(j * 40 - 40, i * 40 - 40, &Img[10]);
			}
			//如果数字被翻开了就显示对应图片
			else if (Map[i][j] >= 48 && Map[i][j] <= 56)
			{
				for (int k = 0; k < 9; k++)
				{
					if (Map[i][j] == k + 48)
					{
						putimage(j * 40 - 40, i * 40 - 40, &Img[k]);
					}
				}

			}
			//如果翻开了地雷就显示地雷图片
			else if (Map[i][j] != '*' && Map[i][j] != '@' && NearBy[i][j] == -1)
			{
				putimage(j * 40 - 40, i * 40 - 40, &Img[9]);
			}
			//如果被标记了就显示标记图片
			else if (Map[i][j] == '@')
			{
				putimage(j * 40 - 40, i * 40 - 40, &Img[11]);
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
		loadimage(&Img[i], path, LENGTH, WIDTH);
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
			closegraph();
		}
	}
	else
	{
		for (int i = 1; i <= ROW; i++)
		{
			for (int j = 1; j <= COL; j++)
			{
				if (Map[i][j] == '@' || Map[i][j] == '*')
				{
					Count++;
				}
			}
		}
	}
	cout << Count << endl;
	if (Count == LANDMINE_NUM)
	{
		HWND choice = GetHWnd();
		time(&end1);
		int i = end1 - start;
		char str[50];
		sprintf(str, "您获胜了，耗时:%ds，是否在玩一次", i);
		int ch = MessageBox(choice, str, "很好", MB_OKCANCEL);
		if (ch == IDOK)
		{
			StartGame();
		}
		else
		{
			closegraph();
		}
	}
}
