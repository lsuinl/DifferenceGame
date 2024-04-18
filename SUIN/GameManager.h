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

		void Title();
		void Ing();
		void End();
		static GameManager* GetInstance();

		static void DestroyInstance();

	private:
		void DrawStartInfo();
		void DrawIngInfo();
		void DrawSomething();
		void StartInput();
		void IngInput();
		void EndInput();
		static GameManager* instance;

		int m_UpdateCount = { 0 };
		int m_FixedUpdateCount = { 0 };
	};
}
