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
        for (int i = 0; i < count; ++i)  //�� ����
            totalLength += strlen(words[i]);

        char* sentences = new char[totalLength + 1]; //���ڿ� �޸� ����
        int currentIndex = 0;

        for (int i = 0; i < count; ++i) { //��ġ��
            strcpy_s(&sentences[currentIndex], totalLength + 1 - currentIndex, words[i]);
            currentIndex += strlen(words[i]);
        }
        sentences[currentIndex] = '\0'; 

        return sentences;
    }
}