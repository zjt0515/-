#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <graphics.h>

#define SIZE 40		//ͼƬ��ȣ���ÿ�����ӵĿ�ȣ�
using namespace std;
int row = 10;		//��������
int col = 10;		//��������
int number = 10;	//��������

IMAGE Img[20];//�洢ͼƬ
int cnt = 0;
time_t TimeStart;//�洢��ʼʱ��
time_t TimeEnd;//�洢����ʱ��
int** NearBy = NULL;
char** Map = NULL;

//��������
void Menu();		//�˵�
void Login();		//��¼
void StartGame();	//��ʼ��Ϸ

void InitGame();						//���ص�ͼ
void MouseClick();					//���
void Islandmine(int x, int y);		//��������ݴ���
void Show();						//ÿ�������Ҫ����ͼƬ
void Judge(int x, int y);			//ʤ����ʧ���ж�

void Expand(int x, int y);					//��0��ը��һƬ�ĵݹ�
void Protect(int x, int y);					//��һ�β��ܵ㵽�״���
void Mark(int x, int y);					//�Ҽ����

void LoadImages();//����ͼƬ
void PrintNearBy();//��ӡ��������֣�����ר��
void nanduxuanz();
void kaishiyouxi();
void Exit();//��ȫ�˳�����

void Setting();
void TextButton();

//���ص�ͼ�������������ף�����ʼ��NearBy����ʼ��map
void InitGame()
{
	initgraph(col * SIZE, row * SIZE,1);
	//��ʼ��()
	cnt = 0;
	//��ʼ��
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
	//��ʼ��
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

	//��������
	srand((unsigned)time(NULL));	//ע����д��벻�ܷŵ�ѭ����
	int x, y;						//�洢�׵�����
	for (int i = 0; i < number;)
	{
		x = rand() % row + 1;	//�����x��Χ1~row
		y = rand() % col + 1;	//�����y��Χ1~col
		if (NearBy[x][y] == 0)	//Ҫ���ǵ��ظ����׵�����
		{
			NearBy[x][y] = -1;
			i++;
		}
	}

	//����ÿ��λ����Χ��������
	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++) {
			if (NearBy[i][j] == -1)
			{
				for (int p = i - 1; p <= i + 1; p++)
				{
					for (int q = j - 1; q <= j + 1; q++)
					{
						if (NearBy[p][q] != -1)//���ǵ��׾�++��(�����ų��Լ�����Ϊ�Լ����ǵ��ף�����ͨ���������)
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
	InitGame();
	time(&TimeStart);
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
	if (x >= 1 && x <= row && y >= 1 && y <= col)
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



//��һ�β��ܵ㵽��
void Protect(int x, int y)
{
	int a, b;
	srand((unsigned)time(NULL));	//ע����д��벻�ܷŵ�ѭ����
	do
	{
		a = rand() % row + 1;	//�����x��Χ1~row
		b = rand() % col + 1;	//�����y��Χ1~col

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

//*****************************������***********************************
int main()
{
	
	LoadImages();
	
	Menu();
	//StartGame();
	getchar();
	return 0;
}

//*****************************�������漰��ͼ�λ��Ĵ���***********************************



//��ʾͼƬ1-8,�հ�,ը�������ӵ�ͼƬ��(����map�е�����/������ʾ��ӦͼƬ)
void Show()
{
	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++)
		{
			//���û�б������ͷ���ͼƬ10
			if (Map[i][j] == '*')
			{
				putimage(j * SIZE - SIZE, i * SIZE - SIZE, &Img[10]);
			}
			//������ֱ������˾���ʾ��ӦͼƬ
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
			//��������˵��׾���ʾ����ͼƬ
			else if (Map[i][j] != '*' && Map[i][j] != '@' && NearBy[i][j] == -1)
			{
				putimage(j * SIZE - SIZE, i * SIZE - SIZE, &Img[9]);
			}
			//���������˾���ʾ���ͼƬ
			else if (Map[i][j] == '@')
			{
				putimage(j * SIZE - SIZE, i * SIZE - SIZE, &Img[11]);
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
		loadimage(&Img[i], path, SIZE, SIZE);
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
		int i = TimeEnd - TimeStart;
		char str[50];
		sprintf(str, "����ʤ�ˣ���ʱ:%ds���Ƿ�����һ��", i);
		int ch = MessageBox(choice, str, "�ܺ�", MB_OKCANCEL);
		if (ch == IDOK)
		{
			StartGame();
		}
		else
		{
			Menu();
		}
	}
}

void Exit()
{
	//�ͷ�new������
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
	//�رմ��ںͳ���
	closegraph();
	exit(0);
}

void Menu()
{
	initgraph(640, 640);
	setbkcolor(WHITE);
	cleardevice();
	TextButton();
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg,EM_MOUSE))
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
					char name[20];
					int isok = InputBox(name, 20, "�����������û���������20�ַ�", "��¼", NULL, 0, 0, false);
					if (isok == 1)
					{
						HWND hwnd = GetHWnd();
						MessageBox(hwnd, "��¼�ɹ�", "��ʾ", MB_OK);
						outtextxy(100, 20, name);
					}
					
				}
				if (msg.x >= 40 && msg.x <= 120 && msg.y >= 400 && msg.y <= 440)
				{
					Setting();
				}
				if (msg.x >= 40 && msg.x <= 120 && msg.y >= 475 && msg.y <= 515)
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
	outtextxy(40, 250, "��ʼ��Ϸ");
	outtextxy(40, 325, "��¼");
	outtextxy(40, 400, "����");
	outtextxy(40, 475, "�˳�");
}
void Setting()
{
	cleardevice();
	outtextxy(40, 50, "�Ѷ�:");
	outtextxy(180, 50, "��");
	outtextxy(300, 50, "��ͨ");
	outtextxy(420, 50, "����");
	outtextxy(40, 125, "Ƥ��:");
	outtextxy(500, 540, "����");
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg,EM_MOUSE))
		{
			if (msg.message==WM_LBUTTONDOWN)
			{
				if (msg.x >= 180 && msg.x <= 260 && msg.y >= 50 && msg.y <= 90)
				{
					row = 8;
					col = 8;
					number = 5;
					MessageBox(NULL, "��ģʽ���óɹ�", "��ʾ", MB_OK);
				}
				if (msg.x >= 300 && msg.x <= 380 && msg.y >= 50 && msg.y <= 90)
				{
					row = 12;
					col = 12;
					number = 15;
					MessageBox(NULL, "��ͨģʽ���óɹ�", "��ʾ", MB_OK);
				}
				if (msg.x >= 420 && msg.x <= 500 && msg.y >= 50 && msg.y <= 90)
				{
					row = 20;
					col = 20;
					number = 60;
					MessageBox(NULL, "����ģʽ���óɹ�", "��ʾ", MB_OK);
				}
				if (msg.x >= 500 && msg.x <= 580 && msg.y >= 540 && msg.y <= 580)
				{
					Menu();
				}
			}
		}
	}
}