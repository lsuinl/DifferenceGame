#include "SUIN.h"
#include "TimeSystem.h"
#include "RenderSystem.h"
#include "InputSystem.h" 
#include "GameManager.h"
#include "Music.h"
#include "MainFeature.h"
#include <string>


#define SCREEN_WIDTH 2400
#define SCREEN_HEIGHT 1200

namespace game
{
	HBITMAP hBackmap = nullptr;
	int state = 1; //0:타이틀, 1: 게임진행 2: 종료
	int ClickLimit=300, ClickTimer = ClickLimit; //클릭시간 제한
	int StageLimit = 6000, StageTimer = StageLimit; //스테이지 시간제한
	int Hint = 5; //힌트 개수
	int Stage = 0; //현재 스테이지
	int Score = 0; //정답 개수

	Music* bgm = new Music("C:\\bgm1.mp3", false);
	void StartInput(input::MouseState mouse) {
		if ((mouse.x > 820 && mouse.x < 1550)&&(mouse.y > 600 && mouse.y < 800))  //시작버튼
		{
			state = 1;
		}
		//else if()//수인의 역사,,
	}
	void IngInput() {

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
		const input::MouseState& mouse = input::GetMouseState();
		const input::MouseState& prevmouse = input::GetPrevMouseState();
			if (mouse.left && ClickTimer > ClickLimit) {
				ClickTimer = 0;
				if (state == 0) 
					StartInput(mouse);
				else if (state == 1) 
					IngInput();
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
		input::InitInput();
		time::InitTime();
		render::InitRender();
		Music::Init();
	}

	void GameManager::Update()
	{
		++m_UpdateCount;

		input::UpdateMouse();

		time::UpdateTime();

		UpdatePlayer();
		input::ResetInput();
		ClickTimer += time::GetDeltaTime();
		StageTimer += time::GetDeltaTime();

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
	void GameManager::Title()
	{
		//타이틀
		DrawBackGround("source/START.bmp", 2390, 1162,0,0);
		DrawInfo();

	}
	void GameManager::Ing()
	{
		//게임 진행
		DrawBackGround("source/ING.bmp", 2390, 1162, 0,0);
		//사진 띄우기
		DrawBackGround("source/7.bmp", 855, 930, 330, 40);
		DrawBackGround("source/7_7.bmp", 855, 930, 1200, 40);
		DrawInfo();
	}
	void GameManager::End()
	{
		//엔딩
		DrawBackGround("source/WIN.bmp", 2390, 1162 ,0,0);
		DrawInfo();
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
				{
					input::KeyDown(msg.wParam);
				}
				else if (msg.message == WM_KEYUP)
				{
					input::KeyUp(msg.wParam);
				}
				else
				{
					DispatchMessage(&msg);
				}
			}
			else
			{
				//FixeUpdate();
	
				Update();

				Render();

				bgm->Update();
			}
		}
		// Free Object
		delete bgm;

		// Release Fmod Llibrary
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

	void GameManager::DrawInfo()
	{
		//render::DrawText(10, 10, str.c_str(), RGB(255, 0, 0));
	}

	void GameManager::DrawBackGround(const char* name,int width, int height, int x, int y)
	{
		hBackmap = render::LoadImages(name,width, height);
		render::DrawBitmap(x, y, hBackmap);
		render::ReleaseImage(hBackmap); 
	}

}