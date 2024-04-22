#include "SUIN.h"
#include "TimeSystem.h"
#include "RenderSystem.h"
#include "InputSystem.h" 
#include "GameManager.h"
#include "Music.h"
#include "MainFeature.h"
#include "ScreenManager.h"
#include <string>
#include <cstdlib>

#define SCREEN_WIDTH 2400
#define SCREEN_HEIGHT 1200

namespace game
{
	Music::SoundManager* soundManager = Music::SoundManager::GetInstance();
	void UpdatePlayer()
	{
		//키보드 이벤트
		if (screen::state == 0 && screen::KeyTimer > screen::KeyLimit) {
			screen::KeyTimer = 0;
			if (GetAsyncKeyState(VK_BACK) & 0x8000 && !screen::names.empty())
				screen::names.pop_back();
			else if(screen::names.length()<10)
				screen::names += input::GetPressedKey();
		}
		//클릭 이벤트
		const input::MouseState& mouse = input::GetMouseState();
		const input::MouseState& prevmouse = input::GetPrevMouseState();
			if (mouse.left && screen::ClickTimer > screen::ClickLimit) {
				screen::ClickTimer = 0;
				if (screen::state == 0) 
					screen::StartInput(mouse);
				else if (screen::state == 1) 
					screen::IngInput(mouse);
				else 
					screen::EndInput(mouse);
			}
	}
	GameManager* GameManager::instance = nullptr;
	GameManager::GameManager(){}
	GameManager::~GameManager(){}
	void GameManager::Initialize()
	{
		Feature::FeatureInit();
		Feature::SetPos(screen::imagenum[0]);
		input::InitInput();
		ts::InitTime();
		render::InitRender();
		// SoundManager를 사용하여 음악 로드
		soundManager->LoadMusic(Music::eSoundList::StartBGM1, false, "source/sound/bgm1.mp3");
		soundManager->LoadMusic(Music::eSoundList::StartBGM2, false, "source/sound/bgm2.mp3");
		soundManager->LoadMusic(Music::eSoundList::StartBGM3, false, "source/sound/bgm3.mp3");
		// SoundManager 초기화
		soundManager->SetVolume(0.5f);
	}
	void GameManager::Update()
	{
		ULONGLONG delta = ts::GetDeltaTime();
		++m_UpdateCount;
		input::UpdateMouse();
		ts::UpdateTime();
		UpdatePlayer();
		input::ResetInput();
		screen::TimesUpdate(delta);
	}
	void GameManager::Render()
	{
		render::BeginDraw();
		if (screen::state == 0)
			screen::Title();
		else if (screen::state == 1)
			screen::Ing();
		else
			screen::End();
		render::EndDraw();
	}
	void GameManager::Finalize()
	{
		render::ReleaseRender();
		Music::SoundManager::DestroyInstance();// SoundManager 해제
	}
	void GameManager::Run()
	{
		MSG msg;
		soundManager->PlayMusic(Music::eSoundList::StartBGM2, Music::eSoundChannel::BGM);// SoundManager를 사용하여 음악 재생
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
			}
		}
		soundManager->StopMusic(Music::eSoundChannel::BGM);// SoundManager를 사용하여 음악 중지 및 해제
	}
	GameManager* GameManager::GetInstance()
	{
		if (instance == nullptr)
			instance = new GameManager();
		return instance;
	}
	void GameManager::DestroyInstance()
	{
		if (instance != nullptr){
			delete instance;
			instance = nullptr;
		}
	}
}