#pragma once
#include "InputSystem.h"
#include "SUIN.h"
namespace screen
{
	extern int state ; //0:Ÿ��Ʋ, 1: �������� 2: ����
	extern int clickLimit, clickTimer; //Ŭ���ð� ����
	extern int keyLimit, keyTimer; //Ű�����Է½ð� ����
	extern int stageLimit, stageTimer; //�������� �ð�����
	extern int stage; //���� ��������
	extern int stageScore;//�������� ���ھ�(5�� ���߱�)
	extern int score; //��ü ���� ����
	extern int imageNum[5];
	extern int image;
	extern int mousex, mousey;
	extern char* names;

	void TimesUpdate(ULONGLONG delta);

	void Title();

	void Ing();

	void End();

	void StartInput(input::MouseState mouse);

	void IngInput(input::MouseState mouse);

	void EndInput(input::MouseState mouse);
}
