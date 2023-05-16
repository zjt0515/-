#include <iostream>
#include <ctime>
#include <cstdio>
#include <easyx.h>
#include <cstdlib>
#define ROW 5				//地图行数
#define COL 5				//地图列数
#define LANDMINE_NUM 2	//地雷数量
using namespace std;

double TimeStart, TimeEnd;

int difficulty = 1; //难度默认1，一共1，2，3三个难度

int NearBy[ROW + 2][COL + 2];	//二维数组每个位置记录周围8个方块地雷数量(-1是地雷)
//注意：全局数组里所有数据都是0，后面不必再手动赋值0
//细节：数组长宽比地图行列数都多了2，也就是数组范围从0~row+1和0~col+1，但实际使用范围只用到1~row和1~col，前后多出来的1位防止后面计算NearBy时的数组越位问题(详细说就是计算时每次遇到地雷，都要将地雷周围8个格子对应的数字++，那么如果地雷在边角，地雷周围8个格子就不全在地图内了，这时如果不扩大数组，数组就会越位，强行防止越位需要写一大堆判断语句)

char Map[ROW + 2][COL + 2];//显示给用户的地图

//未实现（写着玩的）
struct user
{
	string name;
	int difficulty;
	int seconds;
};


//函数声明
void Menu();//菜单
void LoadMap();//加载地图
void PrintNearBy();//打印
void PrintMap();//打印
void Input();//输入
void Islandmine(int flag, int x, int y);//输入后数据处理
void Judge(int x, int y);//胜利或失败判断
void open(int x, int y);//打开0后炸开一片的递归
void GameOver();		//游戏结束！ 
void Login();			//登录
void StartGame();		//开始游戏


//加载地图，包括随机埋地雷，并初始化NearBy，初始化map
void LoadMap()
{
	//随机埋地雷
	memset(NearBy, 0, sizeof(NearBy));//重置地图
	srand((unsigned)time(NULL));	//注意该行代码不能放到循环内
	int x, y;						//存储雷的坐标
	for (int i = 0; i < LANDMINE_NUM ;)
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

	//dx[k]和dy[k] 代表周围8个方块x、y方向上的坐标变化
	int dx[8] = { -1,-1,-1,0,0,1,1,1 }, dy[8] = { -1,0,1,-1,1,-1,0,1 };

	for (int i = 1; i <= ROW; i++)
	{
		for (int j = 1; j <= COL; j++) {
			if (NearBy[i][j] == -1)
			{
				for (int k = 0; k < 8; k++)
				{
					if (NearBy[i + dx[k]][j + dy[k]] != -1)//雷旁边的雷不能++
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
}

//打印Map
void PrintMap()
{
	system("cls");
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
			printf("%3c", Map[i][j]);
		}
		cout << endl;
	}
	cout << "---------------------" << endl;
}



//判定是否是地雷，是否不是地雷
void Islandmine(int flag, int x, int y)
{
	//模拟鼠标右键标记
	if (flag == 1)
	{
		if (Map[x][y] == '@')
		{
			Map[x][y] = '*';
			cout << "取消标记成功" << endl;
			PrintMap();
		}
		else
		{
			Map[x][y] = '@';
			cout << "标记成功" << endl;
			PrintMap();
		}
	}
	//模拟鼠标左键打开
	else
	{
		//遇到地雷
		if (NearBy[x][y] == -1)
		{
			Judge(x, y);
		}

		//不是地雷
		else
		{
			Map[x][y] = NearBy[x][y] + 48;//先打开
			open(x, y);//如果是0，就要特殊操作了
			PrintMap();
			PrintNearBy();
			Judge(x, y);//每次输入后都要判断是否胜利
		}
	}
}
//递归点开格子
void open(int x, int y)
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
				
					if (Map[i][j]=='*' && (i != x || j != y))//没有被打开，并且不是自己
					{
						Map[i][j] = NearBy[i][j] + 48;//0旁边8个一定会被打开
						open(i, j);//递归调用旁边8个，旁边8个如果是0就和刚才一样，不是0,open的判断就过不去，不会再递归调用，仅仅打开了自己
					}
				}
			}
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
	if (x >= 1 && x <= ROW && y >= 1 && y <= COL && (Map[x][y] == '*'||Map[x][y] =='@'))
	{
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

//胜利或失败判断，每输入一次后判断一下
//目前的判断胜利逻辑是检测地图上有几个没有被翻开，没有被翻开的数目=雷数才能获胜，会不会遇到bug？
void Judge(int x, int y)
{
	int Count = 0;
	if (NearBy[x][y] == -1)
	{
		GameOver();
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
	cout << Count<<endl;
	if (Count == LANDMINE_NUM)
	{
		cout << "获胜，耗时:";
		cout << "是否再来一把？y/n" << endl;
		char ch;
		cin >> ch;
		if (ch == 'y')
		{
			StartGame();
		}
		else
		{
			exit(0);
		}
	}
}



int main()
{
	Menu();
	return 0;
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

//开始游戏
void StartGame()
{
	LoadMap();
	while (true) {
		Input();
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