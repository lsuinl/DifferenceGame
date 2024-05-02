#pragma once

namespace Feature
{
	void InitFeature(); //기능 초기화

	void DrawCorrect(); //정답 그리기

	void SetPos(int num); //정답 좌표 셋팅

	bool CheckCorrect(int x, int y); //정답 체크하기

	void SetHint(); //힌트 활성화

	void DrawHint(); //힌트그리기

	void SetAnimation(bool right); //애니메이션 활성화

	bool GetAnimation(); //애니메이션 상태보기

	void DrawAnimation(const char* , const char*); //애니메이션 그리기

	void SetStartAnimation();//시작애니메이션 활성화

	bool GetStartAnimation();//시작애니메이션 상태보기

	void DrawStartAnimation();//시작애니메이션 그리기

	void Ranking(const char*, int score); //랭킹 기록

	void DrawRankingInfo(const char* name, int score); //랭킹정보(엔딩에서)

	void DrawRanking(); //랭킹정보(캐릭터창에서)
}		