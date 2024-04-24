#include "ScreenManager.h"
#include "SUIN.h"
#include "RenderSystem.h"
#include "MainFeature.h"
#include "InputSystem.h"
#include "Music.h"
#include "GameManager.h"
#include "util.h"
#include <random>
namespace screen
{
	 int state = 2; //0:Ÿ��Ʋ, 1: �������� 2: ����
	 int ClickLimit = 300, ClickTimer = ClickLimit; //Ŭ���ð� ����
	 int KeyLimit = 90, KeyTimer = KeyLimit; //Ű�����Է½ð� ����
	 int StageLimit = 50000, StageTimer = 0; //�������� �ð�����
	 int Stage = 0; //���� ��������
	 int StageScore = 0;//�������� ���ھ�(5�� ���߱�)
	 int Score = 0; //��ü ���� ����
	 int imagenum[5] = { 0,0,0,0,0 };
	 int mousex, mousey;
	 char* names = new char[11];
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
		render::DrawBackGround("source//START.bmp", 2390, 1162,0,0,false);
		DrawStartInfo();
	}
	void Ing()	//���� ����
	{
		if (Stage == 5) {//2��������(0,1)
			Music::soundManager->StopMusic(Music::eSoundChannel::Effect);
			Music::soundManager->PlayMusic(Music::eSoundList::win, Music::eSoundChannel::BGM);
			state = 2;
			Feature::Ranking(names, Score); //��ŷ ����
		}
		render::DrawBackGround("source//ING.bmp", 2390, 1162, 0, 0, false);
		const char* preImg = util::IntToChar(imagenum[Stage - 1]);
		const char* curImg = util::IntToChar(imagenum[Stage]);
		const char* part1[] = { "source//", preImg,".bmp" };
		const char* part2[] = { "source//", preImg, "_", preImg , ".bmp" };
		const char* part3[] = { "source//",curImg, ".bmp" };
		const char* part4[] = { "source//", curImg, "_", curImg , ".bmp" };
		char* filename = util::SumChar(part1,3);
		char* filenames = util::SumChar(part2,5);
		char* anifilename = util::SumChar(part3,3);
		char* anifilenames = util::SumChar(part4,5);
		//ȭ�� ��ȯ�� ���� ������
		if (Feature::GetAnimation()) {
			render::DrawBackGround(filename, 855, 930, 330, 40, false);
			render::DrawBackGround(filenames, 855, 930, 1200, 40, false);
			StageTimer = 0;
		}
		else {
			//Ÿ�Ӿƿ�üũ
			if (StageTimer > StageLimit) {
				Music::soundManager->PlayMusic(Music::eSoundList::fail, Music::eSoundChannel::Effect);
				Stage++;
				StageScore = 0;
				changeCheck = true;
				Feature::SetAnimation(false);
			}
			if (changeCheck == true) {
				StageNect();
				changeCheck = false;
			}
			render::DrawBackGround(anifilename, 855, 930, 330, 40, false);
			render::DrawBackGround(anifilenames, 855, 930, 1200, 40, false);
		}
		
		Feature::DrawCorrect(); //���� ������
		Feature::DrawAnimation(anifilename, anifilenames); //ȭ�� ��ȯ �ִϸ��̼� 
		Feature::DrawHint();//��Ʈ ������
		DrawIngInfo(); //���� ����
		Feature::DrawRankingInfo();
		render::DrawText(90, 60, util::IntToChar(StageTimer), RGB(253, 208, 0), 100); //Ÿ�̸�(�ִϸ��̼���ȯ)
		if(Feature::GetStartAnimation())Feature::DrawStartAnimation();
	}
	void End()//����
	{
		render::DrawBackGround("source//WIN.bmp", 2390, 1162, 0, 0, false);
		Feature::DrawRanking();
		Feature::DrawRankingInfo();
		//DrawIngInfo();
	}
	void DrawStartInfo() {
		render::DrawText(1200 - (util::CheckSize(names) * 32), 200, names, RGB(253, 208, 0), 100);
		render::DrawText(850, 300,"Ʋ���׸�ã��", RGB(253, 255, 255), 150);
		render::DrawText(2240 - (util::CheckSize(names) * 10), 680, names, RGB(253, 208, 0), 30);
		Feature::DrawRankingInfo();
	}
	void DrawIngInfo() {
		char* scores;
		scores = util::IntToChar(Score);
		render::DrawText(145 - (util::CheckSize(names)), 40, names, RGB(253, 208, 0), 30);
		render::DrawText(90, 60, scores, RGB(253, 208, 0), 100);
	}
	void StartInput(input::MouseState mouse) {
		if ((mouse.x > 820 && mouse.x < 1550) && (mouse.y > 600 && mouse.y < 800))  //���۹�ư
		{
			Music::soundManager->PlayMusic(Music::eSoundList::start, Music::eSoundChannel::Effect);
			std::random_device rd;
			std::mt19937 gen(rd());  // Mersenne Twister �˰����� ����� ���� ���� ������
			std::uniform_int_distribution<> dis(1, 19);  // 1���� 100������ ����
			int randomNumber = dis(gen);

			for (int i = 0; i < 5; i++) 
				screen::imagenum[i] = dis(gen);
			
			StageTimer = 0;
			Stage = 0; //���� ��������
			StageScore = 0;//�������� ���ھ�(5�� ���߱�)
			Score = 0; //��ü ���� ����
			char* names = new char[10];
			changeCheck = false;
			Feature::SetStartAnimation();
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
				Music::soundManager->PlayMusic(Music::eSoundList::success, Music::eSoundChannel::Effect);
				Stage++;
				StageScore = 0;
				changeCheck = true;
				Feature::SetAnimation(true);
			}
		}
		else {
			StageTimer += 2000; //�ð� ����
			ClickTimer -= 2000; //Ŭ�� ����
		}
	}
	void EndInput(input::MouseState mouse) {
		state = 0;
		Music::soundManager->PlayMusic(Music::eSoundList::StartBGM, Music::eSoundChannel::BGM);
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