#pragma once

#include <string>
#include <conio.h>
#include <graphics.h>

//�ߵĽڵ�뾶
#define SNAKE_RADIU 9
//ʳ��İ뾶
#define FOOD_RADIU 8

//�ߵĽڵ���
#define SNAKE_WIDTH 20

//COLORREF BG_COLOR = RGB(0,0,0);		//������ɫΪ��ɫ

#define BG_COLOR 0


//�����ö��
enum class Dir { DIR_UP = 1, DIR_RIGHT = 2, DIR_DOWN = 3, DIR_LEFT = 4 };

//��Ľṹ��
struct Point {
	int x;
	int y;

	Point() :x(-1), y(-1) {}
	Point(int dx, int dy) :x(dx), y(dy) {}
	Point(const Point& point) :x(point.x), y(point.y) {}

	bool operator==(const Point& point)
	{
		return (this->x == point.x) && (this->y == point.y);
	}

};

//��¼������Ϣ
struct PlayerMsg
{
	int id;
	int score;
	int len;
	std::string r_time;	//��¼ʱ��

	PlayerMsg()
	{
		id = 99;
		score = 0;
		len = 0;
		r_time = "";
	}
};

struct SortPlayerMsg 
{
	bool operator()(const PlayerMsg &msg1, const PlayerMsg &msg2)
	{
		if (msg1.score == msg2.score)
		{
			return msg1.r_time > msg2.r_time;
		}
		else return msg1.score > msg2.score;
	}
};