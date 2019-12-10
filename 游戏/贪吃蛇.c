#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdbool.h>

#define M 23           //��
#define N 75           //��
#define up 0           //����
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

void init()   //���򻯵ĳ�ʼ����
{
    srand((unsigned)time(NULL));              //�����������Ϊ���ڵ�ʱ��
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);   //����ָ��
    int i,j;
    for (i=0; i<M; i++)    //�����ͼ�����ұ߽�
    {
        map[i][0]=1;
        map[i][N-1]=1;
    }
    for (j=0; j<N; j++)    //�����ͼ�����±߽�
    {
        map[0][j]=1;
        map[M-1][j]=1;
    }
    slength=4;                //��ʼ���������ͷ����
    snake[0][0]=M/2;
    snake[0][1]=N/2;
    for (i=1; i<4; i++)       //����ʼ���߼�������
    {
        snake[i][0]=snake[0][0]+i;
        snake[i][1]=snake[0][1];
    }
}

void gotoxy(int i,int j)        //������ƶ���ָ��λ��
{
    COORD position={j,i};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),position);
}

void drawmap()
{
    int i,j;
    gotoxy(0,0);           //ÿ�λ���һ�ε�ͼ��ͳ�ʼ��
    for(i=0; i<M; i++)
    {
        for(j=0; j<N; j++)  //����ͼ
           {
              if (map[i][j]==0)
                 printf(" ");
              else
                 printf("#");
           }
           printf("\n");
    }
}

void drawsnake()      //����
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
    gotoxy(snake[slength-1][0],snake[slength-1][1]);   //ÿ��ѭ���ߵ�����汻����
    printf(" ");                               //��β��
    if (eaten)
    {
        slength++;
        eaten=false;         //��ֹ���ޱ䳤
    }
    for (i=slength; i>0; i--)                  //�ú���һ�������ǰ��һ����  �����ƶ������������
    {
        snake[i][0]=snake[i-1][0];
        snake[i][1]=snake[i-1][1];
    }
    switch(direction)
    {
    case up:

        snake[0][0]--;      //����
        break;

    case down:

        snake[0][0]++;      //����
        break;

    case left:

        snake[0][1]--;      //����
        break;

    case right:

        snake[0][1]++;      //����
        break;

    }
}

void key()        //�ü��̿��Ʒ���
{
    if (kbhit()!=0)                        //kbhit()��������Ϊ��⵽�������뷵��0���򷵻ط�0
    {
        char in;
        while(!kbhit() == 0)               //getch()��������Ϊ���ûس�ֱ������
            in=getch();
        switch(in)
        {
        case 'w':
        case 'W':
            if (direction!=down)          //���ܷ�������
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

int check(int ii,int jj)        //ɸѡ�ϸ�ĵ�
{
    int i;
    if (map[ii][jj] == 1)       //������ϰ����򷵻�0
        return 0;
    for (i=0; i<slength; i++)   //���������򷵻�0
    {
        if (ii == snake[i][0] && jj == snake[i][0])
            return 0;
    }
    if (ii == 0 || ii == M-1 || jj == 0 || jj == N-1)    //�����߽��򷵻�0
        return 0;
    return 1;
}

void food()      //����ʳ��
{
    int i,j;
    do
    {
        i=rand()%M;    //��֮���
        j=rand()%N;    //��֮���
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
    if (snakeX == 0 || snakeX == M-1 || snakeY == N-1 || snakeY == 0)        //�ж���ͷ��û��ײǽ
        isgameover=true;
    for (int i=1; i<slength; i++)
    {
        if(snake[i][0]==snakeX && snake[i][1]==snakeY)                       //�ж���ͷ��û�гԵ��Լ�
            isgameover=true;
    }
    return isgameover;
}

void printfscore()
{
   gotoxy(0,N+2);
   printf("  ̰����С��Ϸ");
   gotoxy(1,N+2);
   printf("  ��ŭ��С��������");
   gotoxy(4,N+2);
   printf("  �÷�:%d",score);
   gotoxy(6,N+2);
   printf("  1.����ģʽ");
   gotoxy(7,N+2);
   printf("  2.��ͨģʽ");
   gotoxy(8,N+2);
   printf("  3.��ģʽ");
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
        Sleep(wait_time);       //�ȴ�500����
        key();
        move();
        if(map[snake[0][0]][snake[0][1]] == -1)     //�����ͷ����ʳ��ͷ���0
        {
            eaten=true;
            score+=10;
            food();
            map[snake[0][0]][snake[0][1]]=0;
        }
        if(gameover())                     //���isgameoverΪ��������
        {
            system("cls");                 //�����Ļ����
            printf("GameOver\n");
            system("pause");
            break;
        }
    }
    getchar();                  //��ֹ�ڿ��ڽ���
    return 0;
}
