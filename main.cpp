#include <iostream>
#include <ctime>
#include <cstdio>
#include <easyx.h>
#include <cstdlib>
#define LANDMINE_NUM 15	//��������
#define LENGTH 40		//ͼƬ���ȣ���ÿ�����ӵĳ���
#define WIDTH 40		//ͼƬ��ȣ���ÿ�����ӵĿ��
using namespace std;
int ROW = 10;
int COL = 10;
int difficulty = 1; //�Ѷ�Ĭ��1��һ��1��2��3�����Ѷ�

IMAGE Img[20];//����ͼƬ
int cnt = 0;
time_t start;//�洢��ʼʱ��
time_t end1;//�洢����ʱ��
int** NearBy = NULL;
char** Map = NULL;


//δʵ�֣�д����ģ�
struct user
{
	string name;
	int difficulty;
	int seconds;
};


//��������
void Menu();		//�˵�
void Login();		//��¼
void StartGame();	//��ʼ��Ϸ

void LoadMap();						//���ص�ͼ
void MouseClick();					//���
void Islandmine(int x, int y);		//��������ݴ���
void Show();						//ÿ�������Ҫ����ͼƬ
void Judge(int x, int y);			//ʤ����ʧ���ж�

void Expand(int x, int y);					//��0��ը��һƬ�ĵݹ�
void Protect(int x, int y);					//��һ�β��ܵ㵽�״���
void Mark(int x, int y);					//�Ҽ����

void LoadImages();//����ͼƬ
void PrintNearBy();//��ӡ��������֣�����ר��

//���ص�ͼ�������������ף�����ʼ��NearBy����ʼ��map
void LoadMap()
{
	//��ʼ��
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
	//��ʼ��
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

	//��������
	srand((unsigned)time(NULL));	//ע����д��벻�ܷŵ�ѭ����
	int x, y;						//�洢�׵�����
	for (int i = 0; i < LANDMINE_NUM;)
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

//��ʼ��Ϸ
void StartGame()
{
	//������Ϸʱ��Ҫ��ȫ�����һЩ��������
	cnt = 0;
	LoadMap();
	time(&start);
	MouseClick();
}


//�ж��Ƿ��ǵ��ף��Ƿ��ǵ���
void Islandmine(int x, int y)
{
	//�����Ѿ��򿪵Ļ����Ѿ�����˵�
	if (Map[x][y] == '@' || Map[x][y] != '*')
	{
		return;
	}
	else if (NearBy[x][y] == -1)
	{
		if (cnt == 0)
		{
			Protect(x, y);
			Map[x][y] = NearBy[x][y] + 48;//�ȴ�
			Expand(x, y);//�����0����Ҫ���������
			PrintNearBy();
			Judge(x, y);//ÿ�������Ҫ�ж��Ƿ�ʤ��
		}
		else
		{
			Map[x][y] = NearBy[x][y] + 48;//�ȴ�
			PrintNearBy();
			Judge(x, y);
		}
		
	}
	//���ǵ���
	else
	{
		Map[x][y] = NearBy[x][y] + 48;//�ȴ�
		Expand(x, y);//�����0����Ҫ���������
		PrintNearBy();
		Judge(x, y);//ÿ�������Ҫ�ж��Ƿ�ʤ��
	}
}

//�Ҽ����
void Mark(int x, int y)
{
	if (Map[x][y] == '@')
	{
		Map[x][y] = '*';
		cout << "ȡ����ǳɹ�" << endl;
		Show();
	}
	else
	{
		Map[x][y] = '@';
		cout << "��ǳɹ�" << endl;
		Show();
	}
}

//�ݹ�㿪����
void Expand(int x, int y)
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
					//û�б��򿪣����Ҳ����Լ�
					if (Map[i][j] == '*' && (i != x || j != y))
					{
						cnt++;
						Map[i][j] = NearBy[i][j] + 48;//0�Ա�8��һ���ᱻ��
						Expand(i, j);//�ݹ�����Ա�8�����Ա�8�������0�ͺ͸ղ�һ��������0,���жϾ͹���ȥ�������ٵݹ���ã����������Լ�
					}
				}
			}
		}
	}
}

//�û����
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



//��һ�β��ܵ㵽��
void Protect(int x, int y)
{
	int a, b;
	srand((unsigned)time(NULL));	//ע����д��벻�ܷŵ�ѭ����
	do
	{
		a = rand() % ROW + 1;	//�����x��Χ1~row
		b = rand() % COL + 1;	//�����y��Χ1~col

	} while (NearBy[a][b] == -1);//�ҵ�һ��û�׵�λ�ð���͵͵ת����

	NearBy[a][b] = -1;
	NearBy[x][y] = 0;
	//�Ȱ�����ԭ��ը����Χ�ĵ�--
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

	//��λ�ü�һ���ף���Χ++
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

//��ӡNearBy�����ǲ��Դ��룡��������
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
	cout << cnt << endl;
}

//*****************************������***********************************
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

//*****************************�������漰��ͼ�λ��Ĵ���***********************************


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
	case 3:exit(1); break;
	default:
		break;
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

//��ʾͼƬ1-8,�հ�,ը�������ӵ�ͼƬ��(����map�е�����/������ʾ��ӦͼƬ)
void Show()
{
	for (int i = 1; i <= ROW; i++)
	{
		for (int j = 1; j <= COL; j++)
		{
			//���û�б������ͷ���ͼƬ10
			if (Map[i][j] == '*')
			{
				putimage(j * 40 - 40, i * 40 - 40, &Img[10]);
			}
			//������ֱ������˾���ʾ��ӦͼƬ
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
			//��������˵��׾���ʾ����ͼƬ
			else if (Map[i][j] != '*' && Map[i][j] != '@' && NearBy[i][j] == -1)
			{
				putimage(j * 40 - 40, i * 40 - 40, &Img[9]);
			}
			//���������˾���ʾ���ͼƬ
			else if (Map[i][j] == '@')
			{
				putimage(j * 40 - 40, i * 40 - 40, &Img[11]);
			}
		}
	}
}

//����ͼƬ
void LoadImages()
{
	//���ö�������
	HWND hnd = GetHWnd();
	SetWindowText(hnd, "������ɨ��");
	char path[50];
	for (int i = 0; i < 12; i++)
	{
		sprintf(path, "./images/%d.jpg", i);
		loadimage(&Img[i], path, LENGTH, WIDTH);
	}
}

//ʤ����ʧ���жϣ�ÿ����һ�κ��ж�һ��
//Ŀǰ���ж�ʤ���߼��Ǽ���ͼ���м���û�б�������û�б���������Ŀ=�������ܻ�ʤ���᲻������bug��
void Judge(int x, int y)
{
	Show();
	int Count = 0;
	if (NearBy[x][y] == -1)
	{
		HWND choice = GetHWnd();
		int ch = MessageBox(choice, "�ı�ը�����Ƿ�����һ�֣�", "ʧ��", MB_OKCANCEL);
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
		sprintf(str, "����ʤ�ˣ���ʱ:%ds���Ƿ�����һ��", i);
		int ch = MessageBox(choice, str, "�ܺ�", MB_OKCANCEL);
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
