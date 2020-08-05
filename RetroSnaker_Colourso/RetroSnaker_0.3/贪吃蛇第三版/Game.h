#pragma once

#include "common.h"
#include "Snake.h"
#include "Food.h"
#include "RankList.h"

class Game
{
private:
	int m_GameState;			//��Ϸ״̬,0����UI��1����Ϸ�У�2�����а�3����Ϸ������
	PlayerMsg m_msg;			//��������
	Snake *m_snake;				//��
	Food *m_food;				//ʳ��
	RankList *m_ranklist;		//���а�

public:
	Game();

	void Init();			//��ʼ��
	void Run();				//���Ƴ���
	void Close();			//�رճ����ͷ���Դ

private:
	void InitData();		//��ʼ������

	void PlayGame();		//��ʼ��Ϸ

	void ShowMainUI();		//չʾ��UI
	void ShowRank();		//���а�չʾ
	void ShowRule();		//չʾ�������

	void DrawGamePlay();	//���Ƴ�ʼ��Ϸ����
	void DrawScore();		//���Ʒ���
	void DrawSnakeLen();	//���Ƴ���
	void DrawSpeed();		//�����ٶ�
	void DrawRunning();		//������������
	void DrawPause();		//������ͣ��ʾ
	void DrawRebegin();		//�������¿�ʼ
	void DrawGameOver();	//������Ϸ����

	void ChangeChooseUI(int left, int top, int right, int bottom, int kind);//�޸�ѡ�е�ѡ����ɫ
	void ClearRegion(int left, int top, int right, int bottom);		//ʹ�ú�ɫ���ָ������
};

