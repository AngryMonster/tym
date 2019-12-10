#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdbool.h>

#define M 23           //宽
#define N 75           //长
#define up 0           //方向
#define down 1
#define left 2
#define right 3

int map[M][N];
int snake[M*N][2];
int slength;
int direction;
bool eaten=false;
int score=0;
int wait_time=500;

void init()   //程序化的初始操作
{
    srand((unsigned)time(NULL));              //设置随机种子为现在的时间
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);   //隐藏指针
    int i,j;
    for (i=0; i<M; i++)    //定义地图的左右边界
    {
        map[i][0]=1;
        map[i][N-1]=1;
    }
    for (j=0; j<N; j++)    //定义地图的上下边界
    {
        map[0][j]=1;
        map[M-1][j]=1;
    }
    slength=4;                //初始化蛇身和蛇头长度
    snake[0][0]=M/2;
    snake[0][1]=N/2;
    for (i=1; i<4; i++)       //给开始的蛇几个坐标
    {
        snake[i][0]=snake[0][0]+i;
        snake[i][1]=snake[0][1];
    }
}

void gotoxy(int i,int j)        //将光标移动到指定位置
{
    COORD position={j,i};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),position);
}

void drawmap()
{
    int i,j;
    gotoxy(0,0);           //每次画完一次地图后就初始化
    for(i=0; i<M; i++)
    {
        for(j=0; j<N; j++)  //画地图
           {
              if (map[i][j]==0)
                 printf(" ");
              else
                 printf("#");
           }
           printf("\n");
    }
}

void drawsnake()      //画蛇
{
    int i;
    for(i=0; i<slength; i++)
    {
        gotoxy(snake[i][0],snake[i][1]);
        printf("@");
    }
}

void move()
{
    int i;
    gotoxy(snake[slength-1][0],snake[slength-1][1]);   //每次循环蛇的最后面被擦掉
    printf(" ");                               //擦尾巴
    if (eaten)
    {
        slength++;
        eaten=false;         //防止无限变长
    }
    for (i=slength; i>0; i--)                  //让后面一个点等于前面一个点  让蛇移动的最基本函数
    {
        snake[i][0]=snake[i-1][0];
        snake[i][1]=snake[i-1][1];
    }
    switch(direction)
    {
    case up:

        snake[0][0]--;      //向上
        break;

    case down:

        snake[0][0]++;      //向下
        break;

    case left:

        snake[0][1]--;      //向左
        break;

    case right:

        snake[0][1]++;      //向右
        break;

    }
}

void key()        //用键盘控制方向
{
    if (kbhit()!=0)                        //kbhit()函数功能为检测到键盘输入返回0否则返回非0
    {
        char in;
        while(!kbhit() == 0)               //getch()函数功能为不用回车直接输入
            in=getch();
        switch(in)
        {
        case 'w':
        case 'W':
            if (direction!=down)          //不能反方向走
                direction=up;
            break;
        case 's':
        case 'S':
            if (direction!=up)
                direction=down;
            break;
        case 'a':
        case 'A':
            if (direction!=right)
                direction=left;
            break;
        case 'd':
        case 'D':
            if (direction!=left)
                direction=right;
            break;
        case '1':
            wait_time=100;
            break;
        case '2':
            wait_time=250;
            break;
        case '3':
            wait_time=400;
            break;

        }
    }
}

int check(int ii,int jj)        //筛选合格的点
{
    int i;
    if (map[ii][jj] == 1)       //如果有障碍物则返回0
        return 0;
    for (i=0; i<slength; i++)   //碰到蛇身则返回0
    {
        if (ii == snake[i][0] && jj == snake[i][0])
            return 0;
    }
    if (ii == 0 || ii == M-1 || jj == 0 || jj == N-1)    //碰到边界则返回0
        return 0;
    return 1;
}

void food()      //画出食物
{
    int i,j;
    do
    {
        i=rand()%M;    //宽之间的
        j=rand()%N;    //长之间的
    }
    while (check(i,j) == 0);
    map[i][j]=-1;
    gotoxy(i,j);
    printf("$");
}

bool gameover()
{
    bool isgameover=false;
    int snakeX=snake[0][0],snakeY=snake[0][1];
    if (snakeX == 0 || snakeX == M-1 || snakeY == N-1 || snakeY == 0)        //判断蛇头有没有撞墙
        isgameover=true;
    for (int i=1; i<slength; i++)
    {
        if(snake[i][0]==snakeX && snake[i][1]==snakeY)                       //判断蛇头有没有吃到自己
            isgameover=true;
    }
    return isgameover;
}

void printfscore()
{
   gotoxy(0,N+2);
   printf("  贪吃蛇小游戏");
   gotoxy(1,N+2);
   printf("  愤怒的小怪兽制作");
   gotoxy(4,N+2);
   printf("  得分:%d",score);
   gotoxy(6,N+2);
   printf("  1.困难模式");
   gotoxy(7,N+2);
   printf("  2.普通模式");
   gotoxy(8,N+2);
   printf("  3.简单模式");
}

int main()
{
    init();
    drawmap();
    food();
    while(1)
    {
        drawsnake();
        printfscore();
        Sleep(wait_time);       //等待500毫秒
        key();
        move();
        if(map[snake[0][0]][snake[0][1]] == -1)     //如果蛇头碰到食物就返回0
        {
            eaten=true;
            score+=10;
            food();
            map[snake[0][0]][snake[0][1]]=0;
        }
        if(gameover())                     //如果isgameover为真则跳出
        {
            system("cls");                 //清除屏幕内容
            printf("GameOver\n");
            system("pause");
            break;
        }
    }
    getchar();                  //防止在框内结束
    return 0;
}
