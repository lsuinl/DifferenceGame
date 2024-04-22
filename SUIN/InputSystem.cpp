#include "SUIN.h"
#include "InputSystem.h"
#include <string>

namespace input
{
    bool isKeyDown[256];
    bool isKeyUp[256];
    bool isKey[256];

    MouseState curMouse;
    MouseState prevMouse;


    void ResetInput()
    {
        for (int i = 0; i < 256; i++)
        {
            isKeyDown[i] = false;
            isKeyUp[i] = false;
        }
    }

    void KeyDown(unsigned int key)    {
        isKeyDown[key] = true;
        isKey[key] = true;
    }

    void KeyUp(unsigned int key)
    {
        isKeyUp[key] = true;
        isKey[key] = false;
    }

    bool IsKeyDown(unsigned int key)
    {
        return isKeyDown[key];
    }

    bool IsKeyUp(unsigned int key)
    {
        return isKeyUp[key];
    }

    bool IsKey(unsigned int key)
    {
        return isKey[key];
    }

    void InitMouse()
    {
        curMouse.x = global::GetWinApp().GetWidth() / 2;
        curMouse.y = global::GetWinApp().GetHeight() / 2;
        curMouse.wheel = 0;

        curMouse.left = false;
        curMouse.right = false;
        curMouse.middle = false;

        prevMouse = curMouse;

        SetCursorPos(curMouse.x, curMouse.y);
    }

    void UpdateMouse()
    {
        prevMouse = curMouse;

        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(global::GetWinApp().GetWindow(), &pt);

        curMouse.x = pt.x;
        curMouse.y = pt.y;
        curMouse.wheel = 0;

        curMouse.left = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
        curMouse.right = (GetKeyState(VK_RBUTTON) & 0x8000) != 0;
        curMouse.middle = (GetKeyState(VK_MBUTTON) & 0x8000) != 0;
    }

    const MouseState& GetMouseState()
    {
        return curMouse;
    }

    const MouseState& GetPrevMouseState()
    {
        return prevMouse;
    }

    void InitInput()
    {
        for (int i = 0; i < 256; i++)
        {
            isKeyDown[i] = false;
            isKeyUp[i] = false;
            isKey[i] = false;
        }

        InitMouse();
    }

    bool IsSame(const MouseState& a, const MouseState& b)
    {
        return a.x == b.x && a.y == b.y && a.wheel == b.wheel && a.left == b.left && a.right == b.right && a.middle == b.middle;
    }

    std::string GetPressedKey() {
        std::string name = "";  // 입력된 키의 문자를 저장할 문자열

        BYTE keyboardState[256];  // 키보드 상태를 저장할 배열

        // 현재 키보드 상태 가져오기
        if (!GetKeyboardState(keyboardState)) {
            return "";
        }

        // 각 키보드 키에 대해 상태 확인
        for (int i = 0; i < 256; ++i) {
            if (keyboardState[i] & 0x80) {  // 키가 눌렸는지 확인
                //if (keyboardState[0x08]) continue;
                char buffer[2] = { 0, 0 };
                // 키보드 상태를 문자로 변환
                if (ToAscii(i, 0, keyboardState, (LPWORD)buffer, 0)) {
                    name += buffer;
                    if (name.length() >= 10) {  // 최대 길이 검사
                        break;
                    }
                }
            }
        }

        return name;
    }
}