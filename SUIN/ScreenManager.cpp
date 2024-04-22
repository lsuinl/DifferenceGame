#include "ScreenManager.h"
#include "SUIN.h"
#include "RenderSystem.h"
#include <string>
#include "MainFeature.h"
#include "InputSystem.h"
#include "Music.h"

namespace screen
{
	 int state = 2; //0:Ÿ��Ʋ, 1: �������� 2: ����
	 int ClickLimit = 300, ClickTimer = ClickLimit; //Ŭ���ð� ����
	 int KeyLimit = 90, KeyTimer = KeyLimit; //Ű�����Է½ð� ����
	 int StageLimit = 5000, StageTimer = 0; //�������� �ð�����
	 int Stage = 0; //���� ��������
	 int StageScore = 0;//�������� ���ھ�(5�� ���߱�)
	 int Score = 0; //��ü ���� ����
	 int imagenum[5] = { 0,0,0,0,0 };
	 int mousex, mousey;
	 std::string names = "NAME";
	 bool changeCheck = false;

	void StageNect() {
		 StageScore = 0;
		 StageTimer = 0;
		 Feature::StageInit();
		 Feature::SetPos(imagenum[Stage]);
	}
	void TimesUpdate(ULONGLONG delta) {
		KeyTimer += delta;
		ClickTimer += delta;
		StageTimer += delta;
	}
	void Title()//Ÿ��Ʋ
	{
		render::DrawBackGround("source/START.bmp", 2390, 1162,0,0,false);
		DrawStartInfo();
	}
	void Ing()	//���� ����
	{
		if (Stage == 1) {//2��������(0,1)
			state = 2;
			Feature::Ranking(names.c_str(),Score); //��ŷ ����
		}
		render::DrawBackGround("source/ING.bmp", 2390, 1162, 0, 0, false);
		std::string filename= "source/" + std::to_string(imagenum[Stage - 1]) + ".bmp";
		std::string filenames= "source/" + std::to_string(imagenum[Stage - 1]) + "_" + std::to_string(imagenum[Stage - 1]) + ".bmp";
		std::string anifilename= "source/" + std::to_string(imagenum[Stage]) + ".bmp";
		std::string anifilenames= "source/" + std::to_string(imagenum[Stage]) + "_" + std::to_string(imagenum[Stage]) + ".bmp";
		//ȭ�� ��ȯ�� ���� ������
		if (Feature::GetAnimation()) {
			render::DrawBackGround(filename.c_str(), 855, 930, 330, 40, false);
			render::DrawBackGround(filenames.c_str(), 855, 930, 1200, 40, false);
			StageTimer = 0;
		}
		else {
			//Ÿ�Ӿƿ�üũ
			if (StageTimer > StageLimit) {
				Stage++;
				StageScore = 0;
				changeCheck = true;
				Feature::SetAnimation(false);
			}
			if (changeCheck == true) {
				StageNect();
				changeCheck = false;
			}
			render::DrawBackGround(anifilename.c_str(), 855, 930, 330, 40, false);
			render::DrawBackGround(anifilename.c_str(), 855, 930, 1200, 40, false);
		}
		
		Feature::DrawCorrect(); //���� ������
		Feature::DrawAnimation(anifilename.c_str(), anifilenames.c_str()); //ȭ�� ��ȯ �ִϸ��̼� 
		Feature::DrawHint();//��Ʈ ������
		DrawIngInfo(); //���� ����
		render::DrawText(90, 60, std::to_string(StageTimer).c_str(), RGB(253, 208, 0), 100); //Ÿ�̸�(�ִϸ��̼���ȯ)
	}
	void End()//����
	{
		render::DrawBackGround("source/WIN.bmp", 2390, 1162, 0, 0, false);
		Feature::DrawRanking();
		DrawIngInfo();
	}
	void DrawStartInfo() {
		render::DrawText(1200 - (names.length() * 32), 200, names.c_str(), RGB(253, 208, 0), 100);
		render::DrawText(850, 300, "Ʋ���׸�ã��", RGB(253, 255, 255), 150);
		render::DrawText(2240 - (names.length() * 10), 680, names.c_str(), RGB(253, 208, 0), 30);
	}
	void DrawIngInfo() {
		std::string scores;
		if (Score < 10)
			scores = "0" + std::to_string(Score);
		else
			scores = std::to_string(Score);

		render::DrawText(145 - (names.length() * 10), 40, names.c_str(), RGB(253, 208, 0), 30);
		render::DrawText(90, 60, scores.c_str(), RGB(253, 208, 0), 100);
	}
	void StartInput(input::MouseState mouse) {
		if ((mouse.x > 820 && mouse.x < 1550) && (mouse.y > 600 && mouse.y < 800))  //���۹�ư
		{
			for (int i = 0; i < 5; i++) 
				screen::imagenum[i] = rand() % 19 + 1;
			StageTimer = 0;
			Stage = 0; //���� ��������
			StageScore = 0;//�������� ���ھ�(5�� ���߱�)
			Score = 0; //��ü ���� ����
			std::string names = "NAME";
			changeCheck = false;
			Feature::InitFeature();
			state = 1;
			Feature::StageInit();
			Feature::SetPos(imagenum[Stage]);
		}
		//else if()//������ ����,,
	}
	void IngInput(input::MouseState mouse) {
		mousex = mouse.x, mousey = mouse.y;
		if ((mouse.x > 640 && mouse.x < 930) && (mouse.y > 980 && mouse.y < 1030))// ��Ʈ����
			Feature::SetHint();
		else if (Feature::CheckCorrect(mouse.x, mouse.y)) { //���� üũ
			Score++;
			StageScore++;
			if (StageScore == 5) {
				Stage++;
				StageScore = 0;
				changeCheck = true;
				Feature::SetAnimation(true);
			}
		}
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
} 