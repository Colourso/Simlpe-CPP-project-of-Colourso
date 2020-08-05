#pragma once

#include "common.h"
#include <list>

class Snake
{
public:
	const int MinSpeed = 1;			//�ߵ���С�ٶ�
	const int MaxSpeed = 25;		//�ߵ�����ٶ�
	const int OrgSpeed = 15;		//�ߵ�ԭʼ�ٶ�

private:
	int m_len;						//�ߵĳ���
	int m_speed;					//�ߵ��ٶ�
	Dir m_direction;				//�ߵķ���
	std::list<Point> m_snakelist;	//�ߵ�����
	Point m_tail;					//���ƶ������β���ڵ㣬��Ҫ���ڳ�ʳ��

public:
	Snake();

	int getLen();					//��ȡ����
	int getSpeed();					//��ȡ�ٶ�
	Dir getDirection();				//��ȡ����
	bool setSpeed(int speed);		//�����ٶȣ����óɹ�����true

	void Move();					//�ƶ�һ��
	void EatFood();					//��ʳ��
	void ChangeDir(Dir dir);		//�ı䷽��
	void Dead();					//����

	bool ColideWall(int left, int top, int right, int bottom);	//��ײ��ǽ
	bool ColideSnake();										//��ײ��������
	bool ColideFood(Point point);							//������ʳ��

	void DrawSnake();				//������
	void DrawSnakeHead(Point pos);	//������ͷ
	void DrawSnakeNode(Point pos);	//�����ߵ�������

	std::list<Point> GetSnakeAllNode();


};