#pragma once

namespace util {
	char* IntToChar(int num); //숫자->문자열

	int CheckSize(const char* name);//글자 크기 체크

	char* SumChar(const char** words, int count); //문자열 합치기
}

