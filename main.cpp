#include <iostream>
#include <ctime>
#include <cstdio>
#include <easyx.h>
#include <cstdlib>
#define ROW 5				//��ͼ����
#define COL 5				//��ͼ����
#define LANDMINE_NUM 2	//��������
using namespace std;

double TimeStart, TimeEnd;

int difficulty = 1; //�Ѷ�Ĭ��1��һ��1��2��3�����Ѷ�

int NearBy[ROW + 2][COL + 2];	//��ά����ÿ��λ�ü�¼��Χ8�������������(-1�ǵ���)
//ע�⣺ȫ���������������ݶ���0�����治�����ֶ���ֵ0
//ϸ�ڣ����鳤��ȵ�ͼ������������2��Ҳ�������鷶Χ��0~row+1��0~col+1����ʵ��ʹ�÷�Χֻ�õ�1~row��1~col��ǰ��������1λ��ֹ�������NearByʱ������Խλ����(��ϸ˵���Ǽ���ʱÿ���������ף���Ҫ��������Χ8�����Ӷ�Ӧ������++����ô��������ڱ߽ǣ�������Χ8�����ӾͲ�ȫ�ڵ�ͼ���ˣ���ʱ������������飬����ͻ�Խλ��ǿ�з�ֹԽλ��Ҫдһ����ж����)

char Map[ROW + 2][COL + 2];//��ʾ���û��ĵ�ͼ

//δʵ�֣�д����ģ�
struct user
{
	string name;
	int difficulty;
	int seconds;
};


//��������
void Menu();//�˵�
void LoadMap();//���ص�ͼ
void PrintNearBy();//��ӡ
void PrintMap();//��ӡ
void Input();//����
void Islandmine(int flag, int x, int y);//��������ݴ���
void Judge(int x, int y);//ʤ����ʧ���ж�
void open(int x, int y);//��0��ը��һƬ�ĵݹ�
void GameOver();		//��Ϸ������ 
void Login();			//��¼
void StartGame();		//��ʼ��Ϸ


//���ص�ͼ�������������ף�����ʼ��NearBy����ʼ��map
void LoadMap()
{
	//��������
	memset(NearBy, 0, sizeof(NearBy));//���õ�ͼ
	srand((unsigned)time(NULL));	//ע����д��벻�ܷŵ�ѭ����
	int x, y;						//�洢�׵�����
	for (int i = 0; i < LANDMINE_NUM ;)
	{
		x = rand() % ROW + 1;	//�����x��Χ1~row
		y = rand() % COL + 1;	//�����y��Χ1~col
		if (NearBy[x][y] == 0)	//Ҫ���ǵ��ظ����׵�����
		{
			NearBy[x][y] = -1;
			i++;
		}
	}


	//����ÿ��λ����Χ��������

	//dx[k]��dy[k] ������Χ8������x��y�����ϵ�����仯
	int dx[8] = { -1,-1,-1,0,0,1,1,1 }, dy[8] = { -1,0,1,-1,1,-1,0,1 };

	for (int i = 1; i <= ROW; i++)
	{
		for (int j = 1; j <= COL; j++) {
			if (NearBy[i][j] == -1)
			{
				for (int k = 0; k < 8; k++)
				{
					if (NearBy[i + dx[k]][j + dy[k]] != -1)//���Աߵ��ײ���++
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

//��ӡMap
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



//�ж��Ƿ��ǵ��ף��Ƿ��ǵ���
void Islandmine(int flag, int x, int y)
{
	//ģ������Ҽ����
	if (flag == 1)
	{
		if (Map[x][y] == '@')
		{
			Map[x][y] = '*';
			cout << "ȡ����ǳɹ�" << endl;
			PrintMap();
		}
		else
		{
			Map[x][y] = '@';
			cout << "��ǳɹ�" << endl;
			PrintMap();
		}
	}
	//ģ����������
	else
	{
		//��������
		if (NearBy[x][y] == -1)
		{
			Judge(x, y);
		}

		//���ǵ���
		else
		{
			Map[x][y] = NearBy[x][y] + 48;//�ȴ�
			open(x, y);//�����0����Ҫ���������
			PrintMap();
			PrintNearBy();
			Judge(x, y);//ÿ�������Ҫ�ж��Ƿ�ʤ��
		}
	}
}
//�ݹ�㿪����
void open(int x, int y)
{
	//���ȼ��������Ƿ��ڵ�ͼ��
	if (x >= 1 && x <= ROW && y >= 1 && y <= COL)
	{
		//���Ϊ0��Ҫ�ݹ�
		if (NearBy[x][y] == 0)
		{
			//�ݹ�����Χ8��
			for (int i = x - 1; i <= x + 1; i++)
			{
				for (int j = y - 1; j <= y + 1; j++)
				{
				
					if (Map[i][j]=='*' && (i != x || j != y))//û�б��򿪣����Ҳ����Լ�
					{
						Map[i][j] = NearBy[i][j] + 48;//0�Ա�8��һ���ᱻ��
						open(i, j);//�ݹ�����Ա�8�����Ա�8�������0�ͺ͸ղ�һ��������0,open���жϾ͹���ȥ�������ٵݹ���ã����������Լ�
					}
				}
			}
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
	if (x >= 1 && x <= ROW && y >= 1 && y <= COL && (Map[x][y] == '*'||Map[x][y] =='@'))
	{
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

//ʤ����ʧ���жϣ�ÿ����һ�κ��ж�һ��
//Ŀǰ���ж�ʤ���߼��Ǽ���ͼ���м���û�б�������û�б���������Ŀ=�������ܻ�ʤ���᲻������bug��
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
		cout << "��ʤ����ʱ:";
		cout << "�Ƿ�����һ�ѣ�y/n" << endl;
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

//��ʼ��Ϸ
void StartGame()
{
	LoadMap();
	while (true) {
		Input();
	}
	
}

//��¼��δ���,д����ģ�
void Login()
{
	cout << "�������û���" << endl;
	string n;
	cin >> n;
	struct user user1;
	user1.name = n;
	cout << "��¼�ɹ�" << endl;
	Menu();
}

//���а����(δ���)
void rank() {

}