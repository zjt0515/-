#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <cstdio>
//#include <graphics.h>//easyx
using namespace std;

double TimeStart, TimeEnd;
const int row = 5;	//��ͼ����
const int col = 5;	//��ͼ����
int difficulty = 1; //�Ѷ�Ĭ��1��һ��1��2��3�����Ѷ�
int Landmine = 5;	//��������
int Clear = 0;		//�����������
int Count = row * col - Landmine;//���״��������Ǹ�������ȥ����
int NearBy[row + 2][col + 2];	//��ά����ÿ��λ�ü�¼��Χ8�������������(-1�ǵ���)
//ע�⣺ȫ���������������ݶ���0�����治�����ֶ���ֵ0
//ϸ�ڣ����鳤��ȵ�ͼ������������2��Ҳ�������鷶Χ��0~row+1��0~col+1����ʵ��ʹ�÷�Χֻ�õ�1~row��1~col��ǰ��������1λ��ֹ�������NearByʱ������Խλ����(��ϸ˵���Ǽ���ʱÿ���������ף���Ҫ��������Χ8�����Ӷ�Ӧ������++����ô��������ڱ߽ǣ�������Χ8�����ӾͲ�ȫ�ڵ�ͼ���ˣ���ʱ������������飬����ͻ�Խλ��ǿ�з�ֹԽλ��Ҫдһ����ж����)

char Map[row + 2][col + 2];//��ʾ���û��ĵ�ͼ

struct user
{
	string name;
	int difficulty;
	int seconds;
};


//��������
void LoadMap();
void PrintNearBy();
void PrintMap();
void Islandmine(int flag, int x, int y);
void Input();
void Menu();
void open(int x, int y);
void GameOver();		//��Ϸ������ 
void Login();
void StartGame();

//���ص�ͼ�������������ף�����ʼ��NearBy����ʼ��map
void LoadMap()
{
	//��������
	srand((unsigned)time(NULL));	//ע����д��벻�ܷŵ�ѭ����
	int x, y;						//�洢�׵�����
	for (int i = 0; i < Landmine;)
	{
		x = rand() % row + 1;	//�������Χ1~row
		y = rand() % col + 1;	//�������Χ1~col
		if (NearBy[x][y] == 0)	//������λ��û�б����ף���ʵ����Ҫ���ǵ��ظ����׵�����
		{
			NearBy[x][y] = -1;
			i++;
		}
	}


	//����ÿ��λ����Χ��������

	//dx[k]��dy[k] ������Χ8������x��y�����ϵ�����仯
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
	//��ʼ����ͼmap
	memset(Map, '*', sizeof(Map));
	PrintMap();

}

//��ӡNearBy
void PrintNearBy()
{
	//system("cls");
	cout << "��ͼ�����ڲ���" << endl;
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

//��ӡMap
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



//�ж��Ƿ��ǵ��ף��Ƿ��ǵ���
void Islandmine(int flag, int x, int y)
{
	//��ǵ���
	if (flag == 1)
	{
		//����Ƿ��Ѿ�����ǹ�
		if (Map[x][y] = '@')
		{
			cout << "���Ѿ���ǹ���" << endl;

		}
		Map[x][y] = '@';
		cout << "��ǳɹ�" << endl;
		PrintMap();
	}
	//ģ��ɨ����Ϸ�������
	else
	{
		//����������
		if (NearBy[x][y] == -1)
		{
			GameOver();
		}
		//�ǿհף�����Ҫ�ݹ�㿪һƬ
		else if (NearBy[x][y] == 0)
		{
			open(x, y);
			PrintMap();
			PrintNearBy();
		}
		//��1-8������
		else
		{
			open(x, y);
			PrintMap();
			PrintNearBy();
		}
	}
}
//�ݹ�㿪����
void open(int x, int y)
{
	//���ȼ��������Ƿ��ڵ�ͼ��
	if (x >= 1 && x <= row && y >= 1 && y <= col)
	{
		Count--;
		//���Ϊ0��Ҫ�ݹ�
		if (NearBy[x][y] == 0)
		{

			//�ݹ�����Χ8���Ƿ��пո���
			for (int i = x - 1; i <= x + 1; i++)
			{
				for (int j = y - 1; j <= y + 1; j++)
				{
					{
						//��Χ8���������0��û�б���
						if (NearBy[i][j] == 0 && Map[i][j] == '*')//����Ҫ��ֹ�ظ���������ѭ��
						{
							Map[x][y] = 48;
							open(i, j);
						}
						//?�������������
						else
						{
							Map[i][j] = NearBy[i][j] + 48;
						}
					}

				}
			}
		}
		//�����Ϊ0�����õݹ�
		else
		{
			Map[x][y] = NearBy[x][y] + 48; Count--;
			return;
		}
	}
}
//�û�����
void Input()
{
	int flag, x, y;
	cout << "������: 1.��ǵ��� 0.��ǲ��ǵ��ף����������� ,��ʽ��0 1 2" << endl;
	scanf_s("%d%d%d", &flag, &x, &y);

	//�ж������Ƿ�������ȷ
	if (x >= 1 && x <= row && y >= 1 && y <= col && Map[x][y] == '*')
	{
		if (Map[x][y])
			cout << "������ȷ" << endl;
		Islandmine(flag, x, y);
	}
	//�����������
	else
	{
		cout << "������󣬿�����Խ����ͼ�˻������Ѿ��㿪�˸õ�" << endl;
		Input();
	}

}




int main()
{
	Menu();
	return 0;
}

//���а����
void rank() {

}

//��Ϸ�˵�
void Menu()
{
	int choice;
	cout << "	����������ѡ��Ķ�Ӧ���" << endl;
	cout << "           1.��¼           " << endl;
	cout << "           2.��ʼ��Ϸ           " << endl;
	cout << "           3.�˳�           " << endl;
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
//��Ϸʧ��
void GameOver()
{
	cout << "�ȵ����ף���Ϸʧ��" << endl;
	PrintNearBy();
	cout << "�Ƿ�����һ�ѣ�y/n" << endl;
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
//��¼
void Login()
{
	cout << "�������û���" << endl;
	string n;
	cin >> n;
	struct user user1;
	user1.name = n;
	Menu();
}
//��ʼ��Ϸ
void StartGame()
{
	LoadMap();
	while (Count != 0)
	{
		Input();
	}
	cout << "��ʤ" << endl;

}
