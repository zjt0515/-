#include <iostream>
#include <ctime>
#include <cstdio>
#include <easyx.h>
#include <cstdlib>
#define ROW 10			//��ͼ����
#define COL 10			//��ͼ����
#define LANDMINE_NUM 10	//��������
#define LENGTH 40		//ͼƬ���ȣ���ÿ�����ӵĳ���
#define WIDTH 40		//ͼƬ��ȣ���ÿ�����ӵĿ��
using namespace std;

double TimeStart, TimeEnd;
int difficulty = 1; //�Ѷ�Ĭ��1��һ��1��2��3�����Ѷ�

int NearBy[ROW + 2][COL + 2];	//��ά����ÿ��λ�ü�¼��Χ8�������������(-1�ǵ���)
//ע�⣺ȫ���������������ݶ���0�����治�����ֶ���ֵ0
//ϸ�ڣ����鳤��ȵ�ͼ������������2��Ҳ�������鷶Χ��0~row+1��0~col+1����ʵ��ʹ�÷�Χֻ�õ�1~row��1~col��ǰ��������1λ��ֹ�������NearByʱ������Խλ����(��ϸ˵���Ǽ���ʱÿ���������ף���Ҫ��������Χ8�����Ӷ�Ӧ������++����ô��������ڱ߽ǣ�������Χ8�����ӾͲ�ȫ�ڵ�ͼ���ˣ���ʱ������������飬����ͻ�Խλ��ǿ�з�ֹԽλ��Ҫдһ����ж����)

char Map[ROW + 2][COL + 2];//��ʾ���û��ĵ�ͼ
IMAGE Img[20];//����ͼƬ
int cnt = 0;


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
void MouseClick();//���
void Islandmine(int x, int y);//��������ݴ���
void Judge(int x, int y);//ʤ����ʧ���ж�
void open(int x, int y);//��0��ը��һƬ�ĵݹ�
void Login();			//��¼
void StartGame();		//��ʼ��Ϸ
void Show();
void LoadImages();
void Mark(int x,int y);
void PrintNearBy();//��ӡ��������֣�����ר��


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
	Show();
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


//�ж��Ƿ��ǵ��ף��Ƿ��ǵ���
void Islandmine(int x, int y)
{
	//�����Ѿ��򿪵Ļ����Ѿ�����˵�
	if (Map[x][y]=='@'||Map[x][y]!='*')
	{
		return;
	}
	//�ǵ��׵��ǵ�һ�δ�
	else if (NearBy[x][y] == -1 && cnt ==0)
	{
		
	}
	else if (NearBy[x][y] == -1)
	{
		Map[x][y] = NearBy[x][y] + 48;//�ȴ�
		Show();
		PrintNearBy();
		Judge(x, y);
	}
	//���ǵ���
	else
	{
		Map[x][y] = NearBy[x][y] + 48;//�ȴ�
		open(x, y);//�����0����Ҫ���������
		Show();
		PrintNearBy();
		Judge(x, y);//ÿ�������Ҫ�ж��Ƿ�ʤ��
	}
}

void Mark(int x,int y)
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
						cnt++;
						Map[i][j] = NearBy[i][j] + 48;//0�Ա�8��һ���ᱻ��
						open(i, j);//�ݹ�����Ա�8�����Ա�8�������0�ͺ͸ղ�һ��������0,open���жϾ͹���ȥ�������ٵݹ���ã����������Լ�
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
	if (peekmessage(&msg))
	{
		int x = msg.y / WIDTH + 1;
		int y = msg.x / LENGTH + 1;
		switch (msg.message)
		{
		case WM_LBUTTONDOWN: Islandmine(x, y); break;
		case WM_RBUTTONDOWN: Mark(x,y); break;
		}
	}

}

//ʤ����ʧ���жϣ�ÿ����һ�κ��ж�һ��
//Ŀǰ���ж�ʤ���߼��Ǽ���ͼ���м���û�б�������û�б���������Ŀ=�������ܻ�ʤ���᲻������bug��
void Judge(int x, int y)
{
	int Count = 0;
	if (NearBy[x][y] == -1)
	{
		HWND choice = GetHWnd();
		int ch = MessageBox(choice, "�ı�ը����", "ʧ��", MB_OKCANCEL);
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
	cout << Count<<endl;
	if (Count == LANDMINE_NUM)
	{
		HWND choice = GetHWnd();
		int ch = MessageBox(choice, "����ʤ�ˣ���ʱ���Ƿ�����һ��", "�ܺ�", MB_OKCANCEL);
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



int main()
{
	initgraph(LENGTH * COL, WIDTH * ROW,EX_SHOWCONSOLE);
	LoadImages();
	Menu();
	closegraph();
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

//��ʼ��Ϸ
void StartGame()
{
	LoadMap();
	while (true) {
		MouseClick();
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

//����map�е�����/������ʾ��ӦͼƬ
void Show()
{
	for (int i = 1; i <= ROW; i++)
	{
		for (int j = 1; j <= COL; j++)
		{
			//���û�б������ͷ���ͼƬ10
			if (Map[i][j] == '*')
			{
				putimage(j * 40-40, i * 40-40, &Img[10]);
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
			else if (Map[i][j]=='@')
			{
				putimage(j * 40 - 40, i * 40 - 40, &Img[11]);
			}
		}
	}
}

//����ͼƬ
void LoadImages()
{
	HWND hnd = GetHWnd();
	SetWindowText(hnd, "������ɨ��");
	char path[50];
	for (int i = 0; i < 12; i++)
	{
		sprintf(path, "./images/%d.jpg", i);
		loadimage(&Img[i], path,LENGTH,WIDTH);
	}
}