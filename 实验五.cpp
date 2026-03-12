/*数独2.0*/
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int DifficultLevel = 1;
int Page = 0, LastPage = 0;
int  Accord[9][9] = {
	{6, 4, 5, 7, 3, 9, 8, 1, 2},
	{2, 1, 8, 6, 4, 5, 9, 7, 3},
	{7, 3, 9, 2, 8, 1, 6, 4, 5},
	{5, 9, 6, 3, 7, 4, 2, 8, 1},
	{4, 8, 7, 5, 1, 2, 3, 6, 9},
	{3, 2, 1, 8, 9, 6, 7, 5, 4},
	{9, 5, 3, 4, 6, 7, 1, 2, 8},
	{8, 7, 2, 1, 5, 3, 4, 9, 6},
	{1, 6, 4, 9, 2, 8, 5, 3, 7}
};//定义参照表方便建立随机的数独表
long  index;

struct {
	int Value = 0;
	int Display = 1;
	int Answer = 0;
}SudokuTab[9][9];

IMAGE Game, Help, Mainpage, Win;

ExMessage msg;

BYTE filter;

void LoadAllResource();//加载资源
void SetBackKey();//绘制返回键的选中和未选中状态
void ReSetQuestion();//重置题目
void InDigit();//向9*9宫格中填入数字
void DrawDifficultLabel(int x, int y);//绘制游戏界面的难度标签
void Submission();//绘制游戏界面的提交答案的标签

int MainPage();//游戏主页面
int GamePage();//游戏界面
int SetPage();//游戏设置界面
int HelpPage();//游戏帮助界面
int Check(int Which);//检查答案是否正确
int ReRank(int root[9], int numb);
int MyRand(int range);//产生1到某个正数的随机数保证不重复用

int main() {

	LoadAllResource();
	initgraph(700, 800);

	do {
		switch (Page)
		{
		case 0: Page = MainPage(); break;//进入主界面
		case 1: Page = GamePage(); break;//进入“游戏”界面
		case 2: Page = SetPage(); break;//进入“设置”界面
		case 3: Page = HelpPage(); break;//进入“帮助”界面
		}
	} while (1);//主函数用来翻页

	return 0;
}

void LoadAllResource() {
	loadimage(&Game, "./Resource/Game.png", 700, 800);
	loadimage(&Help, "./Resource/Sudoku_help.png", 700, 800);
	loadimage(&Mainpage, "./Resource/MainPage.png", 700, 800);
	loadimage(&Win, "./Resource/Victory.jpg", 700, 800);
}//加载资源

void SetBackKey() {
	if (pow(msg.x - 700, 2) + pow(msg.y - 800, 2) > 150 * 150) {
		setfillcolor(RGB(90, 90, 160));
	}
	else if (pow(msg.x - 700, 2) + pow(msg.y - 800, 2) < 150 * 150) {
		setfillcolor(YELLOW);
	}//更具不同情况改变底色表示鼠标即将选中
	solidcircle(700, 800, 150);
	settextcolor(BLACK);
	settextstyle(50, 30, "楷体");
	outtextxy(570, 710, "返回");
}//绘制“返回键”

int ReRank(int root[9], int numb) {
	for (int i = 0; i < 9; i++) {
		if (root[i] == numb) {
			return i;
		}
	}
	return NULL;
}

int MyRand(int range) {
	srand((unsigned)time(NULL) + index);
	index += 1;
	return rand() % range + 1;
}//产生随机数1

int Check(int Which) {
	int CheckTab[9][9] = { 0 };
	if (Which == 0) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				CheckTab[i][j] = SudokuTab[i][j].Value;
			}
		}
	}
	else if (Which == 1) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (SudokuTab[i][j].Display == 0 || SudokuTab[i][j].Display == -2) {
					CheckTab[i][j] = SudokuTab[i][j].Answer;
				}
				else if (SudokuTab[i][j].Display == 1) {
					CheckTab[i][j] = SudokuTab[i][j].Value;
				}
			}
		}
	}//选择要检查的数组

	for (int i = 0; i < 9; i++) {
		int Tab[10] = { 0 };
		for (int j = 0; j < 9; j++) {
			Tab[CheckTab[i][j]]++;
			Tab[CheckTab[j][i]]++;
		}
		for (int k = 1; k < 10; k++) {
			if (Tab[k] >= 3 || Tab[k] == 1 || Tab[k] == 0) {
				return 0;
			}
		}
		printf("\n");
	}//检查横行和竖行是否符合题意思
	for (int i = 0; i < 9; i += 3) {
		for (int j = 0; j < 9; j += 3) {
			int Tab[10] = { 0 };
			for (int I = i; I < i + 3; I++) {
				for (int J = j; J < j + 3; J++) {
					Tab[CheckTab[I][J]]++;
				}
			}
			for (int i = 1; i < 10; i++) {
				if (Tab[i] == 0 || Tab[j] >= 2) {
					return 0;
				}
			}
		}
	}//检查九宫格中的数是否符合题意
	return 1;
}//检查数独表是否符合数独规则

void ReSetQuestion() {
	int row;
	int root[9] = { 0 };

	for (int i = 0; i < 9; i++)
		root[i] = MyRand(9);//产生范围为1~9的一维数组
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (root[i] == root[j]) {
				root[i] = MyRand(9);
			}
		}
	}//产生1~9的一维数组

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			row = ReRank(root, Accord[i][j]);
			SudokuTab[i][j].Value = root[(row + 1) % 9];
		}
	}//制作数独表
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			SudokuTab[i][j].Display = 1;
			SudokuTab[i][j].Answer = 0;
		}
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 1, k = 0; j <= DifficultLevel;) {
			k = MyRand(9) - 1;
			if (SudokuTab[i][k].Display != 0) {
				SudokuTab[i][k].Display = 0;
				j++;
			}
		}
	}
}

void InDigit() {
	int a = 0, b = 0;
	int Digit = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (SudokuTab[i][j].Display == 0) {
				if (SudokuTab[i][j].Answer == 0) {
					setfillcolor(RGB(150, 250, 250));//淡蓝色
					solidrectangle(55 + j * 65, 55 + i * 65, 45 + (j + 1) * 65, 45 + (i + 1) * 65);
				}
				else if (SudokuTab[i][j].Answer != 0) {
					setfillcolor(RGB(150, 250, 100));//浅绿色
					solidrectangle(55 + j * 65, 55 + i * 65, 45 + (j + 1) * 65, 45 + (i + 1) * 65);
				}
			}
			else if (SudokuTab[i][j].Display == -2) {
				setfillcolor(RED);//红色
				solidrectangle(55 + j * 65, 55 + i * 65, 45 + (j + 1) * 65, 45 + (i + 1) * 65);
			}
		}
	}
	settextstyle(55, 45, "楷体");
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			a = 57 + j * 65;
			b = 62 + i * 65;
			if (SudokuTab[i][j].Display == 1) {
				Digit = SudokuTab[i][j].Value;
			}
			else if (SudokuTab[i][j].Display == 0 || SudokuTab[i][j].Display == -2) {
				Digit = SudokuTab[i][j].Answer;
			}
			switch (Digit)
			{

			case 1: outtextxy(a, b, "1"); break;
			case 2: outtextxy(a, b, "2"); break;
			case 3: outtextxy(a, b, "3"); break;
			case 4: outtextxy(a, b, "4"); break;
			case 5: outtextxy(a, b, "5"); break;
			case 6: outtextxy(a, b, "6"); break;
			case 7: outtextxy(a, b, "7"); break;
			case 8: outtextxy(a, b, "8"); break;
			case 9: outtextxy(a, b, "9"); break;
			}
		}
	}
}//将题目打印在9*9的宫格中

void DrawDifficultLabel(int x, int y) {

	int Which = 0;
	if (x > 300 && x < 400 && y > 0 && y < 40) {
		Which = 1;
	}
	else if (1) {
		Which = 0;
	}//判断鼠标是否选中“难度标签”
	setfillcolor(RGB(90, 90, 160));
	if (Which == 1) {
		setfillcolor(YELLOW);
	}
	solidrectangle(300, 0, 400, 40);
	settextcolor(BLACK);
	settextstyle(40, 25, "楷体");
	switch (DifficultLevel)
	{
	case 1: outtextxy(300, 0, "简单"); break;
	case 2: outtextxy(300, 0, "一般"); break;
	case 3: outtextxy(300, 0, "困难"); break;
	case 4: outtextxy(300, 0, "恐怖"); break;
	default:
		break;
	}
}//绘制设置界面

void Submission() {
	int which = 0;
	if (msg.x > 50 && msg.x < 260 && msg.y < 700 && msg.y > 650) {
		which = 1;
	}
	setfillcolor(RGB(90, 90, 160));
	if (which == 1) {
		setfillcolor(YELLOW);
	}
	settextstyle(50, 30, "楷体");
	solidrectangle(50, 650, 300, 710);
	outtextxy(56, 656, "提交答案");
}//绘制提交答案标签

int MainPage() {
	cleardevice();
	putimage(0, 0, &Mainpage);
	setfillcolor(RGB(90, 90, 160));
	for (int i = 0; i < 3; i++)
		solidrectangle(200, 300 + 200 * i, 500, 200 + 200 * i);
	settextcolor(BLUE);
	setbkmode(TRANSPARENT);
	settextstyle(140, 75, "楷体");
	outtextxy(50, 25, "数独游戏");
	settextcolor(BLACK);
	settextstyle(70, 50, "宋体");
	outtextxy(250, 215, "开始");
	outtextxy(250, 415, "设置");
	outtextxy(250, 615, "帮助");//绘制“主界面”
	LastPage = 0;
	do {
		getmessage(&msg, filter = -1);
		setfillcolor(YELLOW);
		if (msg.x > 200 && msg.x < 500 && msg.y > 200 && msg.y < 300) {
			solidcircle(225, 250, 17), solidcircle(475, 250, 17);
			if (msg.message == WM_LBUTTONDOWN) {
				return 1;
			}
		}//选择“开始”返回游戏界面标签“1”
		else if (msg.x > 200 && msg.x < 500 && msg.y > 400 && msg.y < 500) {
			solidcircle(225, 450, 17), solidcircle(475, 450, 17);
			if (msg.message == WM_LBUTTONDOWN) {
				return 2;
			}
		}//选择“设置”返回游戏设置界面标签“2”
		else if (msg.x > 200 && msg.x < 500 && msg.y > 600 && msg.y < 700) {
			solidcircle(225, 650, 17), solidcircle(475, 650, 17);
			if (msg.message == WM_LBUTTONDOWN) {
				return 3;
			}
		}//选择“帮助”返回游戏帮助界面标签“3”
		else if (1) {
			setfillcolor(RGB(90, 90, 160));
			for (int i = 0; i < 3; i++)
				solidcircle(225, 250 + i * 200, 17), solidcircle(475, 250 + i * 200, 17);
		}//鼠标未选择涂去标记
	} while (1);

	return 0;
}

int GamePage() {
	cleardevice();
	putimage(0, 0, &Game);
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 1);
	for (int i = 0; i < 8; i++) {
		line(115 + 65 * i, 50, 115 + 65 * i, 635);
		line(50, 115 + 65 * i, 635, 115 + 65 * i);
	}
	setlinestyle(PS_SOLID, 2);
	for (int i = 0; i < 4; i++) {
		line(50 + 195 * i, 50, 50 + 195 * i, 635);
		line(50, 50 + 195 * i, 635, 50 + 195 * i);
	}//绘制界面

	do {
		ReSetQuestion();
	} while (!Check(0));//重置题目直到题目符合数独游戏规则

	do {
		int a = 0, b = 0;
		getmessage(&msg, filter = -1);
		Submission();
		SetBackKey();
		InDigit();
		DrawDifficultLabel(msg.x, msg.y);
		a = (msg.y - 50) / 65, b = (msg.x - 50) / 65;
		if (msg.message == WM_LBUTTONDOWN) {
			if (msg.x > 300 && msg.x < 400 && msg.y > 0 && msg.y < 40) {
				LastPage = 1;
				return 2;
			}//点击“难度标签”
			else if (pow(msg.x - 700, 2) + pow(msg.y - 800, 2) < 150 * 150) {
				LastPage = 1;
				return 0;
			}//点击“返回标签”
			if (msg.x > 55 && msg.x < 635 && msg.y > 55 && msg.y < 635) {
				if (SudokuTab[a][b].Display == 0 || SudokuTab[a][b].Display == -2) {
					do {
						if (_kbhit()) {
							char key = _getch();
							if (key <= '9' && key >= '1') {
								SudokuTab[a][b].Answer = key - 48;
							}
							SudokuTab[a][b].Display = 0;
							break;
						}
					} while (1);
				}//点击9*9宫格“做题”
			}
			if (msg.x > 50 && msg.x < 300 && msg.y > 650 && msg.y < 710) {
				int Result = Check(1);
				if (Result) {
					putimage(0, 0, &Win);
					do {
						getmessage(&msg, filter = -1);
						if (msg.message == WM_LBUTTONDOWN) {
							return 1;
						}
					} while (1);
				}//给出游戏胜利界面
				else if (Result == 0) {
					for (int i = 0; i < 9; i++) {
						for (int j = 0; j < 9; j++) {
							if (SudokuTab[i][j].Display == 0) {
								if (SudokuTab[i][j].Value != SudokuTab[i][j].Answer) {
									SudokuTab[i][j].Display = -2;
								}
							}
						}
					}
					InDigit();
					do {
						getmessage(&msg, filter = -1);
						if (msg.message == WM_LBUTTONDOWN) {
							for (int i = 0; i < 9; i++) {
								for (int j = 0; j < 9; j++) {
									if (SudokuTab[i][j].Display == -2) {
										SudokuTab[i][j].Display = 0;
									}
								}
							}
							break;
						}
					} while (1);
				}
			}//选择“提交答案”
		}//判断按钮
	} while (1);
	return 0;
}//绘制游戏界面

int SetPage() {
	cleardevice();
	putimage(0, 0, &Mainpage);
	setfillcolor(RGB(90, 90, 160));
	for (int i = 0; i < 4; i++)
		solidrectangle(200, 265 + 160 * i, 500, 165 + 160 * i);
	settextcolor(BLACK);
	settextstyle(100, 50, "楷体");
	outtextxy(50, 0, "游戏难度设置");
	settextcolor(BLUE);
	setbkmode(TRANSPARENT);
	settextstyle(70, 50, "楷体");
	outtextxy(250, 180, "简单");
	outtextxy(250, 340, "一般");
	outtextxy(250, 500, "困难");
	outtextxy(250, 660, "恐怖");
	setfillcolor(YELLOW);
	switch (DifficultLevel)
	{
	case 1: solidcircle(225, 215, 17); break;
	case 2: solidcircle(225, 375, 17); break;
	case 3: solidcircle(225, 535, 17); break;
	case 4: solidcircle(225, 695, 17); break;
	}//绘制“设置界面”

	do {
		getmessage(&msg, filter = 1);
		setfillcolor(YELLOW);
		switch (DifficultLevel)
		{
		case 1: solidcircle(225, 215, 17); break;
		case 2: solidcircle(225, 375, 17); break;
		case 3: solidcircle(225, 535, 17); break;
		case 4: solidcircle(225, 695, 17); break;
		}//标记选中难度等级
		SetBackKey();
		if (msg.message == WM_LBUTTONDOWN) {
			if (msg.x > 200 && msg.x < 500 && msg.y > 165 && msg.y < 260) {
				DifficultLevel = 1;
			}
			else if (msg.x > 200 && msg.x < 500 && msg.y > 325 && msg.y < 420) {
				DifficultLevel = 2;
			}
			else if (msg.x > 200 && msg.x < 500 && msg.y > 485 && msg.y < 580) {
				DifficultLevel = 3;
			}
			else if (msg.x > 200 && msg.x < 500 && msg.y > 645 && msg.y < 740) {
				DifficultLevel = 4;
			}
			else if (pow(msg.x - 700, 2) + pow(msg.y - 800, 2) < 150 * 150) {
				return LastPage;//返回上一页可能是主界面，也可能是游戏界面
			}//选择难度和判断是否返回

			setfillcolor(RGB(90, 90, 160));
			for (int i = 0; i < 4; i++)
				solidcircle(225, 215 + 160 * i, 17);//判断进行难度选择后清空标记
		}

	} while (1);//设置界面按钮选择
	return 0;
}//绘制游戏设置界面

int HelpPage() {
	cleardevice();
	putimage(0, 0, &Help);

	do {
		getmessage(&msg, filter = -1);
		if (pow(msg.x - 700, 2) + pow(msg.y - 800, 2) < 150 * 150) {
			SetBackKey();
			if (msg.message == WM_LBUTTONDOWN) {
				return 0;
			}
		}
		else if (1) {
			SetBackKey();
		}
	} while (1);//帮助界面按钮选择

	return 0;
}//绘制游戏帮助界面