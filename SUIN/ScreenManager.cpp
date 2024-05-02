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
	 int state = 0; //0:Ÿ��Ʋ, 1: �������� 2: ����
	 int clickLimit = 300, clickTimer = clickLimit; //Ŭ���ð� ����
	 int keyLimit = 90, keyTimer = keyLimit; //Ű�����Է½ð� ����
	 int stageLimit = 60000, stageTimer = 0; //�������� �ð�����
	 int stage = 0; //���� ��������
	 int stageScore = 0;//�������� ���ھ�(5�� ���߱�)
	 int score = 0; //��ü ���� ����
	 int imageNum[5] = { 0,0,0,0,0 };
	 int mousex, mousey;
	 char* names = new char[11];
	 
	void StageNect() 
	{
		 stage++;
		 stageScore = 0;
		 stageTimer = 0;
		 Feature::SetPos(imageNum[stage]);
	}

	void TimesUpdate(ULONGLONG delta) 
	{
		keyTimer += delta;
		clickTimer += delta;
		stageTimer += delta;
	}

	void Title()//Ÿ��Ʋ
	{
		render::DrawBackGround("source//START.bmp", 2390, 1162,0,0,false);
		if(util::CheckSize(names)==0)
			render::DrawText(970, 200, "�̸��� �Է��ϼ���.", RGB(255, 255, 255), 70);
		else
			render::DrawText(1200 - (util::CheckSize(names) * 32), 200, names, RGB(253, 208, 0), 100);
		render::DrawText(850, 300, "Ʋ���׸�ã��", RGB(253, 255, 255), 150);
		Feature::DrawRankingInfo(names, score);	//�����׸���
	}

	void Ing()	//���� ����
	{
		render::DrawBackGround("source//ING.bmp", 2390, 1162, 0, 0, false);
		if (stage == 5) //��������
		{
			Music::soundManager->StopMusic(Music::eSoundChannel::Effect);
			Music::soundManager->PlayMusic(Music::eSoundList::win, Music::eSoundChannel::BGM);
			Feature::Ranking(names, score); //��ŷ ����
			state = 2;
		}

		//���� �̹��� ���ڿ� ����
		const char* preImg = util::IntToChar(imageNum[stage - 1]);
		const char* curImg = util::IntToChar(imageNum[stage]);
		const char* part1[] = { "source//", preImg,".bmp" };
		const char* part2[] = { "source//", preImg, "_", preImg , ".bmp" };
		const char* part3[] = { "source//",curImg, ".bmp" };
		const char* part4[] = { "source//", curImg, "_", curImg , ".bmp" };
		char* filename = util::SumChar(part1,3);
		char* filenames = util::SumChar(part2,5);
		char* anifilename = util::SumChar(part3,3);
		char* anifilenames = util::SumChar(part4,5);

		//ȭ�� ��ȯ�� ���� ó��
		if (Feature::GetAnimation()) { 
			//�ִϸ��̼� �������ϰ��, �������� �׸��� Ÿ�̸� ���ư��� ����
			render::DrawBackGround(filename, 855, 930, 330, 40, false);
			render::DrawBackGround(filenames, 855, 930, 1200, 40, false);
			stageTimer = 0;
		}
		else {
			if (stageTimer > stageLimit) {//Ÿ�Ӿƿ�üũ
				Music::soundManager->PlayMusic(Music::eSoundList::fail, Music::eSoundChannel::Effect);
				Feature::SetAnimation(false); //����. �ִϸ��̼�
				StageNect();
			}
			render::DrawBackGround(anifilename, 855, 930, 330, 40, false);
			render::DrawBackGround(anifilenames, 855, 930, 1200, 40, false);
		}

		Feature::DrawCorrect(); //����
		Feature::DrawHint();//��Ʈ
		render::DrawBackGround("source//time.bmp", 860 - (stageTimer * 0.0143), 30,970 + (stageTimer * 0.0143), 985, false); //970~1830=860
		//render::DrawText(90, 60, util::IntToChar(StageTimer), RGB(253, 208, 0), 100); //Ÿ�̸�(�ִϸ��̼���ȯ)
		Feature::DrawRankingInfo(names, score);	//���系����
		Feature::DrawAnimation(anifilename, anifilenames); //ȭ�� ��ȯ �ִϸ��̼� 
		if(Feature::GetStartAnimation())Feature::DrawStartAnimation(); //����ȭ�� �ִϸ��̼�
	}

	void End()//����
	{
		render::DrawBackGround("source//WIN.bmp", 2390, 1162, 0, 0, false);
		Feature::DrawRanking();//��ü��ŷǥ
		Feature::DrawRankingInfo(names, score);//�����׸���
	}

	void StartInput(input::MouseState mouse) 
	{
		if ((mouse.x > 820 && mouse.x < 1550) && (mouse.y > 600 && mouse.y < 800))  //���۹�ư
		{
			Music::soundManager->PlayMusic(Music::eSoundList::start, Music::eSoundChannel::Effect); //����
			//���������� �̹��� �ʱ�ȭ
			std::random_device rd;
			std::mt19937 gen(rd());  // Mersenne Twister �˰����� ����� ���� ���� ������
			std::uniform_int_distribution<> dis(1, 19);  // 1���� 100������ ����
			int randomNumber = dis(gen);
			for (int i = 0; i < 5; i++) {
				int num;
				do {
					num = dis(gen);  // ������ ����
				} while (std::find(screen::imageNum, screen::imageNum + i, num) != screen::imageNum + i);  // �ߺ� üũ
				screen::imageNum[i] = num;  // �ߺ����� ���� �� ����
			}
			//�ʱ�ȭ
			score = 0; 
			StageNect();
			Feature::SetStartAnimation();
			Feature::InitFeature();
			state = 1;
		}
		else if ((mouse.x > 2100 && mouse.x < 2400) && (mouse.y > 970 && mouse.y < 1200)) { //������
			PostQuitMessage(0);
		}
		//else if()�߰����������
	}

	void IngInput(input::MouseState mouse) 
	{
		mousex = mouse.x, mousey = mouse.y;
		if ((mouse.x > 640 && mouse.x < 930) && (mouse.y > 980 && mouse.y < 1030))// ��Ʈ����
			Feature::SetHint();
		else if ((mouse.x > 2100 && mouse.x < 2400) && (mouse.y > 970 && mouse.y < 1200)) { //������
			PostQuitMessage(0);
		}
		else if ((mouse.x > 320 && mouse.x < 2200) && (mouse.y > 30 && mouse.y < 970)) {
			if (Feature::CheckCorrect(mouse.x, mouse.y)) { //���� üũ
				score++;
				stageScore++;
				if (stageScore == 5) {
					Music::soundManager->PlayMusic(Music::eSoundList::success, Music::eSoundChannel::Effect);
					StageNect();
					Feature::SetAnimation(true);
				}
			}
			else { //Ʋ�����
				stageTimer += 1000; //�ð� ����
				clickTimer -= 1000; //Ŭ�� ����
			}
		}
	}

	void EndInput(input::MouseState mouse) 
	{
		if ((mouse.x > 2100 && mouse.x < 2400) && (mouse.y > 970 && mouse.y < 1200)) { //������
			PostQuitMessage(0);
		}
		stage = -1;
		state = 0;
		Music::soundManager->PlayMusic(Music::eSoundList::StartBGM, Music::eSoundChannel::BGM);
	}
} 