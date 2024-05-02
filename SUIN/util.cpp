#include "util.h"
#include "SUIN.h"

namespace util {
	char* IntToChar(int num) {
		char* chars = new char[3];
		chars[0] = (num / 10)+48;
		chars[1] = (num % 10) + 48;
        chars[2] = '\0';
		return chars;
	}

	int CheckSize(const char* name) {
		for (int i = 0; i < 20; i++) 
			if (name[i] =='\0') return i;
		return 10;
	}

    char* SumChar(const char** words, int count) {
        int totalLength = 0;
        for (int i = 0; i < count; ++i)  //총 길이
            totalLength += strlen(words[i]);

        char* sentences = new char[totalLength + 1]; //문자열 메모리 생성
        int currentIndex = 0;

        for (int i = 0; i < count; ++i) { //합치기
            strcpy_s(&sentences[currentIndex], totalLength + 1 - currentIndex, words[i]);
            currentIndex += strlen(words[i]);
        }
        sentences[currentIndex] = '\0'; 

        return sentences;
    }
}