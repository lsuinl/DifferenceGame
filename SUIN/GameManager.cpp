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
	int state = 1; //0:Ÿ��Ʋ, 1: �������� 2: ����
	int ClickLimit=300, ClickTimer = ClickLimit; //Ŭ���ð� ����
	int StageLimit = 6000, StageTimer = StageLimit; //�������� �ð�����
	int Hint = 5; //��Ʈ ����
	int Stage = 0; //���� ��������
	int Score = 0; //���� ����

	Music* bgm = new Music("C:\\bgm1.mp3", false);
	void StartInput(input::MouseState mouse) {
		if ((mouse.x > 820 && mouse.x < 1550)&&(mouse.y > 600 && mouse.y < 800))  //���۹�ư
		{
			state = 1;
		}
		//else if()//������ ����,,
	}
	void IngInput() {

	}
	void EndInput(input::MouseState mouse) {
		state = 0;
		/*
		//������
		if (mouse.left && (mouse.x > 0 && mouse.x < 1024) && (mouse.y > 0&& mouse.y < 768) && state == 0)  //���۹�ư
		{
			state = 0;
		}
		//�ٽ��ϱ�(1��)
		if (mouse.left && (mouse.x > 1024 - 400 && mouse.x < 1024 - 200) && (mouse.y > 768 - 400 && mouse.y < 768 - 200) && state == 0)  //���۹�ư
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
		//Ÿ��Ʋ
		DrawBackGround("source/START.bmp", 2390, 1162,0,0);
		DrawInfo();

	}
	void GameManager::Ing()
	{
		//���� ����
		DrawBackGround("source/ING.bmp", 2390, 1162, 0,0);
		//���� ����
		DrawBackGround("source/7.bmp", 855, 930, 330, 40);
		DrawBackGround("source/7_7.bmp", 855, 930, 1200, 40);
		DrawInfo();
	}
	void GameManager::End()
	{
		//����
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