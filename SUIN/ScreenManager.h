#pragma once
#include "InputSystem.h"
#include "SUIN.h"
namespace screen
{
	extern int state ; //0:타이틀, 1: 게임진행 2: 종료
	extern int ClickLimit, ClickTimer; //클릭시간 제한
	extern int KeyLimit, KeyTimer; //키보드입력시간 제한
	extern int StageLimit, StageTimer; //스테이지 시간제한
	extern int Stage; //현재 스테이지
	extern int StageScore;//스테이지 스코어(5개 맞추기)
	extern int Score; //전체 정답 개수
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
