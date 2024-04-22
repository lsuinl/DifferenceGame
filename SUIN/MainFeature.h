#pragma once

namespace Feature
{
	void FeatureInit();
	void DrawCorrect();
	void SetPos(int num);
	bool CheckCorrect(int x, int y);
	void StageInit();
	void SetHint();
	void DrawHint();
	void SetAnimation(bool right);
	bool GetAnimation();
	void DrawAnimation(const char* , const char*);
	void Ranking(const char*, int score);
	void DrawRanking();
	void InitFeature();
}