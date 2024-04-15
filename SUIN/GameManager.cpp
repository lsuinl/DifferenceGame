#include "SUIN.h"
#include "TimeSystem.h"
#include "RenderSystem.h"
#include "InputSystem.h"

#include "GameManager.h"

#include <string>

namespace game
{
	struct Object
	{
		float x;
		float y;
		float size;
		float speed;

		COLORREF color;

		void SetPos(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		void Move(float x, float y)
		{
			this->x += x;
			this->y += y;
		}
	};

	Object player = { global::GetWinApp().GetWidth() / 2 ,global::GetWinApp().GetHeight() / 2, 10, 10, RGB(255, 255, 0) };

	const int bludeCircleMax = 5000;
	int blueCircleCount = 0;
	Object blueCircles[bludeCircleMax];
	HBITMAP hBackmap = nullptr;

	void UpdatePlayer()
	{
		// 게임 로직은 여기에 추가
		if (input::IsKeyDown('A'))
		{
			player.Move(-player.speed, 0);
		}
		else if (input::IsKeyDown('D'))
		{
			player.Move(player.speed, 0);
		}
		if (input::IsKeyDown('W'))
		{
			player.Move(0, -player.speed);
		}
		else if (input::IsKeyDown('S'))
		{
			player.Move(0, player.speed);
		}
	}

	void UpdateBlueCircle()
	{
			const input::MouseState& mouse = input::GetMouseState();
			const input::MouseState& prevmouse = input::GetPrevMouseState();

			if (input::IsSame(mouse, prevmouse))
			{
				return;
			}

			if (blueCircleCount < bludeCircleMax && mouse.left)
			{
				blueCircles[blueCircleCount].SetPos(mouse.x, mouse.y);
				blueCircles[blueCircleCount].color = RGB(0, 0, 255);
				blueCircles[blueCircleCount].size = 10;
				blueCircles[blueCircleCount].speed = 0;
				blueCircleCount++;
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
	}

	void GameManager::Update()
	{
		++m_UpdateCount;

		input::UpdateMouse();

		time::UpdateTime();

		UpdatePlayer();
		UpdateBlueCircle();

		input::ResetInput();

	}

	void GameManager::FixeUpdate()
	{
		static ULONGLONG elapsedTime;

		elapsedTime += time::GetDeltaTime();

		while (elapsedTime >= 20) //0.02초
		{
			++m_FixedUpdateCount;

			elapsedTime -= 20;
		}
	}

	void GameManager::Render()
	{
		render::BeginDraw();
		DrawFPS();
		DrawSomething();
		DrawPlayer();
		DrawBackGround();
		render::EndDraw();
	}
	void GameManager::Finalize()
	{
		render::ReleaseRender();
	}
	void GameManager::Run()
	{
		MSG msg;

		while (true)
		{
			//GetMessage 는 큐에 메시지가 있을 때까지 대기, 블러킹
			//PeekMessage 는 메시지가 있으면 처리하고 아니면 넌블러킹
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
				FixeUpdate();

				Update();

				Render();
			}
		}
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

	void GameManager::DrawFPS()
	{
		static ULONGLONG elapsedTime;
		static int UpdateCount;
		static int FixedUpdateCount;

		elapsedTime += time::GetDeltaTime();

		if (elapsedTime >= 1000)
		{
			elapsedTime = 0;
			;
			UpdateCount = m_UpdateCount;
			FixedUpdateCount = m_FixedUpdateCount;

			m_UpdateCount = 0;
			m_FixedUpdateCount = 0;
		}

		std::string str = "FPS: " + std::to_string(time::GetFrameRate());
		str += "           Update " + std::to_string(UpdateCount);
		str += "           FixedUpdate " + std::to_string(FixedUpdateCount);

		render::DrawText(10, 10, str.c_str(), RGB(255, 0, 0));
	}

	void GameManager::DrawPlayer()
	{
		render::DrawCircle(player.x, player.y, player.size, player.color);
	}

	void GameManager::DrawBackGround()
	{
		hBackmap = render::LoadImages("bitmap10.bmp");  // 함수 이름 수정
		render::DrawBitmap(0, 0, hBackmap);
		render::ReleaseImage(hBackmap);  // 이미지 메모리 해제
	}

	void GameManager::DrawSomething()
	{

			for (int i = 0; i < blueCircleCount; i++)
			{
				render::DrawCircle(blueCircles[i].x, blueCircles[i].y, blueCircles[i].size, blueCircles[i].color);
			}

			render::DrawLine(player.x - 50, player.y + 50, player.x + 50, player.y + 50, RGB(255, 0, 0));
			render::DrawRect(player.x - 25, player.y - 25, 50, 50, RGB(255, 0, 255));

	}
}