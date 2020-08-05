#include "Game.h"
#include <conio.h>
#include <graphics.h>

Game::Game()
{
	this->m_snake = nullptr;
	this->m_food = nullptr;

	this->m_GameState = 0;
	this->m_msg = PlayerMsg();
	this->m_ranklist = new RankList();
}

void Game::Init()
{
	initgraph(640, 480);
}

void Game::Run()
{
	ShowMainUI();

	while (true)
	{
		if (m_GameState == 0 && MouseHit())	//�������������
		{
			MOUSEMSG mouse = GetMouseMsg();//��ȡ�������Ϣ
			if (mouse.mkLButton)
			{
				if (mouse.x >= 240 && mouse.x <= 400 && mouse.y >= 195 && mouse.y <= 235)
				{
					//���������˿�ʼѡ��
					ChangeChooseUI(240, 195, 400, 235, 1);
					m_GameState = 1;

					FlushMouseMsgBuffer();//��������Ϣ��������

					Sleep(500);
					PlayGame();
				}
				else if (mouse.x >= 240 && mouse.x <= 400 && mouse.y >= 255 && mouse.y <= 295)
				{
					//���а�ѡ��
					ChangeChooseUI(240, 255, 400, 295, 2);
					m_GameState = 2;

					FlushMouseMsgBuffer();//��������Ϣ��������

					Sleep(500);
					ShowRank();
				}
				else if (mouse.x >= 240 && mouse.x <= 400 && mouse.y >= 315 && mouse.y <= 355)
				{
					//����ѡ��
					ChangeChooseUI(240, 315, 400, 355, 3);
					m_GameState = 3;

					FlushMouseMsgBuffer();//��������Ϣ��������

					Sleep(500);
					ShowRule();
				}
				else if (mouse.x >= 240 && mouse.x <= 400 && mouse.y >= 375 && mouse.y <= 415)
				{
					//�˳�ѡ��
					ChangeChooseUI(240, 375, 400, 415, 4);
					Sleep(1000);
					return;
				}
			}
		}

		if ((m_GameState == 2 || m_GameState == 3) && MouseHit()) //�����а������Ϸ�����е��
		{
			MOUSEMSG mouse = GetMouseMsg();//��ȡ�������Ϣ
			if (mouse.mkLButton)
			{
				if (mouse.x >= 20 && mouse.x <= 63 && mouse.y >= 20 && mouse.y <= 43)
				{
					//�������ѡ��
					ChangeChooseUI(20, 20, 63, 43, 5);
					Sleep(500);

					FlushMouseMsgBuffer();//��������Ϣ��������

					m_GameState = 0;
					ShowMainUI();
				}
			}
		}
	}
}

void Game::Close()
{
	closegraph();
}

void Game::InitData()
{
	if (this->m_snake != nullptr)
	{
		delete(this->m_snake);
		this->m_snake = nullptr;
	}
	if (this->m_food != nullptr)
	{
		delete(this->m_food);
		this->m_food = nullptr;
	}

	this->m_msg = PlayerMsg();
	this->m_snake = new Snake();
	this->m_food = new Food();
}

void Game::PlayGame()
{
	InitData();
	DrawGamePlay();

	BeginBatchDraw();

	bool backMainUI = false;
	bool rePlayGame = false;
	bool changeShowData = false;

	while (true)
	{
		changeShowData = false;
		//ʳ��
		if (this->m_food->getState() == false)
		{
			m_food->Generate(this->m_snake);
			m_food->DrawFood();
		}

		//�������
		if (_kbhit())
		{
			char key = _getch();
			switch (key)
			{
			case 72:  //��
			case 119: //w
				m_snake->ChangeDir(Dir::DIR_UP);
				break;
			case 80:  //��
			case 115: //s
				m_snake->ChangeDir(Dir::DIR_DOWN);
				break;
			case 75: //��
			case 97: //a
				m_snake->ChangeDir(Dir::DIR_LEFT);
				break;
			case 77:  //��
			case 100: //d
				m_snake->ChangeDir(Dir::DIR_RIGHT);
				break;
			case 99: //c ����
				changeShowData = m_snake->setSpeed(m_snake->getSpeed() + 1);
				break;
			case 120://x ����
				changeShowData = m_snake->setSpeed(m_snake->getSpeed() - 1);
				break;
			case 122://z �ع�ԭ��
				changeShowData = m_snake->setSpeed(m_snake->OrgSpeed);
				break;
			case 32: //�ո���ͣ
				DrawPause();	//״̬����ʾ
				FlushBatchDraw();//������������
				while (true)
				{
					if (_kbhit())
					{
						char key = _getch();
						if (key == 32)		//���ո����
						{
							ClearRegion(505, 240, 640, 480);
							DrawRunning();	//���Ƴ�������������
							FlushBatchDraw();//������������
							break;
						}
						else if (key == 27) //esc�� �˳�
						{
							ChangeChooseUI(510, 347, 588, 373, 6);
							FlushBatchDraw();//������������
							Sleep(500);

							backMainUI = true;
							break;
						}
					}

					if (MouseHit())
					{
						MOUSEMSG mouse = GetMouseMsg();//��ȡ�������Ϣ
						if (mouse.mkLButton)
						{
							if (mouse.x >= 510 && mouse.x <= 588 && mouse.y >= 347 && mouse.y <= 373)
							{
								//�������ѡ��
								ChangeChooseUI(510, 347, 588, 373, 6);
								FlushBatchDraw();//������������
								Sleep(500);

								backMainUI = true;
								break;
							}
						}
					}
				}
				break;
			}

			//���Ҫ�ص���UI
			if (backMainUI)
			{
				FlushMouseMsgBuffer();//��������Ϣ��������
				break;		//������ѭ��
			}
		}

		//������ʾ����
		if (changeShowData)
		{
			ClearRegion(505, 115, 640, 200);
			DrawScore();
			DrawSnakeLen();
			DrawSpeed();
		}

		//�ƶ�
		m_snake->Move();

		//��ʳ��
		if (m_snake->ColideFood(m_food->getPos()))
		{
			m_snake->EatFood();

			this->m_food->setState(false);

			//�������ӣ���������
			this->m_msg.score += 10;

			//��������
			ClearRegion(505, 115, 640, 200);
			DrawScore();
			DrawSnakeLen();
			DrawSpeed();
		}

		//��ײ���
		if (m_snake->ColideWall(10, 10, 470, 470) || m_snake->ColideSnake())
		{
			////��Ϸ����
			m_snake->Dead();

			//�����Ϸ�����ػ���ȥ����
			ClearRegion(0, 0, 479, 480);

			m_snake->DrawSnake();

			//�������¿�ʼ
			DrawRebegin();

			//����GameOVer
			DrawGameOver();

			FlushBatchDraw();//��������

			while (true)
			{
				if (_kbhit())
				{
					char key = _getch();
					if (key == 32)		//���ո����
					{
						//�������һ��ѡ��
						ChangeChooseUI(510, 397, 588, 423, 7);
						FlushBatchDraw();//������������
						Sleep(500);

						rePlayGame = true;
						break;
					}
					else if (key == 27) //esc�� �˳�
					{
						ChangeChooseUI(510, 347, 588, 373, 6);
						FlushBatchDraw();//������������
						Sleep(500);

						backMainUI = true;
						break;
					}
				}

				if (MouseHit())
				{
					MOUSEMSG mouse = GetMouseMsg();//��ȡ�������Ϣ
					if (mouse.mkLButton)
					{
						if (mouse.x >= 510 && mouse.x <= 588 && mouse.y >= 347 && mouse.y <= 373)
						{
							//�������ѡ��
							ChangeChooseUI(510, 347, 588, 373, 6);
							FlushBatchDraw();//������������
							Sleep(500);

							backMainUI = true;
							break;
						}
						else if (mouse.x >= 510 && mouse.x <= 588 && mouse.y >= 397 && mouse.y <= 423)
						{

							//�������һ��ѡ��
							ChangeChooseUI(510, 397, 588, 423, 7);
							FlushBatchDraw();//������������
							Sleep(500);

							rePlayGame = true;
							break;
						}
					}
				}
			}

			//���Ҫ�ص���UI
			if (backMainUI)
			{
				FlushMouseMsgBuffer();//��������Ϣ��������
				break;		//������ѭ��
			}

			//���Ҫ���¿�ʼ��Ϸ
			if (rePlayGame)
			{
				FlushMouseMsgBuffer();//��������Ϣ��������
				break;		//������ѭ��
			}
		}

		//�����Ļ
		setbkcolor(BG_COLOR);//���ñ���ɫ
		ClearRegion(0, 0, 479, 480);

		this->m_snake->DrawSnake();

		if (this->m_food->getState() == true)
		{
			m_food->DrawFood();
		}

		FlushBatchDraw();//������������

		int sleep = 30 - m_snake->getSpeed();
		Sleep(sleep * 10);
	}

	EndBatchDraw();//������������

	//���Ҫ�ص���UI
	if (backMainUI)
	{
		m_msg.len = m_snake->getLen();

		m_ranklist->SaveMsg(m_msg);
		m_ranklist->SaveToRank();

		m_GameState = 0;
		ShowMainUI();
	}

	//���Ҫ���¿�ʼ��Ϸ
	if (rePlayGame)
	{
		m_msg.len = m_snake->getLen();

		m_ranklist->SaveMsg(m_msg);
		m_ranklist->SaveToRank();

		PlayGame();
	}

}

void Game::ShowMainUI()
{
	//�����Ļ
	setbkcolor(BG_COLOR);//���ñ���ɫ
	cleardevice();

	/*--------------���Ʊ���----------*/
	settextstyle(80, 0, L"΢���ź�");
	settextcolor(0XFFFFFF);
	outtextxy(230, 60, L"̰����");

	settextstyle(18, 0, L"΢���ź�");
	outtextxy(380, 140, L"By Colourso");

	settextstyle(20, 0, L"΢���ź�");
	settextcolor(0XFF5555);
	outtextxy(365, 160, L"(www.colourso.top)");
	settextcolor(0XFFFFFF);

	/*--------------����ѡ��----------*/

	setlinecolor(0xFF00FF);
	rectangle(240, 195, 400, 235);
	rectangle(240, 255, 400, 295);
	rectangle(240, 315, 400, 355);
	rectangle(240, 375, 400, 415);


	settextstyle(28, 0, L"΢���ź�");
	settextcolor(0x55FFFF);//��ɫ

	outtextxy(297, 200, L"��ʼ");
	outtextxy(287, 260, L"���а�");
	outtextxy(297, 320, L"����");
	outtextxy(297, 380, L"�˳�");

	settextstyle(24, 0, L"΢���ź�");
	settextcolor(0xAAAAAA);
	outtextxy(255, 450, L"(�����ѡ��)");
}

void CharToTCHAR(const char * _char, TCHAR * tchar)
{
	int iLength;
	iLength = MultiByteToWideChar(CP_UTF8, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, _char, strlen(_char) + 1, tchar, iLength);

}

void Game::ShowRank()
{
	//�����Ļ
	setbkcolor(BG_COLOR);//���ñ���ɫ
	cleardevice();

	/*--------------���ƹ���----------*/
	settextstyle(60, 0, L"΢���ź�");
	settextcolor(0XFFFFFF);
	outtextxy(160, 40, L"̰�������а�");

	/*--------------���Ʒ��ؼ�----------*/
	setlinecolor(0xFFFFFF);
	rectangle(20, 20, 63, 43);

	settextcolor(0XFFFFFF);
	settextstyle(20, 0, L"����");
	outtextxy(22, 22, L" �� ");

	/*---------------�������а���Ϣ---------------*/
	settextcolor(0XFFFFFF);
	settextstyle(20, 0, L"����");
	outtextxy(140, 120, L"����");
	outtextxy(240, 120, L"����");
	outtextxy(340, 120, L"����");
	outtextxy(490, 120, L"����");

	std::vector<PlayerMsg> msg = m_ranklist->getRankList();
	if (msg.size() == 0)
	{
		settextcolor(0x5555FF);
		settextstyle(40, 0, L"����");
		outtextxy(230, 200, L"��������");
	}
	for (int i = 0; i < msg.size(); ++i)
	{
		if (i == 0)
		{
			settextcolor(0x5555FF);
			settextstyle(15, 0, L"����");
		}
		else
		{
			settextcolor(0XFFFFFF);
			settextstyle(15, 0, L"����");
		}
		TCHAR t[40];
		_stprintf_s(t, _T("%d"), msg[i].id);
		outtextxy(145, 150 + 30 * i, t);

		_stprintf_s(t, _T("%d"), msg[i].score);
		outtextxy(245, 150 + 30 * i, t);

		_stprintf_s(t, _T("%d"), msg[i].len);
		outtextxy(345, 150 + 30 * i, t);

		//_stprintf_s(t, _T("%s"), msg[i].r_time.c_str());
		CharToTCHAR(msg[i].r_time.c_str(), t);
		outtextxy(450, 150 + 30 * i, t);

		settextcolor(0XFFFFFF);
	}
}

void Game::ShowRule()
{
	//�����Ļ
	setbkcolor(BG_COLOR);//���ñ���ɫ��ɫ
	cleardevice();

	/*--------------���ƹ���----------*/
	settextstyle(60, 0, L"΢���ź�");
	settextcolor(0XFFFFFF);
	outtextxy(160, 40, L"̰���߰�������");

	settextcolor(0XFFFFFF);
	settextstyle(20, 0, L"����");
	outtextxy(230, 150, L"�� �� �� �� ���Ʒ���");
	outtextxy(230, 180, L"w  a  s  d  ���Ʒ���");
	outtextxy(230, 210, L"�ٶȵȼ�1-25��Ĭ��12");
	outtextxy(240, 240, L"c�����٣�x������");
	outtextxy(240, 270, L"z���ָ�ԭʼ�ٶ�");
	outtextxy(240, 300, L"�ո����ͣ/����");

	outtextxy(180, 350, L"�뽫���뷨����Ӣ�����뷨״̬��");

	/*--------------���Ʒ��ؼ�----------*/

	setlinecolor(0xFFFFFF);
	rectangle(20, 20, 63, 43);

	settextcolor(0XFFFFFF);
	settextstyle(20, 0, L"����");
	outtextxy(22, 22, L" �� ");
}

void Game::DrawGamePlay()
{
	//�����Ļ
	setbkcolor(BG_COLOR);//���ñ���ɫ
	cleardevice();

	//�����Ϊ2����ɫʵ�ߣ��ָ���Ϸ��
	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 2);
	line(482, 0, 482, 480);

	//���ߺ�ʳ��
	this->m_snake->DrawSnake();

	//���Ʒ���
	DrawScore();

	//����������
	DrawSnakeLen();

	//�����ٶ�
	DrawSpeed();

	//������Ϸ״̬
	DrawRunning();
}

void Game::DrawScore()
{
	TCHAR s[] = _T("��÷�����");
	settextstyle(16, 0, _T("����"));
	settextcolor(0xFFFFFF);//��
	outtextxy(510, 120, s);

	settextcolor(0xFF5555);//����
	TCHAR t[5];
	_stprintf_s(t, _T("%d"), m_msg.score); // �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
	outtextxy(590, 120, t);
	settextcolor(0xFFFFFF);//��
}

void Game::DrawSnakeLen()
{
	settextstyle(16, 0, L"����");
	outtextxy(510, 150, L"�����ȣ�");

	settextcolor(0xFF55FF);//����
	TCHAR t[5];
	_stprintf_s(t, _T("%d"), m_snake->getLen()); // �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
	outtextxy(590, 150, t);
	settextcolor(0xFFFFFF);//��
}

void Game::DrawSpeed()
{
	TCHAR s[] = _T("��ǰ�ٶȣ�");
	settextstyle(16, 0, _T("����"));
	outtextxy(510, 180, s);

	int speed = m_snake->getSpeed();//�ٶȵȼ���ʾΪ1 - 25
	if (speed <= 9)
	{
		settextcolor(0x00AA00);//»
	}
	else if (speed >= 18)
	{
		settextcolor(0x0000AA);//��
	}
	else
	{
		settextcolor(0x55FFFF);//��
	}
	TCHAR t[5];
	_stprintf_s(t, _T("%d"), speed); // �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
	outtextxy(590, 180, t);
	settextcolor(0xFFFFFF);//��
}

void Game::DrawRunning()
{
	settextcolor(0x55FF55);//����
	settextstyle(16, 0, L"����");
	outtextxy(510, 250, L"��Ϸ ����");
	outtextxy(510, 280, L"ף��Ŀ���");
	settextcolor(0xFFFFFF);//��
}

void Game::DrawPause()
{
	settextcolor(0xFF55FF);//����
	settextstyle(16, 0, L"����");
	outtextxy(510, 250, L"��Ϸ ��ͣ");
	outtextxy(510, 280, L"�ո������");
	outtextxy(510, 310, L"Esc���˳�");
	settextcolor(0xFFFFFF);//��

	setlinecolor(0xFFFFFF);
	rectangle(510, 347, 588, 373);

	settextcolor(0XFFFFFF);
	settextstyle(16, 0, L"����");
	outtextxy(517, 352, L"�˳���Ϸ");
}

void Game::DrawRebegin()
{
	settextcolor(0x5555FF);//����
	settextstyle(16, 0, L"����");
	outtextxy(510, 250, L"��Ϸ ����");
	outtextxy(510, 280, L"�ո������һ��");
	outtextxy(510, 310, L"Esc���˳�");
	settextcolor(0xFFFFFF);//��

	setlinecolor(0xFFFFFF);
	rectangle(510, 347, 588, 373);
	rectangle(510, 397, 588, 423);

	settextcolor(0XFFFFFF);
	settextstyle(16, 0, L"����");
	outtextxy(517, 352, L"�˳���Ϸ");
	outtextxy(517, 402, L"����һ��");
}

void Game::DrawGameOver()
{
	settextcolor(0xFFFFFF);//����
	settextstyle(48, 0, L"����");
	outtextxy(170, 210, L"GAMEOVER");
}

void Game::ChangeChooseUI(int left, int top, int right, int bottom, int kind)
{
	setfillcolor(0XFFFFFF);				//ʹ�ð�ɫ�����һ����
	fillrectangle(left, top, right, bottom);

	setlinecolor(0x55FF55);				//���߱߿�����Ϊ����ɫ
	rectangle(left, top, right, bottom);

	settextstyle(28, 0, L"΢���ź�");
	settextcolor(0xFF5555);//����ɫ

	setbkcolor(0XFFFFFF);				//���ñ���ɫΪ��ɫ�����ֵı���ɫ�ͻ��ɰ�ɫ
	switch (kind)
	{
	case 1:
		outtextxy(297, 200, L"��ʼ");
		break;
	case 2:
		outtextxy(287, 260, L"���а�");
		break;
	case 3:
		outtextxy(297, 320, L"����");
		break;
	case 4:
		outtextxy(297, 380, L"�˳�");
		break;
	case 5:
		settextcolor(0);
		settextstyle(20, 0, L"����");
		outtextxy(22, 22, L" �� ");
		break;
	case 6:
		settextcolor(0);
		settextstyle(16, 0, L"����");
		outtextxy(517, 352, L"�˳���Ϸ");
		break;
	case 7:
		settextcolor(0);
		settextstyle(16, 0, L"����");
		outtextxy(517, 402, L"����һ��");
		break;
	}
	setbkcolor(BG_COLOR);	//�ָ�����ɫ
}

void Game::ClearRegion(int left, int top, int right, int bottom)
{
	//��ɫ��ȫ��䣬�ޱ߿�ľ���
	setfillcolor(BG_COLOR);
	setfillstyle(BS_SOLID);
	solidrectangle(left, top, right, bottom);
}
