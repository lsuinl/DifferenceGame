#pragma once

namespace game
{
	class GameManager
	{
	public:
		GameManager();

		~GameManager();

		void Initialize();

		void Update();

		void FixeUpdate();

		void Render();

		void Finalize();

		void Run();

		static GameManager* GetInstance();

		static void DestroyInstance();

	private:
		void DrawBackGround();
		void DrawFPS();
		void DrawPlayer();
		void DrawSomething();

		static GameManager* instance;

		int m_UpdateCount = { 0 };
		int m_FixedUpdateCount = { 0 };
	};
}
