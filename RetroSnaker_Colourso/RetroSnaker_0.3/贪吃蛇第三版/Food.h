#pragma once

#include "common.h"
#include "Snake.h"

class Food
{
private:
	Point m_pos;
	bool m_state;

public:
	Food();

	bool getState();
	void setState(bool state);
	Point getPos();				//��ȡʳ������

	void Generate(Snake *snake);//�����µ�ʳ��

	void DrawFood();

};

