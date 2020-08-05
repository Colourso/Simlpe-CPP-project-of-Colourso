#include "Food.h"
#include <ctime>
#include <stdlib.h>
#include <graphics.h>
#include <algorithm>

Food::Food()
{
	//��ʼ��ʳ�������
	this->m_state = true;
	this->m_pos = Point(310, 230);
}

bool Food::getState()
{
	return this->m_state;
}

void Food::setState(bool state)
{
	this->m_state = state;
}

Point Food::getPos()
{
	return this->m_pos;
}

void Food::Generate(Snake * snake)
{
	//����ʳ��Ҫ���ȡ�ߵ�����ڵ�������Ƿ����ɵ�ʳ����������ߵ�����
	int x = 0;
	int y = 0;
	bool isOk = false;

	while (true)
	{
		//ʹ�������������ʳ��
		srand(time(0));
		x = (rand() % 24) * 20 + 10;
		y = (rand() % 24) * 20 + 10;

		//����Ƿ����ߵ�����
		isOk = 1;
		
		std::list<Point> pos = snake->GetSnakeAllNode();
		std::list<Point>::iterator it = find(pos.begin(),pos.end(),Point(x,y));
		//find()��Ҫ�õ�Point������ == ������


		if (it == pos.end())//����
		{
			this->m_pos = Point(x,y);//�޸�����
			this->m_state = true;
			return;
		}
	}
}

void Food::DrawFood()
{
	//��ɫ��ȫ��䣬�ޱ߿��Բ
	setfillcolor(RED);
	setfillstyle(BS_SOLID);
	solidcircle(this->m_pos.x, this->m_pos.y, FOOD_RADIU);
}
