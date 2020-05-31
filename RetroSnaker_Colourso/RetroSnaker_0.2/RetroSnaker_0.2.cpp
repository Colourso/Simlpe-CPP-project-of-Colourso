#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

//��ͷ����Ԥ����
#define UP 1	//��
#define RIGHT 2 //��
#define DOWN 3  //��
#define LEFT 4  //��

#define MAX_SPEED 5		//����ٶ�
#define MIN_SPEED 29	//��С�ٶ�
#define ORG_SPEED 18	//ԭʼ�ٶ�

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
int g_speed = ORG_SPEED; //�����ٶȣ����ӳٵȴ�ʱ��
Snake snake;	 //��Ϸ�е��߱���
Food food;		 //ʳ��

/*-------------��������---------------*/

void init();	 //��ʼ��
void close();	 //�ͷ���Դ
void play();	 //��ʼ��Ϸ

void initdata(); //��Ϸ���ݳ�ʼ��
void drawgame(); //���ƽ���
void gameplay(); //��Ϸ���̿���


		/*---��Ϸ����غ���---*/

void drawfood(Food food);		  //����ʳ��
void drawsnake(Snake snake);	  //������
void drawsnakenode(Node node);	  //�����ߵĽ��
void drawsnakehead(Node node);  //���ߵ�ͷ��
void clearsnakenode(Node node);   //����ߵĽ��
void snakebodymove(Snake &snake); //����������ǰ����
bool collidesnake(Snake snake);   //��ײ������

		/*---����չʾ����غ���---*/

void cleardisplay();			  //�������չʾ������
void drawscore();				  //���Ʒ���
void drawnodes();				  //�����߳���
void drawspeed();				  //�����ٶ�

		/*---����״̬����غ���---*/

void clearstate();				//�������״̬������
void drawpause();				//�����ͣ��Ϣ
void drawruntime();				//����������е���Ϣ
void drawover();				//�����Ϸ������Ϣ
void drawoverchoose();			//������Ϸ�������ѡ��


/*-------------�������---------------*/

int main()
{
	init();
	play();
	close();
	return 0;
}

/*-------------����ʵ��---------------*/

//��ʼ��
void init()
{
	initgraph(640, 480);   //��ʼ������
}

//��Ϸ
void play()
{
	initdata();
	drawgame();
	gameplay();
}

//�ͷ���Դ
void close()
{
	_getch();
	closegraph();
}

//��ʼ������
void initdata()
{
	//����ʳ���ʼ��
	//ʳ���ʼ����Ϊ(310,230)
	food.x = 310;
	food.y = 230;
	food.exist = true;

	//�߳�ʼ����Ϊ3����ͷ����Ϊ(210,230)

	//����ߵ�ͷ�ڵ��β�ڵ�����Ϣ
	if (snake.head != NULL && snake.tail != NULL)
	{
		LinkNode linknode = snake.head;
		while (linknode != snake.tail)
		{
			linknode = linknode->next;
			free(linknode->pre);
		}
		free(snake.tail);
		snake.head = NULL;
		snake.tail = NULL;
	}


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
	//��ʼ���ٶ�
	g_speed = ORG_SPEED;
}

//���ƽ���
void drawgame()
{
	//�����Ļ
	setbkcolor(0);//���ñ���ɫ��ɫ
	cleardevice();

	//�����Ϊ2����ɫʵ�ߣ��ָ���Ϸ��
	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 2);
	line(482, 0, 482, 480);

	/*--------------���ƹ��ܽ�����----------*/
	settextstyle(16, 0, L"����");
	settextcolor(0XFFFFFF);
	outtextxy(530, 20, L"�淨����");
	settextstyle(14, 0, L"����");
	outtextxy(490, 50, L"�� �� �� �� ���Ʒ���");
	outtextxy(490, 70, L"w a s d ���Ʒ���");
	outtextxy(490, 90, L"�ٶȵȼ�1-25��Ĭ��12");
	outtextxy(500, 110, L"c�����٣�x������");
	outtextxy(500, 130, L"z���ָ�ԭʼ�ٶ�");
	outtextxy(500, 150, L"�ո����ͣ/����");
	outtextxy(500, 170, L"Esc��ֱ�ӹرճ���");

	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 2);
	line(482, 190, 640, 190);

	/*--------------��������չʾ��----------*/
	settextstyle(16, 0, L"����");
	settextcolor(0XFFFFFF);
	outtextxy(530, 200, L"����չʾ");

	//���Ʒ���
	drawscore();

	//����������
	drawnodes();

	//�����ٶ�
	drawspeed();

	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 2);
	line(482, 300, 640, 300);

	/*--------------���Ƴ���״̬��----------*/
	settextstyle(16, 0, L"����");
	settextcolor(0XFFFFFF);
	outtextxy(530, 310, L"��Ϸ״̬");

	//���Ƴ�������������
	drawruntime();

	settextstyle(14, 0, L"����");
	settextcolor(0XFFFFFF);
	outtextxy(530, 450, L"By Colourso");//������Ϣ
	outtextxy(510, 465, L"www.colourso.top");

	/*--------------��Ϸ��----------*/
	//����ʳ��
	drawfood(food);

	//������
	drawsnake(snake);
}

//��ʼ��Ϸ
void gameplay()
{
	Node ends; //ĩβ���
	bool speedchange = false;
	BeginBatchDraw();
	while (true)
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

		//�������
		if (_kbhit())
		{
			char key = _getch();
			switch (key)
			{
			case 72:  //��
			case 119: //w
				if (snake.direction != UP && snake.direction != DOWN)
					snake.direction = UP;
				break;
			case 80:  //��
			case 115: //s
				if (snake.direction != UP && snake.direction != DOWN)
					snake.direction = DOWN;
				break;
			case 75: //��
			case 97: //a
				if (snake.direction != LEFT && snake.direction != RIGHT)
					snake.direction = LEFT;
				break;
			case 77:  //��
			case 100: //d
				if (snake.direction != LEFT && snake.direction != RIGHT)
					snake.direction = RIGHT;
				break;
			case 99: //c ����
				if (g_speed > MAX_SPEED)
				{
					g_speed -= 1;
					speedchange = true;
				}
				break;
			case 120://x ����
				if (g_speed < MIN_SPEED)
				{
					g_speed += 1;
					speedchange = true;
				}
				break;
			case 122://z �ع�ԭ��
				g_speed = ORG_SPEED;
				speedchange = true;
				break;
			case 32: //�ո���ͣ
				drawpause();	//״̬����ʾ
				while (true)
				{
					if (_kbhit())
					{
						char key = _getch();
						if (key == 32)		//���ո����
						{
							clearstate();
							drawruntime();	//���Ƴ�������������
							break;
						}
					}
				}
				break;
			case 27://Escֱ���˳�
				exit(0);
				break;
			}
		}

		//�ٶȱ仯�Ļ���������չʾ������
		if (speedchange)
		{
			speedchange = false;
			cleardisplay();
			drawscore();
			drawnodes();
			drawspeed();
		}

		//�����߹���β�˽��
		clearsnakenode(ends);

		//ǰ��������ͷ���߹��Ľڵ�
		switch (snake.direction)
		{
		case UP:
		{
			//ͷ��y-20,x����
			snakebodymove(snake);
			snake.head->y -= 20;
			drawsnakehead(*(snake.head));
			drawsnakenode(*(snake.head->next));//��ԭ����ͷ������������ɫ
			break;
		}
		case RIGHT:
		{
			//ͷ��x+20,y����
			snakebodymove(snake);
			snake.head->x += 20;
			drawsnakehead(*(snake.head));
			drawsnakenode(*(snake.head->next));
			break;
		}
		case DOWN:
		{
			//ͷ��y+20,x����
			snakebodymove(snake);
			snake.head->y += 20;
			drawsnakehead(*(snake.head));
			drawsnakenode(*(snake.head->next));
			break;
		}
		case LEFT:
		{
			//ͷ��x-20,y����
			snakebodymove(snake);
			snake.head->x -= 20;
			drawsnakehead(*(snake.head));
			drawsnakenode(*(snake.head->next));
			break;
		}
		}

		//�߳�ʳ��
		if (snake.head->x == food.x && snake.head->y == food.y && food.exist)
		{
			food.exist = false;

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

			cleardisplay();
			drawscore();
			drawnodes();
			drawspeed();
		}


		FlushBatchDraw();

		//��ײ�ж�
		bool collidewall = snake.head->x < 10 || snake.head->x > 470 || snake.head->y < 10 || snake.head->y > 470;
		if (collidewall || collidesnake(snake))
		{
			drawover();
			drawoverchoose();
			while (true)
			{
				if (_kbhit())
				{
					char key = _getch();
					if (key == 114)		//��r���¿�ʼ
					{
						Sleep(1000);	//�ӳ�1��
						play();
					}
					else if (key == 113)//��q�˳�
					{
						cleardevice();
						TCHAR s[] = _T("Game Over");
						settextstyle(48, 0, _T("����"));
						outtextxy(230, 210, s);

						settextstyle(24, 0, _T("����"));
						outtextxy(250, 260, L"��÷�����");

						settextcolor(0xFF5555);//����
						TCHAR t[5];
						_stprintf_s(t, _T("%d"), g_score); // �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
						outtextxy(380, 260, t);
						settextcolor(0xFFFFFF);//��

						Sleep(1500);
						exit(0);
					}
				}
			}
			break;
		}
		Sleep(g_speed * 10);
		EndBatchDraw();
	}
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

//���ߵ�ͷ��
void drawsnakehead(Node node)
{
	//��ɫ��ȫ��䣬�ޱ߿��������
	setfillcolor(0xAA00AA);
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
	drawsnakehead(*linknode);//��ͷ��
	linknode = linknode->next;
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

//�������չʾ��
void cleardisplay()
{
	//��ɫ��ȫ��䣬�ޱ߿��������
	setfillcolor(BLACK);
	setfillstyle(BS_SOLID);
	solidrectangle(485, 220, 640, 290);
}

//���Ʒ���
void drawscore()
{
	TCHAR s[] = _T("��÷�����");
	settextstyle(16, 0, _T("����"));
	outtextxy(500, 230, s);

	settextcolor(0xFF5555);//����
	TCHAR t[5];
	_stprintf_s(t, _T("%d"), g_score); // �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
	outtextxy(580, 230, t);
	settextcolor(0xFFFFFF);//��
}

//�����߳���
void drawnodes()
{
	settextstyle(16, 0, L"����");
	outtextxy(500, 250, L"�����ȣ�");

	settextcolor(0xFF55FF);//����
	TCHAR t[5];
	_stprintf_s(t, _T("%d"), snake.num); // �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
	outtextxy(580, 250, t);
	settextcolor(0xFFFFFF);//��
}

//�����ٶ�
void drawspeed()
{
	TCHAR s[] = _T("��ǰ�ٶȣ�");
	settextstyle(16, 0, _T("����"));
	outtextxy(500, 270, s);

	int speed = 30 - g_speed;//�ٶȵȼ���ʾΪ1 - 25
	if (speed <= 9)
	{
		settextcolor(0x0000AA);//��-
	}
	else if (speed >= 18)
	{
		settextcolor(0x00AA00);//��
	}
	else
	{
		settextcolor(0x55FFFF);//��
	}
	TCHAR t[5];
	_stprintf_s(t, _T("%d"), speed); // �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
	outtextxy(580, 270, t);
	settextcolor(0xFFFFFF);//��
}

//�������״̬������
void clearstate()
{
	//��ɫ��ȫ��䣬�ޱ߿��������
	setfillcolor(BLACK);
	setfillstyle(BS_SOLID);
	solidrectangle(485, 330, 640, 450);
}

//���Ƴ�����ͣ״̬
void drawpause()
{
	settextcolor(0xFF55FF);//����
	settextstyle(16, 0, L"����");
	outtextxy(530, 340, L"��Ϸ��ͣ");
	outtextxy(500, 360, L"���ո������");
	settextcolor(0xFFFFFF);//��
}

//���Ƴ�������������
void drawruntime()
{
	settextcolor(0x55FF55);//����
	settextstyle(16, 0, L"����");
	outtextxy(530, 340, L"����ing");
	settextcolor(0xFFFFFF);//��
}

void drawover()
{
	settextcolor(0x5555FF);//����
	settextstyle(16, 0, L"����");
	outtextxy(530, 340, L"GameOver");
	settextcolor(0xFFFFFF);//��
}

void drawoverchoose()
{
	setlinecolor(0xFFFFFF);
	rectangle(530, 370, 600, 400);
	settextcolor(0x55FF55);//����
	settextstyle(16, 0, L"����");
	outtextxy(540, 377, L"����(r)");
	settextcolor(0xFFFFFF);//��

	setlinecolor(0xFFFFFF);
	rectangle(530, 420, 600, 450);
	settextcolor(0x5555FF);//����
	settextstyle(16, 0, L"����");
	outtextxy(540, 427, L"�˳�(q)");
	settextcolor(0xFFFFFF);//��
}