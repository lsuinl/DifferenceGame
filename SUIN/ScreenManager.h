#pragma once
#include "InputSystem.h"
#include "SUIN.h"
namespace screen
{
	extern int state ; //0:타이틀, 1: 게임진행 2: 종료
	extern int clickLimit, clickTimer; //클릭시간 제한
	extern int keyLimit, keyTimer; //키보드입력시간 제한
	extern int stageLimit, stageTimer; //스테이지 시간제한
	extern int stage; //현재 스테이지
	extern int stageScore;//스테이지 스코어(5개 맞추기)
	extern int score; //전체 정답 개수
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
