#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <graphics.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define SIZE 40		//ͼƬ��ȣ���ÿ�����ӵĿ�ȣ�
#define N 5		//��ʾ�ļ�¼����
#define M N*3   //3���Ѷȣ�һ��15����¼
using namespace std;
int row = 9;		//��������
int col = 9;		//��������
int number = 10;	//��������

IMAGE Img[20];//�洢ͼƬ
int cnt = 0;//���Ѿ��򿪵ĸ���
time_t TimeStart;//�洢��ʼʱ��
time_t TimeEnd;//�洢����ʱ��
int** NearBy = NULL;
char** Map = NULL;

//��������
void Menu();		//�˵�
void StartGame();	//��ʼ��Ϸ

void InitGame();					//��Ϸ��ʼ��
void MouseClick();					//���
void Islandmine(int x, int y);		//��������ݴ���
void Show();						//ÿ�������Ҫ����ͼƬ
void Judge(int x, int y);			//ʤ����ʧ���ж�

void Expand(int x, int y);					//��0��ը��һƬ�ĵݹ�
void Protect(int x, int y);					//��һ�β��ܵ㵽�״���
void Mark(int x, int y);					//�Ҽ����

void LoadImages();//����ͼƬ
void PrintNearBy();//��ӡ��������֣�����ר��
void Exit();//��ȫ�˳�����

void Setting();
void ShowRank();
void TextButton();
//�ļ�����
void ReadFile();
void SortRank();
void WriteFile();

typedef struct User
{
	int Level;
	char name[20];
	int time;
}User;

User Top[M+1];
User CurrentUser = {1,"����"};//�浱ǰ�����û���Ϣ��Ĭ�����Ѷ�1������

//���ص�ͼ�������������ף�����ʼ��NearBy����ʼ��map
void InitGame()
{
	initgraph(col * SIZE, row * SIZE, 1);

	cnt = 0;//���ô򿪵ĸ��Ӹ�����¼��
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
	mciSendString("open ./images/start.mp3", 0, 0, 0);
	mciSendString("play ./images/start.mp3", 0, 0, 0);
	time(&TimeStart);
	MouseClick();
}


//�ж��Ƿ��ǵ��ף��Ƿ��ǵ���
void Islandmine(int x, int y)
{
	//�����Ѿ��򿪵Ļ����Ѿ�����˵�ֱ��return
	if (Map[x][y] == '@' || Map[x][y] != '*')return;
	//�����˵���
	else if (NearBy[x][y] == -1)
	{
		//��һ�ξ�������Ҫ����
		if (cnt == 0)
		{
			Protect(x, y);
		}
			Expand(x, y);
			PrintNearBy();
	}
	//���ǵ���
	else
	{
		Expand(x, y);
		PrintNearBy();
	}
	Judge(x, y);//����ʽ����Ƿ���Ӯ
}

//�Ҽ����
void Mark(int x, int y)
{
	if (Map[x][y] == '@')
	{
		Map[x][y] = '&';
		cout << "�ʺųɹ�" << endl;
		Show();
	}
	else if (Map[x][y] == '&')
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
	Map[x][y] = NearBy[x][y] + 48;
	if (NearBy[x][y] == -1)return;//����������Ϊ���øú������Ը� ����\0\1-8�������  ͬʱʹ�õ�
	cnt++;//��¼���Ѿ������ĸ���
	if (NearBy[x][y] > 0)return;
	if (!(x >= 1 && x <= row && y >= 1 && y <= col))return;
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			
			if (Map[i][j] == '*')
			{
				Expand(i, j);
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
	initgraph(640, 640, 1);
	Menu();
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
			else if (Map[i][j] != '*' && Map[i][j] != '@'&&Map[i][j] != '&' && NearBy[i][j] == -1)
			{
				putimage(j * SIZE - SIZE, i * SIZE - SIZE, &Img[9]);
			}
			//���������˾���ʾ���ͼƬ
			else if (Map[i][j] == '@')
			{
				putimage(j * SIZE - SIZE, i * SIZE - SIZE, &Img[11]);
			}
			//���ʺ��˾���ʾ�ʺ�
			else if (Map[i][j] == '&')
			{
				putimage(j * SIZE - SIZE, i * SIZE - SIZE, &Img[12]);
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
	loadimage(&Img[19], "./images/bg.png", 640, 640);
	for (int i = 0; i < 12; i++)
	{
		sprintf(path, "./images/%d.jpg", i);
		loadimage(&Img[i], path, SIZE, SIZE);
	}
}

//ʤ����ʧ���жϣ�ÿ����һ�κ��ж�һ��
//Ŀǰ���ж�ʤ���߼��Ǽ���ͼ���м���û�б�������û�б���������Ŀ=�������ܻ�ʤ���᲻������bug��
//���£���д��expand������������׼ȷ���㱻�����ĸ����ж��ٸ�
void Judge(int x, int y)
{
	Show();
	if (NearBy[x][y] == -1)
	{
		for (int i = 1; i <= row; i++)
		{
			for (int j = 1; j <= col; j++)
			{
				if (NearBy[i][j] == -1)
				{
					Map[i][j] == -1 + 48;
				}
			}
		}
		Show();
		HWND choice = GetHWnd();
		int ch = MessageBox(choice, "��ʧ���ˣ��Ƿ�����һ�֣�", "ʧ��", MB_OKCANCEL);
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

	if (cnt == row * col - number)
	{
		HWND choice = GetHWnd();
		time(&TimeEnd);
		int time = TimeEnd - TimeStart;
		CurrentUser.time = time;
		char str[50];
		sprintf(str, "����ʤ�ˣ���ʱ:%ds���Ƿ�����һ��", time);
		ReadFile();
		SortRank();
		WriteFile();
		int ch = MessageBox(choice, str, "�ܺ�", MB_OKCANCEL);
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
	setbkcolor(WHITE);
	cleardevice();
	putimage(0, 0, &Img[19]);
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
					int isok = InputBox(CurrentUser.name, 10, "�����������û��������ʹ��10���ַ����޷�ʹ�ÿհ���", "��¼", NULL, 0, 0, false);
					if (isok == 1 )
					{
						HWND hwnd = GetHWnd();
						MessageBox(hwnd, "��¼�ɹ�", "��ʾ", MB_OK);
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
	outtextxy(40, 250, "��ʼ��Ϸ");
	outtextxy(40, 325, "��¼");
	outtextxy(40, 400, "����");
	outtextxy(40, 475, "����");
	outtextxy(40, 550, "�˳�");
}
void Setting()
{
	cleardevice();
	settextcolor(BLACK);
	settextstyle(40, 0, "VonwaonBitmap 16px");
	outtextxy(40, 50, "�Ѷ�:");
	outtextxy(180, 50, "��");
	outtextxy(300, 50, "��ͨ");
	outtextxy(420, 50, "����");
	//outtextxy(40, 125, "Ƥ��:");
	outtextxy(500, 540, "����");
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
					row = 9;
					col = 9;
					number = 10;
					MessageBox(NULL, "��ģʽ���óɹ�", "��ʾ", MB_OK);
				}
				if (msg.x >= 300 && msg.x <= 380 && msg.y >= 50 && msg.y <= 90)
				{
					CurrentUser.Level = 2;
					row = 16;
					col = 16;
					number = 40;
					MessageBox(NULL, "��ͨģʽ���óɹ�", "��ʾ", MB_OK);
				}
				if (msg.x >= 420 && msg.x <= 500 && msg.y >= 50 && msg.y <= 90)
				{
					CurrentUser.Level = 3;
					row = 16;
					col = 30;
					number = 99;
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
void ShowRank()
{
	cleardevice();
	outtextxy(500, 540, "����");
	outtextxy(20, 20, "��:");
	outtextxy(20, 220, "��ͨ:");
	outtextxy(20, 440, "����:");
	settextstyle(20, 0, "VonwaonBitmap 12px");
	ReadFile();
	for (int i = 1; i <= M; i++)
	{
		char time[20];
		sprintf(time, "%d", Top[i].time);
		outtextxy(300, 40 + 30 * i, Top[i].name);
		outtextxy(380, 40 + 30 * i, time);
	}
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg, EX_MOUSE))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				if (msg.x >= 500 && msg.x <= 580 && msg.y >= 540 && msg.y <= 580)
				{
					Menu();
				}
			}
		}
	}
}

//*************************�ļ�����*************************

//�����ļ��е����ݣ��Խ����������ݱȽ�
void ReadFile()
{
	int i = 1;                       /*�˴�iӦ��ʼ��Ϊ0*/
	FILE* fp;                        /*�����ļ�ָ��*/
	fp = fopen("./rank.txt", "r");     /*���ı��ļ�׼����������*/
	if (fp == 0)                     /*����ļ���ʧ�� */
	{
		printf("source file error\n");
		exit(1);
	}
	while (!feof(fp))               /*���ļ�δ����ʱ����ѭ��*/
	{
		/*�ṹ��������±��1Ȼ���������*/
		fscanf(fp, "%d%s%d", &Top[i].Level, Top[i].name, &Top[i].time);
		i++;
	}
	//���Դ���
	for (int i = 1; i <= M; i++)
	{
		printf("%d%s%d\n", Top[i].Level, Top[i].name, Top[i].time);
	}
	fclose(fp);                     /*�ر��ļ�*/
}
//���ݱȽ�
void SortRank()
{	
	int last;
	int i = 0;
	int cnt = 0;
	switch (CurrentUser.Level)
	{
	case 1: i = last = 5; break;
	case 2: i = last = 10; break;
	case 3: i = last = 15; break;
	}
	if (CurrentUser.time>=Top[i].time)
	{
		return;//���һ�����Ȳ���ֱ��return��
	}
	else
	{
		//�������ڶ�����ʼ��
		do
		{
			//����5�Σ���ֹ�����������ı�
			if (cnt == 5)
			{
				break;
			}
			i--;
			cnt++;
		} while (CurrentUser.time < Top[i].time );//��while��˵����ǰ�û��Ȳ���top[i]�����ܱȹ�top[i+1]
		//cnt������cnt�Σ���ǰ�û����ǵ�����cnt�������磬ֻ����1�Σ���˵��û�ȹ������ڶ��������ǵ�����һ��������2�Σ���˵��û�ȹ����������������ǵ����ڶ���
	}
	cout << cnt << endl << i<< endl;
	int king = ++i;//����++i���ǵ�ǰ�û�Ӧ��ȡ���ĵط���������ȡ��������Ӧ���Ȱ�i�Լ�����ķ���������λ
	for (int k = 0; k < cnt - 1; last--,k++)//����cnt-1����λ(���磬����cnt�������ѵó�Ҫ����cnt-1����λ)
	{
		strcpy(Top[last].name, Top[last-1].name);
		Top[last].time = Top[last-1].time;
	}
	//�����ǻ�
	strcpy(Top[king].name, CurrentUser.name);
	Top[king].time = CurrentUser.time;
}
//��������д��
void WriteFile()
{
	int i;
	FILE* fp;
	fp = fopen("./rank.txt", "w");   /*��w��ʽ���ı��ļ�׼��д������*/
	if (fp == 0)	      					 /* ����ļ���ʧ�� */
	{
		printf("create new file error\n");	//�����˳�����
		exit(1);
	}
	for (i = 1; i <= M; i++)
	{ 
		fprintf(fp, "%d %s %d\n", Top[i].Level, Top[i].name, Top[i].time);//д���ļ�
	}
	fclose(fp);              /*�ر��ļ�*/
}
