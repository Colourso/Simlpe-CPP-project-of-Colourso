#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

//��ͷ����Ԥ����
#define UP 1	//��
#define RIGHT 2 //��
#define DOWN 3  //��
#define LEFT 4  //��

/*-------------�ṹ�嶨��---------------*/

typedef struct Node //�ߵĽڵ�
{
	int x;			   //�����
	int y;			   //������
	struct Node *next; //ָ����һ���ڵ��ָ��
	struct Node *pre;  //ָ��ǰһ���ڵ��ָ��
} *LinkNode;

struct Snake //�ߵĽṹ��
{
	LinkNode head; //ָ��ͷ�ڵ��ָ��
	LinkNode tail; //ָ��β�ڵ��ָ��
	int direction; //��ͷ����
	int num;	   //�ڵ���Ŀ
};

struct Food //ʳ��ṹ��
{
	int x;		//������
	int y;		//������
	bool exist; //�Ƿ���ڣ�1��ʾ����
};

/*-------------ȫ�ֱ���---------------*/

int g_score = 0; //��÷���
Snake snake;	 //��Ϸ�е��߱���
Food food;		 //ʳ��

/*-------------��������---------------*/

void init();	 //��ʼ��
void drawgame(); //���ƽ���
void gameplay(); //��ʼ��Ϸ
void close();	//�ͷ���Դ

void drawfood(Food food);		  //����ʳ��
void drawsnake(Snake snake);	  //������
void drawsnakenode(Node node);	//�����ߵĽ��
void clearsnakenode(Node node);   //����ߵĽ��
void snakebodymove(Snake &snake); //����������ǰ����
bool collidesnake(Snake snake);   //��ײ������
void drawscore();				  //���Ʒ���
void clearscore();				  //�������

/*-------------�������---------------*/

int main()
{
	init();
	drawgame();
	gameplay();
	close();
	return 0;
}

/*-------------����ʵ��---------------*/

//��ʼ��
void init()
{
	initgraph(640, 480);   //��ʼ������
	//setorigin(0, 480);	 //��������ԭ��
	//setaspectratio(1, -1); //ʹy������Ϊ��

	//����ʳ���ʼ��
	//ʳ���ʼ����Ϊ(310,230)
	food.x = 310;
	food.y = 230;
	food.exist = true;

	//�߳�ʼ����Ϊ3����ͷ����Ϊ(210,230)

	//�ڶ��Ͽ��ٿռ䣬��ֹ�˺������������ڽ�������ȫ�����ͷŵ�
	LinkNode nodes2 = (LinkNode)malloc(sizeof(Node));
	nodes2->x = 170;
	nodes2->y = 230;
	nodes2->next = nullptr;
	LinkNode nodes1 = (LinkNode)malloc(sizeof(Node));
	nodes1->x = 190;
	nodes1->y = 230;
	nodes1->next = nodes2;
	LinkNode nodes0 = (LinkNode)malloc(sizeof(Node));
	nodes0->x = 210;
	nodes0->y = 230;
	nodes0->next = nodes1;
	nodes2->pre = nodes1;
	nodes1->pre = nodes0;
	nodes0->pre = nullptr;
	snake.head = nodes0;
	snake.tail = nodes2;
	snake.direction = RIGHT;
	snake.num = 3;

	//��ʼ����
	g_score = (snake.num - 3) * 10;
}

//���ƽ���
void drawgame()
{
	//�����Ϊ2����ɫʵ�ߣ��ָ�����
	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 2);
	line(482, 0, 482, 480);

	//����ʳ��
	drawfood(food);

	//������
	drawsnake(snake);

	//���Ʒ���
	drawscore();
}

//��ʼ��Ϸ
void gameplay()
{
	Node ends; //ĩβ���
	while (1)
	{
		ends.x = snake.tail->x;
		ends.y = snake.tail->y;

		if (!food.exist)
		{
			int x;
			int y;
			while (1)
			{
				srand(time(0));
				x = (rand() % 24) * 20 + 10;
				y = (rand() % 24) * 20 + 10;

				//ʳ��λ�ü���㷨
				LinkNode linknode = snake.head;
				bool cont = true;
				while (linknode != snake.tail->next)
				{
					if (linknode->x == x && linknode->y == y)
					{
						cont = false;
						break;
					}
					linknode = linknode->next;
				}
				if (cont)
				{
					break;
				}
			}

			food.x = x;
			food.y = y;
			food.exist = true;
			drawfood(food);
		}

		//����
		if (_kbhit())
		{
			char key = _getch();
			switch (key)
			{
			case 72: //��
				if (snake.direction != UP && snake.direction != DOWN)
					snake.direction = UP;
				break;
			case 80: //��
				if (snake.direction != UP && snake.direction != DOWN)
					snake.direction = DOWN;
				break;
			case 75: //��
				if (snake.direction != LEFT && snake.direction != RIGHT)
					snake.direction = LEFT;
				break;
			case 77: //��
				if (snake.direction != LEFT && snake.direction != RIGHT)
					snake.direction = RIGHT;
				break;
			}
		}
		//ǰ��
		switch (snake.direction)
		{
		case UP:
		{
			//ͷ��y-20,x����
			snakebodymove(snake);
			snake.head->y -= 20;
			drawsnakenode(*(snake.head));
			break;
		}
		case RIGHT:
		{
			//ͷ��x+20,y����
			snakebodymove(snake);
			snake.head->x += 20;
			drawsnakenode(*(snake.head));
			break;
		}
		case DOWN:
		{
			//ͷ��y+20,x����
			snakebodymove(snake);
			snake.head->y += 20;
			drawsnakenode(*(snake.head));
			break;
		}
		case LEFT:
		{
			//ͷ��x-20,y����
			snakebodymove(snake);
			snake.head->x -= 20;
			drawsnakenode(*(snake.head));
			break;
		}
		}
		//�����߹��Ľ��
		clearsnakenode(ends);

		//�߳�ʳ��
		if (snake.head->x == food.x && snake.head->y == food.y && food.exist)
		{
			food.exist = false;
			//isClear = false;

			//�����仯
			g_score += 10;

			//��ĩβ�����µĽ�㣬ά��˫������Ĺ�ϵ
			LinkNode nodes = (LinkNode)malloc(sizeof(Node));
			nodes->x = ends.x;
			nodes->y = ends.y;
			nodes->next = snake.tail->next;
			nodes->pre = snake.tail;
			snake.tail->next = nodes;
			snake.tail = nodes;
			++snake.num;

			drawsnakenode(ends);

			clearscore();
			drawscore();
		}

		//��ײ�ж�
		bool collidewall = snake.head->x < 10 || snake.head->x > 470 || snake.head->y < 10 || snake.head->y > 470;
		if (collidewall || collidesnake(snake))
		{
			cleardevice();
			TCHAR s[] = _T("game over");
			settextstyle(48, 0, _T("����"));
			outtextxy(230, 280, s);
			break;
		}
		Sleep(150);
	}
}

//�ͷ���Դ
void close()
{
	_getch();
	closegraph();
}

//����ʳ��
void drawfood(Food food)
{
	//��ɫ��ȫ��䣬�ޱ߿��Բ
	setfillcolor(RED);
	setfillstyle(BS_SOLID);
	solidcircle(food.x, food.y, 8);
}

//�����ߵĽ��
void drawsnakenode(Node node)
{
	//��ɫ��ȫ��䣬�ޱ߿��������
	setfillcolor(GREEN);
	setfillstyle(BS_SOLID);
	solidrectangle(node.x - 9, node.y + 9, node.x + 9, node.y - 9);
}

//����ߵĽ��
void clearsnakenode(Node node)
{
	//��ɫ��ȫ��䣬�ޱ߿��������
	setfillcolor(BLACK);
	setfillstyle(BS_SOLID);
	solidrectangle(node.x - 9, node.y + 9, node.x + 9, node.y - 9);
}

//������
void drawsnake(Snake snake)
{
	LinkNode linknode = snake.head;
	while (linknode != snake.tail->next)
	{
		drawsnakenode(*linknode);
		linknode = linknode->next;
	}
}

//����������ǰ����
void snakebodymove(Snake &snake)
{
	LinkNode linknode = snake.tail;
	while (linknode != snake.head)
	{
		linknode->x = linknode->pre->x;
		linknode->y = linknode->pre->y;

		linknode = linknode->pre;
	}
}

//��ײ������
bool collidesnake(Snake snake)
{
	if (snake.num <= 3)
		return false;
	LinkNode node = snake.head->next;
	while (node != snake.tail->next)
	{
		if (node->x == snake.head->x && node->y == snake.head->y)
			return true;
		node = node->next;
	}
	return false;
}

//���Ʒ���
void drawscore()
{
	TCHAR s[] = _T("��÷�����");
	settextstyle(16, 0, _T("����"));
	outtextxy(485, 300, s);

	TCHAR t[5];
	_stprintf_s(t, _T("%d"), g_score); // �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
	outtextxy(560, 300, t);
}

//�������
void clearscore()
{
	//��ɫ��ȫ��䣬�ޱ߿��������
	setfillcolor(BLACK);
	setfillstyle(BS_SOLID);
	solidrectangle(485, 480, 640, 300);
}
