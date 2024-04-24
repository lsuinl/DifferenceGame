#include "ScreenManager.h"
#include "SUIN.h"
#include "RenderSystem.h"
#include "MainFeature.h"
#include "InputSystem.h"
#include "Music.h"
#include "util.h"
#include <cstdlib>
namespace screen
{
	 int state = 0; //0:타이틀, 1: 게임진행 2: 종료
	 int ClickLimit = 300, ClickTimer = ClickLimit; //클릭시간 제한
	 int KeyLimit = 90, KeyTimer = KeyLimit; //키보드입력시간 제한
	 int StageLimit = 5000, StageTimer = 0; //스테이지 시간제한
	 int Stage = 0; //현재 스테이지
	 int StageScore = 0;//스테이지 스코어(5개 맞추기)
	 int Score = 0; //전체 정답 개수
	 int imagenum[5] = { 0,0,0,0,0 };
	 int mousex, mousey;
	 char* names = new char[11];
	 bool changeCheck = false;
	 
	void StageNect() {
		 StageScore = 0;
		 StageTimer = 0;
		 Feature::StageInit();
		 Feature::SetPos(imagenum[Stage]);
	}
	void TimesUpdate(ULONGLONG delta) {
		KeyTimer += delta;
		ClickTimer += delta;
		StageTimer += delta;
	}
	void Title()//타이틀
	{
		render::DrawBackGround("source//START.bmp", 2390, 1162,0,0,false);
		DrawStartInfo();
	}
	void Ing()	//게임 진행
	{
		if (Stage == 1) {//2스테이지(0,1)
			state = 2;
			Feature::Ranking(names, Score); //랭킹 갱신
		}
		render::DrawBackGround("source//ING.bmp", 2390, 1162, 0, 0, false);
		const char* preImg = util::IntToChar(imagenum[Stage - 1]);
		const char* curImg = util::IntToChar(imagenum[Stage]);
		const char* part1[] = { "source//", preImg,".bmp" };
		const char* part2[] = { "source//", preImg, "_", preImg , ".bmp" };
		const char* part3[] = { "source//",curImg, ".bmp" };
		const char* part4[] = { "source//", curImg, "_", curImg , ".bmp" };
		char* filename = util::SumChar(part1,3);
		char* filenames = util::SumChar(part2,5);
		char* anifilename = util::SumChar(part3,3);
		char* anifilenames = util::SumChar(part4,5);
		//화면 전환에 따른 랜더링
		if (Feature::GetAnimation()) {
			render::DrawBackGround(filename, 855, 930, 330, 40, false);
			render::DrawBackGround(filenames, 855, 930, 1200, 40, false);
			StageTimer = 0;
		}
		else {
			//타임아웃체크
			if (StageTimer > StageLimit) {
				Stage++;
				StageScore = 0;
				changeCheck = true;
				Feature::SetAnimation(false);
			}
			if (changeCheck == true) {
				StageNect();
				changeCheck = false;
			}
			render::DrawBackGround(anifilename, 855, 930, 330, 40, false);
			render::DrawBackGround(anifilenames, 855, 930, 1200, 40, false);
		}
		
		Feature::DrawCorrect(); //정답 렌더링
		Feature::DrawAnimation(anifilename, anifilenames); //화면 전환 애니메이션 
		Feature::DrawHint();//힌트 렌더링
		DrawIngInfo(); //현재 정보
		render::DrawText(90, 60, util::IntToChar(StageTimer), RGB(253, 208, 0), 100); //타이머(애니메이션전환)
	}
	void End()//엔딩
	{
		render::DrawBackGround("source//WIN.bmp", 2390, 1162, 0, 0, false);
		Feature::DrawRanking();
		DrawIngInfo();
	}
	void DrawStartInfo() {

		render::DrawText(1200 - (util::CheckSize(names) * 32), 200, names, RGB(253, 208, 0), 100);
		render::DrawText(850, 300,"틀린그림찾기", RGB(253, 255, 255), 150);
		render::DrawText(2240 - (util::CheckSize(names) * 10), 680, names, RGB(253, 208, 0), 30);
	}
	void DrawIngInfo() {

		char* scores;
		scores = util::IntToChar(Score);
		
		render::DrawText(145 - (util::CheckSize(names)), 40, names, RGB(253, 208, 0), 30);
		render::DrawText(90, 60, scores, RGB(253, 208, 0), 100);
	}
	void StartInput(input::MouseState mouse) {
		if ((mouse.x > 820 && mouse.x < 1550) && (mouse.y > 600 && mouse.y < 800))  //시작버튼
		{
			for (int i = 0; i < 5; i++) 
				screen::imagenum[i] = rand() % 19 + 1;
			StageTimer = 0;
			Stage = 0; //현재 스테이지
			StageScore = 0;//스테이지 스코어(5개 맞추기)
			Score = 0; //전체 정답 개수
			char* names = new char[10];
			changeCheck = false;
			Feature::InitFeature();
			state = 1;
			Feature::StageInit();
			Feature::SetPos(imagenum[Stage]);
		}
		//else if()//수인의 역사,,
	}
	void IngInput(input::MouseState mouse) {
		mousex = mouse.x, mousey = mouse.y;
		if ((mouse.x > 640 && mouse.x < 930) && (mouse.y > 980 && mouse.y < 1030))// 힌트보기
			Feature::SetHint();
		else if (Feature::CheckCorrect(mouse.x, mouse.y)) { //정답 체크
			Score++;
			StageScore++;
			if (StageScore == 5) {
				Stage++;
				StageScore = 0;
				changeCheck = true;
				Feature::SetAnimation(true);
			}
		}
	}
	void EndInput(input::MouseState mouse) {
		state = 0;
		/*
		//나가기
		if (mouse.left && (mouse.x > 0 && mouse.x < 1024) && (mouse.y > 0&& mouse.y < 768) && state == 0)  //시작버튼
		{
			state = 0;
		}
		//다시하기(1로)
		if (mouse.left && (mouse.x > 1024 - 400 && mouse.x < 1024 - 200) && (mouse.y > 768 - 400 && mouse.y < 768 - 200) && state == 0)  //시작버튼
		{
			state = 0;
		}
		*/
	}
} 