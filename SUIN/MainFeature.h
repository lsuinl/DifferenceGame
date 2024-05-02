#pragma once

namespace Feature
{
	void InitFeature(); //��� �ʱ�ȭ

	void DrawCorrect(); //���� �׸���

	void SetPos(int num); //���� ��ǥ ����

	bool CheckCorrect(int x, int y); //���� üũ�ϱ�

	void SetHint(); //��Ʈ Ȱ��ȭ

	void DrawHint(); //��Ʈ�׸���

	void SetAnimation(bool right); //�ִϸ��̼� Ȱ��ȭ

	bool GetAnimation(); //�ִϸ��̼� ���º���

	void DrawAnimation(const char* , const char*); //�ִϸ��̼� �׸���

	void SetStartAnimation();//���۾ִϸ��̼� Ȱ��ȭ

	bool GetStartAnimation();//���۾ִϸ��̼� ���º���

	void DrawStartAnimation();//���۾ִϸ��̼� �׸���

	void Ranking(const char*, int score); //��ŷ ���

	void DrawRankingInfo(const char* name, int score); //��ŷ����(��������)

	void DrawRanking(); //��ŷ����(ĳ����â����)
}		