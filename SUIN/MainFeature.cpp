#include "MainFeature.h"
#include "SUIN.h"
#include "RenderSystem.h"
#include "GameManager.h"

namespace Feature
{
	bool rightnum[5] = { 0,0,0,0,0 };
	int hint = 5;
	int xpos[5], ypos[5], cx[5], cy[5];

	void DrawCorrect( ) {
		for (int i = 0; i < 5; i++) {
			if(rightnum[i] == true) {
				render::DrawBackGround("source/correct.bmp", cx[i], cy[i], xpos[i], ypos[i], true);
				render::DrawBackGround("source/correct.bmp", cx[i], cy[i], xpos[i] + 870, ypos[i], true);
			}
		}
	}
	bool CheckCorrect(int x, int y) {
		for (int i = 0; i < 5; i++) {
			if (((xpos[i]<x && xpos[i] + cx[i]>x) || (xpos[i] + 870 < x && xpos[i] + 870 + cx[i] > x)) && ypos[i]<y && ypos[i] + cy[i]>y && !rightnum[i]) {
				rightnum[i] = true;
				return true;
			}
		}
		return false;
	}
	void StageInit() {
		for(int i=0;i<5;i++)
			rightnum[i] = false;
	}
	void Hint() {

	}
	void SetPos(int num) {
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
		else if (num == 0) {//7번
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