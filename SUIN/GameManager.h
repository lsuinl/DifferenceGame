#pragma once
#include "SUIN.h"
namespace game
{
	class GameManager
	{
	public:
		GameManager();
		~GameManager();
		void Initialize();
		void Update();
		void Render();
		void Finalize();
		void Run();
		static GameManager* GetInstance();
		static void DestroyInstance();
		static GameManager* instance;
		int m_UpdateCount = { 0 };
		int m_FixedUpdateCount = { 0 };
	};
}
