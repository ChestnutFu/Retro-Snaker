#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<time.h>
#pragma warning(disable:4996)
/*----画面刷新速度---*/
#define velocity 500
/*----地图定义----*/
const int H = 15;//图高
const int L = 30;//图宽
char GameMap[H][L];
/*----游戏按键及状态-----*/
int over = 0;//游戏是否结束
int change_x[4] = { 0, 0, -1, 1 };
int change_y[4] = { -1, 1, 0, 0 };
/*----蛇定义----*/
const char Shead = '@';//蛇头
const char Sbody = '+';//蛇身
const char Sfood = '#';//食物
const char Snode = '.';//地图
int Slen = 1;//蛇长度
struct Snack
{
	int x, y;//位置
	int now;//前进方向，0-左，1-右，2-上，3-下
}snack[H*L];//蛇

/*----函数声明-----*/
void Initial();//初始化函数
void Show();//刷新地图
void creatFood();//产生食物
void getButton();//获取按键判断方向
void move();//移动函数
bool IsBorder();
void checkHeadMove(int x, int y);


/*--------主函数---------*/
int main()
{
	int willcontinue;
	do{
		Initial();
		Show();
		printf("\n按c继续游戏，按除c外的任意键退出\n");

		willcontinue = getch();
	} while (willcontinue == 99);
	return 0;
}

void Initial()
{
	system("cls");
	Slen = 1;
	over = 0;
	//初始化
	int Sheadx, Sheady;

	memset(GameMap, '.', sizeof(GameMap));//初始化

	Sheadx = rand() % H;
	Sheady = rand() % L;
	GameMap[Sheadx][Sheady] = Shead;
	snack[0].x = Sheadx;
	snack[0].y = Sheady;
	snack[0].now = -1;
	creatFood();

	for (int i = 0; i<H; i++)
	{
		for (int j = 0; j<L; j++)
		{
			printf("%c", GameMap[i][j]);
		}
		printf("\n");
	}

	printf("按任意键开始游戏\n");

	getch();//接受一个按键
	getButton();//取出按键
}
void creatFood()
{
	int fx, fy;
	while (true)
	{
		fx = rand() % H;
		fy = rand() % L;

		if (GameMap[fx][fy] == Snode)//fx、fy不在蛇在的位置
		{
			GameMap[fx][fy] = Sfood;
			break;
		}
	}
}
void getButton()
{
	if (kbhit() != 0)
	{
		int key;
		while (kbhit() != 0)
		{
			key = getch();
		}
		//如果有多个按键取最后一个
		switch (key)
		{
		case 75:
			snack[0].now = 0;
			break;//左
		case 77:
			snack[0].now = 1;
			break;//右
		case 72:
			snack[0].now = 2;
			break;//上
		case 80:
			snack[0].now = 3;
			break;//下
		default:
			break;
		}
	}
}
void Show()
{
	while (true)
	{
		_sleep(velocity);
		//每XX毫秒刷新一次地图
		getButton();
		move();//移动
		if (over)
		{
			printf("\n-----游戏结束-----\n");
			break;
		}
		if (Slen == H*L)
		{
			printf("\n-----游戏胜利-----\n");
			break;
		}
		//刷新地图
		system("cls");
		for (int i = 0; i<H; i++)
		{
			for (int j = 0; j<L; j++)
			{
				printf("%c", GameMap[i][j]);
			}
			printf("\n");
		}
		printf("\n----- 当前积分：%d -----\n", Slen - 1);
		printf("输入上下左右键改变方向");
	}
}
void move()
{
	int l = Slen;
	//保存蛇头位置
	int x = snack[0].x;
	int y = snack[0].y;
	GameMap[x][y] = '.';

	snack[0].x = snack[0].x + change_x[snack[0].now];
	snack[0].y = snack[0].y + change_y[snack[0].now];
	//更新蛇头位置
	if (!IsBorder())over = 1;
	checkHeadMove(x, y);
	if (l == Slen)//没吃到食物，蛇身向前移动
	{
		for (int i = 1; i<Slen; i++)
		{
			if (i == 1)//尾节点
			{
				GameMap[snack[i].x][snack[i].y] = Snode;
			}
			if (i == Slen - 1)//蛇头后面一个节点特殊处理
			{
				snack[i].x = x;
				snack[i].y = y;
				snack[i].now = snack[0].now;
			}
			else//其他节点和前面一个节点一样
			{
				snack[i].x = snack[i + 1].x;
				snack[i].y = snack[i + 1].y;
				snack[i].now = snack[i + 1].now;
			}


			GameMap[snack[i].x][snack[i].y] = Sbody;

		}
	}

}
bool IsBorder()
{
	if (snack[0].x >= 0 && snack[0].x<H
		&&snack[0].y >= 0 && snack[0].y<L)
		return true;
	else
		return false;
}
void checkHeadMove(int x, int y)
{
	if (GameMap[snack[0].x][snack[0].y] == Snode)
		GameMap[snack[0].x][snack[0].y] = Shead;
	else
	{
		if (GameMap[snack[0].x][snack[0].y] == Sfood)//吃到食物
		{
			GameMap[snack[0].x][snack[0].y] = Shead;
			snack[Slen].x = x;//蛇身增加
			snack[Slen].y = y;
			snack[Slen].now = snack[0].now;
			GameMap[snack[Slen].x][snack[Slen].y] = Sbody;
			Slen++;
			creatFood();//吃到食物就产生一个
		}
		else
			over = 1;//碰到自己
	}
}
