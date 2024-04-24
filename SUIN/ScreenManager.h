#pragma once
#include "InputSystem.h"
#include "SUIN.h"
namespace screen
{
	extern int state ; //0:Ÿ��Ʋ, 1: �������� 2: ����
	extern int ClickLimit, ClickTimer; //Ŭ���ð� ����
	extern int KeyLimit, KeyTimer; //Ű�����Է½ð� ����
	extern int StageLimit, StageTimer; //�������� �ð�����
	extern int Stage; //���� ��������
	extern int StageScore;//�������� ���ھ�(5�� ���߱�)
	extern int Score; //��ü ���� ����
	extern int imagenum[5];
	extern int image;
	extern int mousex, mousey;
	extern char* names;

	void SetImage(int num);
	void TimesUpdate(ULONGLONG delta);
	void Title();
	void Ing();
	void End();
	void StartInput(input::MouseState mouse);
	void IngInput(input::MouseState mouse);
	void EndInput(input::MouseState mouse);
	void DrawStartInfo();
	void DrawIngInfo();
}
