#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <cstdio>
//#include <graphics.h>//easyx
using namespace std;

double TimeStart, TimeEnd;
const int row = 5;	//地图行数
const int col = 5;	//地图列数
int difficulty = 1; //难度默认1，一共1，2，3三个难度
int Landmine = 5;	//地雷数量
int Clear = 0;		//清除地雷数量
int Count = row * col - Landmine;//排雷次数，就是格子数减去雷数
int NearBy[row + 2][col + 2];	//二维数组每个位置记录周围8个方块地雷数量(-1是地雷)
//注意：全局数组里所有数据都是0，后面不必再手动赋值0
//细节：数组长宽比地图行列数都多了2，也就是数组范围从0~row+1和0~col+1，但实际使用范围只用到1~row和1~col，前后多出来的1位防止后面计算NearBy时的数组越位问题(详细说就是计算时每次遇到地雷，都要将地雷周围8个格子对应的数字++，那么如果地雷在边角，地雷周围8个格子就不全在地图内了，这时如果不扩大数组，数组就会越位，强行防止越位需要写一大堆判断语句)

char Map[row + 2][col + 2];//显示给用户的地图

struct user
{
	string name;
	int difficulty;
	int seconds;
};


//函数声明
void LoadMap();
void PrintNearBy();
void PrintMap();
void Islandmine(int flag, int x, int y);
void Input();
void Menu();
void open(int x, int y);
void GameOver();		//游戏结束！ 
void Login();
void StartGame();

//加载地图，包括随机埋地雷，并初始化NearBy，初始化map
void LoadMap()
{
	//随机埋地雷
	srand((unsigned)time(NULL));	//注意该行代码不能放到循环内
	int x, y;						//存储雷的坐标
	for (int i = 0; i < Landmine;)
	{
		x = rand() % row + 1;	//随机数范围1~row
		y = rand() % col + 1;	//随机数范围1~col
		if (NearBy[x][y] == 0)	//如果这个位置没有被埋雷，其实就是要考虑到重复埋雷的问题
		{
			NearBy[x][y] = -1;
			i++;
		}
	}


	//计算每个位置周围地雷数量

	//dx[k]和dy[k] 代表周围8个方块x、y方向上的坐标变化
	int dx[8] = { -1,-1,-1,0,0,1,1,1 }, dy[8] = { -1,0,1,-1,1,-1,0,1 };

	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++) {
			if (NearBy[i][j] == -1)
			{
				for (int k = 0; k < 8; k++)
				{
					if (NearBy[i + dx[k]][j + dy[k]] != -1)
						NearBy[i + dx[k]][j + dy[k]]++;
				}
			}
		}
	}
	//初始化地图map
	memset(Map, '*', sizeof(Map));
	PrintMap();

}

//打印NearBy
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
}

//打印Map
void PrintMap()
{
	system("cls");
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
			printf("%3c", Map[i][j]);
		}
		cout << endl;
	}
	cout << "---------------------" << endl;
}



//判定是否是地雷，是否不是地雷
void Islandmine(int flag, int x, int y)
{
	//标记地雷
	if (flag == 1)
	{
		//检查是否已经被标记过
		if (Map[x][y] = '@')
		{
			cout << "您已经标记过了" << endl;

		}
		Map[x][y] = '@';
		cout << "标记成功" << endl;
		PrintMap();
	}
	//模拟扫雷游戏左键单击
	else
	{
		//遇到地雷了
		if (NearBy[x][y] == -1)
		{
			GameOver();
		}
		//是空白，还需要递归点开一片
		else if (NearBy[x][y] == 0)
		{
			open(x, y);
			PrintMap();
			PrintNearBy();
		}
		//是1-8的数字
		else
		{
			open(x, y);
			PrintMap();
			PrintNearBy();
		}
	}
}
//递归点开格子
void open(int x, int y)
{
	//首先检验坐标是否在地图内
	if (x >= 1 && x <= row && y >= 1 && y <= col)
	{
		Count--;
		//如果为0，要递归
		if (NearBy[x][y] == 0)
		{

			//递归检查周围8个是否还有空格子
			for (int i = x - 1; i <= x + 1; i++)
			{
				for (int j = y - 1; j <= y + 1; j++)
				{
					{
						//周围8个：如果是0且没有被排
						if (NearBy[i][j] == 0 && Map[i][j] == '*')//必须要防止重复，否则死循环
						{
							Map[x][y] = 48;
							open(i, j);
						}
						//?下面可能有问题
						else
						{
							Map[i][j] = NearBy[i][j] + 48;
						}
					}

				}
			}
		}
		//如果不为0，不用递归
		else
		{
			Map[x][y] = NearBy[x][y] + 48; Count--;
			return;
		}
	}
}
//用户输入
void Input()
{
	int flag, x, y;
	cout << "请输入: 1.标记地雷 0.标记不是地雷，再输入坐标 ,格式：0 1 2" << endl;
	scanf_s("%d%d%d", &flag, &x, &y);

	//判断坐标是否输入正确
	if (x >= 1 && x <= row && y >= 1 && y <= col && Map[x][y] == '*')
	{
		if (Map[x][y])
			cout << "输入正确" << endl;
		Islandmine(flag, x, y);
	}
	//坐标输入错误
	else
	{
		cout << "坐标错误，可能是越过地图了或者您已经点开了该点" << endl;
		Input();
	}

}




int main()
{
	Menu();
	return 0;
}

//排行榜相关
void rank() {

}

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
	case 3:exit(0); break;
	default:
		break;
	}
}
//游戏失败
void GameOver()
{
	cout << "踩到地雷，游戏失败" << endl;
	PrintNearBy();
	cout << "是否再来一把？y/n" << endl;
	char ch;
	if ((ch = getchar()) == 'y')
	{
		StartGame();
	}
	else
	{
		exit(0);
	}
}
//登录
void Login()
{
	cout << "请输入用户名" << endl;
	string n;
	cin >> n;
	struct user user1;
	user1.name = n;
	Menu();
}
//开始游戏
void StartGame()
{
	LoadMap();
	while (Count != 0)
	{
		Input();
	}
	cout << "获胜" << endl;

}
