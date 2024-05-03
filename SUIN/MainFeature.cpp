#include "MainFeature.h"
#include "SUIN.h"
#include "RenderSystem.h"
#include "TimeSystem.h"
#include "Music.h"
#include "util.h"
struct rank {
	const char* name;
	int score;
};

namespace Feature
{
	bool rightnum[5] = { 0,0,0,0,0 };
	int hintRe = 5;
	int xpos[5], ypos[5], cx[5], cy[5];

	//logo: 성공, 실패 로고, Animation: 화면전환애니메이션
	bool animation = false, perfect;
	int logoLimit = 1000, animationLimit = 2000, animationTime = 0; 
	//시작 애니메이션 
	bool startAnimation = false;
	int startAnimationLimit = 2500, startAnimationTime = 0;
	//힌트
	bool hint = false;
	int hintLimit = 5000,hintTime=0, hintIndex=-1;
	
	//랭킹
	rank* ranks[6] = {
	new rank{"Player1", 5},
	new rank{"Player2", 4},
	new rank{"Player3", 3},
	new rank{"Player4", 2},
	new rank{"Player5", 1},
	new rank{"Player6", 1}
	}; 

	void InitFeature() 
	{
		 hintRe = 5;
		 hint = false;
		 animation = false;
		 animationTime = 0;
		 hintTime = 0;
	}

	void Ranking(const char* name, int score)
	{
		int check = 6;
		for (int i = 0; i < 6; i++) {
			if (ranks[i]->score <= score) {
				check = i;
				break;  // 이미 정렬된 배열에서 해당 조건을 만족하는 위치를 찾았으므로 루프를 종료합니다.
			}
		}
		if (check < 6) {
			for (int i = 5; i > check; i--) {
				ranks[i] = ranks[i - 1];
			}
		}
		ranks[check] = new rank{ name, score };  // 새로운 Rank 객체를 동적으로 할당하여 저장합니다.
	}

	void DrawRankingInfo(const char* names, int score) 
	{
		render::DrawText(145 - (util::CheckSize(names) * 7), 40, names, RGB(253, 208, 0), 30);
		render::DrawText(90, 60, util::IntToChar(score), RGB(253, 208, 0), 100);
		render::DrawText(145 - (util::CheckSize(ranks[0]->name) * 7), 360, ranks[1]->name, RGB(255, 255, 255), 30);
		render::DrawText(90, 380, util::IntToChar(ranks[0]->score), RGB(253, 208, 0), 100);
		render::DrawText(145 - (util::CheckSize(ranks[1]->name) * 7), 670, ranks[2]->name, RGB(255, 255, 255), 30);
		render::DrawText(90, 690, util::IntToChar(ranks[1]->score), RGB(253, 208, 0), 100);
		render::DrawText(2230 - (util::CheckSize(ranks[2]->name) * 7), 40, ranks[3]->name, RGB(255, 255, 255), 30);
		render::DrawText(2180, 60, util::IntToChar(ranks[2]->score), RGB(253, 208, 0), 100);
		render::DrawText(2230 - (util::CheckSize(ranks[3]->name) * 7), 360, ranks[4]->name, RGB(255, 255, 255), 30);
		render::DrawText(2180, 380, util::IntToChar(ranks[3]->score), RGB(253, 208, 0), 100);
		render::DrawText(2230 - (util::CheckSize(ranks[4]->name)*7), 670, ranks[5]->name, RGB(255, 255, 255), 30);
		render::DrawText(2180, 690, util::IntToChar(ranks[4]->score), RGB(253, 208, 0), 100);
	}

	void DrawRanking() 
	{
		int yname[6] = {315,357,399,441,488,530};
		for (int i = 0; i < 6; i++) {
			render::DrawText(900, yname[i], ranks[i]->name, RGB(255, 255, 255), 30);
			render::DrawText(1220, yname[i], util::IntToChar(ranks[i]->score), RGB(255, 255, 255), 30);
		}
	}

	void SetAnimation(bool right) 
	{
		perfect = right;
		animation = true;
	}

	bool GetAnimation() 
	{
		return animation;
	}

	void DrawAnimation(const char* pic1, const char* pic2)
	{
		if (animation) {
			animationTime += ts::GetDeltaTime();
			if (animationTime < logoLimit) {//로고 띄우기
				if(perfect)render::DrawBackGround("source//perfect.bmp", 1700, 600, 400, 200, true);
				else render::DrawBackGround("source//defect.bmp", 1200, 700, 600, 200, true);
			}
			else if(animationTime < animationLimit) {//애니메이션 젼환
				render::DrawBackGround(pic1, 855, 930 - ((animationLimit - animationTime)*0.93), 330, 40, false);
				render::DrawBackGround(pic2, 855, 930 - ((animationLimit - animationTime) * 0.93), 1200, 40, false);
			}
			else if(animationTime >= animationLimit) { //전환 종료
				animationTime = 0;
				animation = false;
			}
		}
	}

	void SetStartAnimation()
	{
		startAnimation = true;
	}

	bool GetStartAnimation() 
	{
		return startAnimation;
	}

	void DrawStartAnimation()
	{
		if (startAnimation) {
			startAnimationTime += ts::GetDeltaTime();
			if (startAnimationTime < startAnimationLimit-(1770/3)) {
				render::DrawBackGround("source//READY.bmp", 1770, 940, 300, 35, false);
			}
			else if (startAnimationTime < startAnimationLimit) {//애니메이션 젼환
				render::DrawBackGround("source//READY.bmp", (startAnimationLimit - startAnimationTime)*3, 940, 300, 35, false);
			}
			else if (startAnimationTime >= startAnimationLimit) { //전환 종료
				startAnimationTime = 0;
				startAnimation = false;
			}
		}
	}

	bool CheckCorrect(int x, int y)
	{
		//범위안 먼저 체크
		for (int i = 0; i < 5; i++) {
			if (((xpos[i]<x && xpos[i] + cx[i]>x) || (xpos[i] + 870 < x && xpos[i] + 870 + cx[i] > x)) && ypos[i]<y && ypos[i] + cy[i]>y && !rightnum[i]) {
				Music::soundManager->PlayMusic(Music::eSoundList::correct, Music::eSoundChannel::Effect); 
				rightnum[i] = true;
				return true;
			}
		}
		//틀렸을 때,
		Music::soundManager->PlayMusic(Music::eSoundList::wrong, Music::eSoundChannel::Effect);
		return false;
	}

	void DrawCorrect()
	{
		for (int i = 0; i < 5; i++) {
			if (rightnum[i] == true) {
				render::DrawBackGround("source//correct.bmp", cx[i], cy[i], xpos[i], ypos[i], true);
				render::DrawBackGround("source//correct.bmp", cx[i], cy[i], xpos[i] + 870, ypos[i], true);
			}
		}
	}

	void SetHint() 
	{
		if (hintRe > 0) {
			Music::soundManager->PlayMusic(Music::eSoundList::hint, Music::eSoundChannel::Effect);
			for (int i = 0; i < 5; i++) {
				if (!rightnum[i]) {
					hintIndex = i;
					break;
				}
			}
			hint = true;
			hintRe--;
		}
	}

	void DrawHint() 
	{
		if (hint) {
			if(hintTime < hintLimit)
				hintTime += ts::GetDeltaTime();
			else {
				hintTime = 0;
				hint = false;
			}
			if (!rightnum[hintIndex]) { //맞추면 안보여줌
				render::DrawBackGround("source//hint.bmp", cx[hintIndex], cy[hintIndex], xpos[hintIndex], ypos[hintIndex], true);
				render::DrawBackGround("source//hint.bmp", cx[hintIndex], cy[hintIndex], xpos[hintIndex] + 870, ypos[hintIndex], true);
			}
		}
		render::DrawText(850, 985, util::IntToChar(hintRe), RGB(253, 208, 0), 50);
	}

	void SetPos(int num)
	{
		for (int i = 0; i < 5; i++) rightnum[i] = false;
		if (num == 1) {//1번
			xpos[0] = 410, xpos[1] = 500, xpos[2] = 550, xpos[3] = 870, xpos[4] = 850;
			ypos[0] = 480, ypos[1] = 160, ypos[2] = 570, ypos[3] = 625, ypos[4] = 420;
			cx[0] = 40, cx[1] = 70, cx[2] = 50, cx[3] = 50, cx[4] = 60;
			cy[0] = 40, cy[1] = 50, cy[2] = 50, cy[3] = 50, cy[4] = 60;
		}
		else if (num == 2) {//2번
			xpos[0] = 350, xpos[1] = 440, xpos[2] = 800, xpos[3] = 930, xpos[4] = 780;
			ypos[0] = 90, ypos[1] = 520, ypos[2] = 360, ypos[3] = 330, ypos[4] = 260;
			cx[0] = 60, cx[1] = 60, cx[2] = 50, cx[3] = 70, cx[4] = 50;
			cy[0] = 60, cy[1] = 60, cy[2] = 50, cy[3] = 70, cy[4] = 50;
		}
		else if (num == 3) {//3번
			xpos[0] = 620, xpos[1] = 330, xpos[2] = 500, xpos[3] = 710, xpos[4] = 790;
			ypos[0] = 940, ypos[1] = 40, ypos[2] = 210, ypos[3] = 390, ypos[4] = 600;
			cx[0] = 150, cx[1] = 90, cx[2] = 50, cx[3] = 50, cx[4] = 50;
			cy[0] = 40, cy[1] = 120, cy[2] = 50, cy[3] = 50, cy[4] = 50;
		}
		else if (num == 4) {//4번
			xpos[0] = 510, xpos[1] = 580, xpos[2] = 790, xpos[3] = 990, xpos[4] = 1060;
			ypos[0] = 230, ypos[1] = 460, ypos[2] = 540, ypos[3] = 910, ypos[4] = 340;
			cx[0] = 60, cx[1] = 60, cx[2] = 50, cx[3] = 100, cx[4] = 120;
			cy[0] = 60, cy[1] = 60, cy[2] = 50, cy[3] = 70, cy[4] = 80;
		}
		else if (num == 5) {//5번
			xpos[0] = 400, xpos[1] = 500, xpos[2] = 670, xpos[3] = 770, xpos[4] = 970;
			ypos[0] = 790, ypos[1] = 110, ypos[2] = 810, ypos[3] = 680, ypos[4] = 490;
			cx[0] = 60, cx[1] = 60, cx[2] = 60, cx[3] = 100, cx[4] = 50;
			cy[0] = 60, cy[1] = 60, cy[2] = 60, cy[3] = 70, cy[4] = 50;
		}
		else if (num == 6) {//6번
			xpos[0] = 520, xpos[1] = 330, xpos[2] = 630, xpos[3] = 950, xpos[4] = 1060;
			ypos[0] = 850, ypos[1] = 80, ypos[2] = 40, ypos[3] = 390, ypos[4] = 310;
			cx[0] = 100, cx[1] = 60, cx[2] = 60, cx[3] = 50, cx[4] = 60;
			cy[0] = 60, cy[1] = 120, cy[2] = 60, cy[3] = 50, cy[4] = 60;
		}
		else if (num == 7) {//7번
			xpos[0] = 365, xpos[1] = 985, xpos[2] = 325, xpos[3] = 900, xpos[4] = 670;
			ypos[0] = 550, ypos[1] = 580, ypos[2] = 400, ypos[3] = 100, ypos[4] = 390;
			cx[0] = 50, cx[1] = 120, cx[2] = 50, cx[3] = 50, cx[4] = 50;
			cy[0] = 50, cy[1] = 50, cy[2] = 50, cy[3] = 50, cy[4] = 50;
		}
		else if (num == 8) {//8번
			xpos[0] = 350, xpos[1] = 620, xpos[2] = 690, xpos[3] = 710, xpos[4] = 960;
			ypos[0] = 540, ypos[1] = 50, ypos[2] = 150, ypos[3] = 530, ypos[4] = 340;
			cx[0] = 100, cx[1] = 60, cx[2] = 120, cx[3] = 60, cx[4] = 60;
			cy[0] = 100, cy[1] = 60, cy[2] = 60, cy[3] = 60, cy[4] = 60;
		}
		else if (num == 9) {//9번
			xpos[0] = 340, xpos[1] = 700, xpos[2] = 670, xpos[3] = 490, xpos[4] = 1050;
			ypos[0] = 610, ypos[1] = 500, ypos[2] = 940, ypos[3] = 890, ypos[4] = 40;
			cx[0] = 60, cx[1] = 120, cx[2] = 170, cx[3] = 120, cx[4] = 120;
			cy[0] = 60, cy[1] = 60, cy[2] = 40, cy[3] = 60, cy[4] = 60;
		}
		else if (num == 10) {//10번
			xpos[0] = 380, xpos[1] = 510, xpos[2] = 860, xpos[3] = 1000, xpos[4] = 1100;
			ypos[0] = 680, ypos[1] = 630, ypos[2] = 330, ypos[3] = 50, ypos[4] = 290;
			cx[0] = 60, cx[1] = 80, cx[2] = 40, cx[3] = 150, cx[4] = 60;
			cy[0] = 60, cy[1] = 80, cy[2] = 120, cy[3] = 60, cy[4] = 60;
		}
		else if (num == 11) {//11번
			xpos[0] = 610, xpos[1] = 650, xpos[2] = 760, xpos[3] = 1120, xpos[4] = 930;
			ypos[0] = 430, ypos[1] = 610, ypos[2] = 840, ypos[3] = 690, ypos[4] = 400;
			cx[0] = 60, cx[1] = 60, cx[2] = 60, cx[3] = 60, cx[4] = 60;
			cy[0] = 60, cy[1] = 60, cy[2] = 60, cy[3] = 60, cy[4] = 50;
		}
		else if (num == 12) {//12번
			xpos[0] = 550, xpos[1] = 680, xpos[2] = 760, xpos[3] = 850, xpos[4] = 970;
			ypos[0] = 280, ypos[1] = 750, ypos[2] = 590, ypos[3] = 460, ypos[4] = 730;
			cx[0] = 120, cx[1] = 60, cx[2] = 60, cx[3] = 60, cx[4] = 60;
			cy[0] = 80, cy[1] = 60, cy[2] = 60, cy[3] = 60, cy[4] = 60;
		}
		else if (num == 13) {//13번
			xpos[0] = 620, xpos[1] = 780, xpos[2] = 830, xpos[3] = 1000, xpos[4] = 1080;
			ypos[0] = 670, ypos[1] = 290, ypos[2] = 70, ypos[3] = 610, ypos[4] = 130;
			cx[0] = 50, cx[1] = 50, cx[2] = 80, cx[3] = 100, cx[4] = 50;
			cy[0] = 50, cy[1] = 50, cy[2] = 80, cy[3] = 50, cy[4] = 50;
		}
		else if (num == 14) {//14번
			xpos[0] = 330, xpos[1] = 500, xpos[2] = 520, xpos[3] = 790, xpos[4] = 1150;
			ypos[0] = 930, ypos[1] = 520, ypos[2] = 160, ypos[3] = 630, ypos[4] = 370;
			cx[0] = 100, cx[1] = 90, cx[2] = 60, cx[3] = 60, cx[4] = 40;
			cy[0] = 60, cy[1] = 60, cy[2] = 60, cy[3] = 60, cy[4] = 60;
		}
		else if (num == 15) {//15번
			xpos[0] = 330, xpos[1] = 460, xpos[2] = 470, xpos[3] = 590, xpos[4] = 660;
			ypos[0] = 390, ypos[1] = 280, ypos[2] = 730, ypos[3] = 600, ypos[4] = 150;
			cx[0] = 60, cx[1] = 60, cx[2] = 60, cx[3] = 60, cx[4] = 60;
			cy[0] = 60, cy[1] = 60, cy[2] = 60, cy[3] = 60, cy[4] = 60;
		}
		else if (num == 16) {//16번
			xpos[0] = 330, xpos[1] = 820, xpos[2] = 1010, xpos[3] = 1050, xpos[4] = 1080;
			ypos[0] = 40, ypos[1] = 270, ypos[2] = 640, ypos[3] = 750, ypos[4] = 850;
			cx[0] = 60, cx[1] = 60, cx[2] = 60, cx[3] = 60, cx[4] = 60;
			cy[0] = 60, cy[1] = 60, cy[2] = 60, cy[3] = 60, cy[4] = 60;
		}
		else if (num == 17) {//17번
			xpos[0] = 320, xpos[1] = 570, xpos[2] = 770, xpos[3] = 810, xpos[4] = 1070;
			ypos[0] = 380, ypos[1] = 50, ypos[2] = 520, ypos[3] = 580, ypos[4] = 300;
			cx[0] = 120, cx[1] = 60, cx[2] = 60, cx[3] = 60, cx[4] = 60;
			cy[0] = 60, cy[1] = 60, cy[2] = 60, cy[3] = 60, cy[4] = 60;
		}
		else if (num == 18) {//18번
			xpos[0] = 440, xpos[1] = 730, xpos[2] = 750, xpos[3] = 770, xpos[4] = 960;
			ypos[0] = 190, ypos[1] = 790, ypos[2] = 690, ypos[3] = 930, ypos[4] = 310;
			cx[0] = 60, cx[1] = 100, cx[2] = 60, cx[3] = 150, cx[4] = 60;
			cy[0] = 100, cy[1] = 60, cy[2] = 60, cy[3] = 60, cy[4] = 60;
			}
		else if (num == 19) {//19번
			xpos[0] = 420, xpos[1] = 470, xpos[2] = 570, xpos[3] = 760, xpos[4] = 950;
			ypos[0] = 790, ypos[1] = 360, ypos[2] = 380, ypos[3] = 740, ypos[4] = 360;
			cx[0] = 60, cx[1] = 60, cx[2] = 60, cx[3] = 90, cx[4] = 60;
			cy[0] = 60, cy[1] = 60, cy[2] = 60, cy[3] = 90, cy[4] = 60;
			}
	}
}