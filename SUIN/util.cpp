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
		for (int i = 0; i < 20; i++) {
			if (name[i] =='\0')
				return i;
		}
		return 10;
	}

    char* SumChar(const char** words, int count) {
        int totalLength = 0;

        // 모든 단어의 총 길이 계산
        for (int i = 0; i < count; ++i) {
            totalLength += strlen(words[i]);
        }

        // null 문자를 포함한 문자열 길이만큼 메모리 할당
        char* sentences = new char[totalLength + 1];
        int currentIndex = 0;

        // 모든 단어를 합쳐서 하나의 문자열로 만듦
        for (int i = 0; i < count; ++i) {
            strcpy_s(&sentences[currentIndex], totalLength + 1 - currentIndex, words[i]);
            currentIndex += strlen(words[i]);
        }

        // null 문자 추가
        sentences[currentIndex] = '\0';

        return sentences;
    }
}