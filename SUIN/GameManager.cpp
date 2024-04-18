#include "SUIN.h"
#include "TimeSystem.h"
#include "RenderSystem.h"
#include "InputSystem.h" 
#include "GameManager.h"
#include "Music.h"
#include "MainFeature.h"
#include <string>
#include <cstdlib>

#define SCREEN_WIDTH 2400
#define SCREEN_HEIGHT 1200

namespace game
{
	HBITMAP hBackmap = nullptr;
	int state = 0; //0:타이틀, 1: 게임진행 2: 종료
	int ClickLimit=300, ClickTimer = ClickLimit; //클릭시간 제한
	int KeyLimit = 90, KeyTimer = KeyLimit; //키보드입력시간 제한
	int StageLimit = 6000, StageTimer = StageLimit; //스테이지 시간제한
	int Stage = 1; //현재 스테이지
	int StageScore = 0;//스테이지 스코어(5개 맞추기)
	int Score = 0; //정답 개수
	int image = 0;
	int mousex, mousey;
	std::string names="ME";

	Music* bgm = new Music("source/sound/bg.mp3", false);
	void StartInput(input::MouseState mouse) {
		if ((mouse.x > 820 && mouse.x < 1550)&&(mouse.y > 600 && mouse.y < 800))  //시작버튼
		{
			state = 1;
		}
		//else if()//수인의 역사,,
	}
	void IngInput(input::MouseState mouse) {
		mousex = mouse.x, mousey = mouse.y;
		if (Feature::CheckCorrect(mouse.x, mouse.y)) {
			Score++;
			StageScore++;
			if (StageScore == 5) {
				image = 1 + std::rand() % 19;
				StageScore = 0;
				Feature::StageInit();
				Feature::SetPos(image);
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

	void UpdatePlayer()
	{
		//키보드 이벤트
		if (state == 0 && KeyTimer > KeyLimit) {
			KeyTimer = 0;
			if (GetAsyncKeyState(VK_BACK) & 0x8000 && !names.empty())
				names.pop_back();
			else if(names.length()<10)
				names += input::GetPressedKey();
		}
		//클릭 이벤트
		const input::MouseState& mouse = input::GetMouseState();
		const input::MouseState& prevmouse = input::GetPrevMouseState();
			if (mouse.left && ClickTimer > ClickLimit) {
				ClickTimer = 0;
				if (state == 0) 
					StartInput(mouse);
				else if (state == 1) 
					IngInput(mouse);
				else 
					EndInput(mouse);
			}
	}
	GameManager* GameManager::instance = nullptr;
	GameManager::GameManager()
	{
	}
	GameManager::~GameManager()
	{
	}
	void GameManager::Initialize()
	{
		image = 1 + std::rand() % 19;
		Feature::SetPos(image);
		input::InitInput();
		time::InitTime();
		render::InitRender();
		Music::Init();
	}

	void GameManager::Update()
	{
		ULONGLONG delta = time::GetDeltaTime();
		++m_UpdateCount;
		input::UpdateMouse();
		time::UpdateTime();
		UpdatePlayer();
		input::ResetInput();
		KeyTimer += delta;
		ClickTimer += delta;
		StageTimer += delta;
	}

	void GameManager::Render()
	{
		render::BeginDraw();
		if (state == 0)
			Title();
		else if (state == 1)
			Ing();
		else
			End();
		render::EndDraw();
	}
	void GameManager::Title()//타이틀
	{
		hBackmap= render::DrawBackGround("source/START.bmp", 2390, 1162,0,0,false);
		DrawStartInfo();
	}
	void GameManager::Ing()	//게임 진행
	{
		hBackmap = render::DrawBackGround("source/ING.bmp", 2390, 1162, 0,0, false);
		std::string filename = "source/" + std::to_string(image) + ".bmp";
		std::string filenames = "source/" + std::to_string(image) + "_"+ std::to_string(image) +".bmp";
		hBackmap = render::DrawBackGround(filename.c_str() , 855, 930, 330, 40, false);
		hBackmap = render::DrawBackGround(filenames.c_str() , 855, 930, 1200, 40, false);
		Feature::DrawCorrect();
		DrawIngInfo();
	}
	void GameManager::End()//엔딩
	{
		hBackmap = render::DrawBackGround("source/WIN.bmp", 2390, 1162 ,0,0, false);
		DrawIngInfo();
	}
	void GameManager::Finalize()
	{
		render::ReleaseRender();
	}
	void GameManager::Run()
	{
		MSG msg;
		bgm->play();
		while (true)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;
				if (msg.message == WM_KEYDOWN)
					input::KeyDown(msg.wParam);
				else if (msg.message == WM_KEYUP)
					input::KeyUp(msg.wParam);
				else
					DispatchMessage(&msg);
			}
			else
			{
				Update();
				Render();
				bgm->Update();
			}
		}
		delete bgm;
		Music::Release();
	}

	GameManager* GameManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new GameManager();
		}
		return instance;
	}
	void GameManager::DestroyInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	void GameManager::DrawStartInfo() {
		render::DrawText(1200-(names.length() * 32), 200, names.c_str(), RGB(253, 208, 0), 100);
		render::DrawText(850, 300,"틀린그림찾기", RGB(253, 255, 255), 150);

		render::DrawText(2240 - (names.length() * 10), 680, names.c_str(), RGB(253, 208, 0), 30);
	}
	void GameManager::DrawIngInfo(){
		std::string scores;
		if (Score < 10)
			scores = "0"+std::to_string(Score);
		else
			scores = std::to_string(Score);

		render::DrawText(145- (names.length() * 10), 40, names.c_str(), RGB(253, 208, 0), 30);
		render::DrawText(90, 60, scores.c_str(), RGB(253, 208, 0),100);
	}


}