#include "SUIN.h"
#include "TimeSystem.h"
#include "RenderSystem.h"
#include "InputSystem.h" 
#include "GameManager.h"
#include "Music.h"
#include "MainFeature.h"
#include "ScreenManager.h"
#include <cstdlib>
#include "util.h"
#define SCREEN_WIDTH 2400
#define SCREEN_HEIGHT 1200

namespace game
{
	void UpdatePlayer()
	{
		//Ű���� �̺�Ʈ ù ������������ �ۿ�. 
		if (screen::state == 0 && screen::keyTimer > screen::keyLimit) {
			screen::keyTimer = 0;
			//���鹮��==�ڷΰ���
			if (GetAsyncKeyState(VK_BACK) & 0x8000 && screen::names[0] != '\0')
				screen::names[util::CheckSize(screen::names)-1] = '\0';
			//��������+�Է��� ���� �Է�
			else if (util::CheckSize(screen::names) < 10) {
				const char* plus[] = {screen::names, input::GetPressedKey()};
				screen::names = util::SumChar(plus, 2);
			}
		}
		//Ŭ�� �̺�Ʈ
		const input::MouseState& mouse = input::GetMouseState();
		const input::MouseState& prevmouse = input::GetPrevMouseState();
			if (mouse.left && screen::clickTimer > screen::clickLimit) {
				screen::clickTimer = 0;
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
		render::InitRender();
		input::InitInput();
		ts::InitTime();
		for (int i = 0; i < 11; i++) screen::names[i] = '\0';	//�̸��ʱ�ȭ
		Music::soundManager->GetInstance(); //����Ŵ��� ����
		Music::soundManager->LoadMusic(Music::eSoundList::StartBGM, true, "source//sound//bgm3.mp3");//�������
		//Music::soundManager->LoadMusic(Music::eSoundList::StartBGM2, false, "source//sound//bgm2.mp3");
		//Music::soundManager->LoadMusic(Music::eSoundList::StartBGM3, false, "source//sound//bgm3.mp3");
		Music::soundManager->LoadMusic(Music::eSoundList::correct, false, "source//sound//correct3.mp3");//����ȿ����

		Music::soundManager->LoadMusic(Music::eSoundList::wrong, false, "source//sound//wrong.mp3");//����ȿ����
		Music::soundManager->LoadMusic(Music::eSoundList::hint, false, "source//sound//hint.mp3");//��Ʈ ȿ����
		Music::soundManager->LoadMusic(Music::eSoundList::win, false, "source//sound//Win.mp3");//����ȿ����
		Music::soundManager->LoadMusic(Music::eSoundList::start, false, "source//sound//start.mp3");//���� ȿ����
		Music::soundManager->LoadMusic(Music::eSoundList::success, false, "source//sound//success.mp3");//�������� ���� ȿ����
		Music::soundManager->LoadMusic(Music::eSoundList::fail, false, "source//sound//fail.mp3");//����

		Music::soundManager->SetVolume(0.5f); //ũ������
	}
	void GameManager::Update()
	{
		ULONGLONG delta = ts::GetDeltaTime();
		ts::UpdateTime();

		input::UpdateMouse();
		input::ResetInput();
		
		UpdatePlayer();
		
		screen::TimesUpdate(delta);
	}
	void GameManager::Render()
	{
		render::BeginDraw();
		//������ �ѹ��� ���� ȭ�� ������
		if (screen::state == 0) screen::Title();
		else if (screen::state == 1) screen::Ing();
		else screen::End();
		render::EndDraw();
	}
	void GameManager::Finalize()
	{
		render::ReleaseRender();
		Music::SoundManager::DestroyInstance();// ����Ŵ��� ����
	}
	void GameManager::Run()
	{
		MSG msg;
		Music::soundManager->PlayMusic(Music::eSoundList::StartBGM, Music::eSoundChannel::BGM);//���� ���
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