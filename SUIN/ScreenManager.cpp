#include "ScreenManager.h"
#include "SUIN.h"
#include "RenderSystem.h"
#include "MainFeature.h"
#include "InputSystem.h"
#include "Music.h"
#include "GameManager.h"
#include "util.h"
#include <random>

namespace screen
{
	 int state = 0; //0:타이틀, 1: 게임진행 2: 종료
	 int clickLimit = 300, clickTimer = clickLimit; //클릭시간 제한
	 int keyLimit = 90, keyTimer = keyLimit; //키보드입력시간 제한
	 int stageLimit = 60000, stageTimer = 0; //스테이지 시간제한
	 int stage = 0; //현재 스테이지
	 int stageScore = 0;//스테이지 스코어(5개 맞추기)
	 int score = 0; //전체 정답 개수
	 int imageNum[5] = { 0,0,0,0,0 };
	 int mousex, mousey;
	 char* names = new char[11];
	 
	void StageNect() 
	{
		 stage++;
		 stageScore = 0;
		 stageTimer = 0;
		 Feature::SetPos(imageNum[stage]);
	}

	void TimesUpdate(ULONGLONG delta) 
	{
		keyTimer += delta;
		clickTimer += delta;
		stageTimer += delta;
	}

	void Title()//타이틀
	{
		render::DrawBackGround("source//START.bmp", 2390, 1162,0,0,false);
		if(util::CheckSize(names)==0)
			render::DrawText(970, 200, "이름을 입력하세요.", RGB(255, 255, 255), 70);
		else
			render::DrawText(1200 - (util::CheckSize(names) * 32), 200, names, RGB(253, 208, 0), 100);
		render::DrawText(850, 300, "틀린그림찾기", RGB(253, 255, 255), 150);
		Feature::DrawRankingInfo(names, score);	//정보그리기
	}

	void Ing()	//게임 진행
	{
		render::DrawBackGround("source//ING.bmp", 2390, 1162, 0, 0, false);
		if (stage == 5) //게임종료
		{
			Music::soundManager->StopMusic(Music::eSoundChannel::Effect);
			Music::soundManager->PlayMusic(Music::eSoundList::win, Music::eSoundChannel::BGM);
			Feature::Ranking(names, score); //랭킹 갱신
			state = 2;
		}

		//사용될 이미지 문자열 지정
		const char* preImg = util::IntToChar(imageNum[stage - 1]);
		const char* curImg = util::IntToChar(imageNum[stage]);
		const char* part1[] = { "source//", preImg,".bmp" };
		const char* part2[] = { "source//", preImg, "_", preImg , ".bmp" };
		const char* part3[] = { "source//",curImg, ".bmp" };
		const char* part4[] = { "source//", curImg, "_", curImg , ".bmp" };
		char* filename = util::SumChar(part1,3);
		char* filenames = util::SumChar(part2,5);
		char* anifilename = util::SumChar(part3,3);
		char* anifilenames = util::SumChar(part4,5);

		//화면 전환에 따른 처리
		if (Feature::GetAnimation()) { 
			//애니메이션 진행중일경우, 이전사진 그리고 타이머 돌아가지 않음
			render::DrawBackGround(filename, 855, 930, 330, 40, false);
			render::DrawBackGround(filenames, 855, 930, 1200, 40, false);
			stageTimer = 0;
		}
		else {
			if (stageTimer > stageLimit) {//타임아웃체크
				Music::soundManager->PlayMusic(Music::eSoundList::fail, Music::eSoundChannel::Effect);
				Feature::SetAnimation(false); //실패. 애니메이션
				StageNect();
			}
			render::DrawBackGround(anifilename, 855, 930, 330, 40, false);
			render::DrawBackGround(anifilenames, 855, 930, 1200, 40, false);
		}

		Feature::DrawCorrect(); //정답
		Feature::DrawHint();//힌트
		render::DrawBackGround("source//time.bmp", 860 - (stageTimer * 0.0143), 30,970 + (stageTimer * 0.0143), 985, false); //970~1830=860
		//render::DrawText(90, 60, util::IntToChar(StageTimer), RGB(253, 208, 0), 100); //타이머(애니메이션전환)
		Feature::DrawRankingInfo(names, score);	//현재내정보
		Feature::DrawAnimation(anifilename, anifilenames); //화면 전환 애니메이션 
		if(Feature::GetStartAnimation())Feature::DrawStartAnimation(); //시작화면 애니메이션
	}

	void End()//엔딩
	{
		render::DrawBackGround("source//WIN.bmp", 2390, 1162, 0, 0, false);
		Feature::DrawRanking();//전체랭킹표
		Feature::DrawRankingInfo(names, score);//정보그리기
	}

	void StartInput(input::MouseState mouse) 
	{
		if ((mouse.x > 820 && mouse.x < 1550) && (mouse.y > 600 && mouse.y < 800))  //시작버튼
		{
			Music::soundManager->PlayMusic(Music::eSoundList::start, Music::eSoundChannel::Effect); //음악
			//스테이지별 이미지 초기화
			std::random_device rd;
			std::mt19937 gen(rd());  // Mersenne Twister 알고리즘을 사용한 랜덤 숫자 생성기
			std::uniform_int_distribution<> dis(1, 19);  // 1부터 100까지의 정수
			int randomNumber = dis(gen);
			for (int i = 0; i < 5; i++) {
				int num;
				do {
					num = dis(gen);  // 랜덤값 생성
				} while (std::find(screen::imageNum, screen::imageNum + i, num) != screen::imageNum + i);  // 중복 체크
				screen::imageNum[i] = num;  // 중복되지 않은 값 저장
			}
			//초기화
			score = 0; 
			StageNect();
			Feature::SetStartAnimation();
			Feature::InitFeature();
			state = 1;
		}
		else if ((mouse.x > 2100 && mouse.x < 2400) && (mouse.y > 970 && mouse.y < 1200)) { //나가기
			PostQuitMessage(0);
		}
		//else if()추가페이지고려
	}

	void IngInput(input::MouseState mouse) 
	{
		mousex = mouse.x, mousey = mouse.y;
		if ((mouse.x > 640 && mouse.x < 930) && (mouse.y > 980 && mouse.y < 1030))// 힌트보기
			Feature::SetHint();
		else if ((mouse.x > 2100 && mouse.x < 2400) && (mouse.y > 970 && mouse.y < 1200)) { //나가기
			PostQuitMessage(0);
		}
		else if ((mouse.x > 320 && mouse.x < 2200) && (mouse.y > 30 && mouse.y < 970)) {
			if (Feature::CheckCorrect(mouse.x, mouse.y)) { //정답 체크
				score++;
				stageScore++;
				if (stageScore == 5) {
					Music::soundManager->PlayMusic(Music::eSoundList::success, Music::eSoundChannel::Effect);
					StageNect();
					Feature::SetAnimation(true);
				}
			}
			else { //틀린경우
				stageTimer += 1000; //시간 제한
				clickTimer -= 1000; //클릭 제한
			}
		}
	}

	void EndInput(input::MouseState mouse) 
	{
		if ((mouse.x > 2100 && mouse.x < 2400) && (mouse.y > 970 && mouse.y < 1200)) { //나가기
			PostQuitMessage(0);
		}
		stage = -1;
		state = 0;
		Music::soundManager->PlayMusic(Music::eSoundList::StartBGM, Music::eSoundChannel::BGM);
	}
} 