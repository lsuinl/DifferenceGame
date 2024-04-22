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
        std::string name = "";  // �Էµ� Ű�� ���ڸ� ������ ���ڿ�

        BYTE keyboardState[256];  // Ű���� ���¸� ������ �迭

        // ���� Ű���� ���� ��������
        if (!GetKeyboardState(keyboardState)) {
            return "";
        }

        // �� Ű���� Ű�� ���� ���� Ȯ��
        for (int i = 0; i < 256; ++i) {
            if (keyboardState[i] & 0x80) {  // Ű�� ���ȴ��� Ȯ��
                //if (keyboardState[0x08]) continue;
                char buffer[2] = { 0, 0 };
                // Ű���� ���¸� ���ڷ� ��ȯ
                if (ToAscii(i, 0, keyboardState, (LPWORD)buffer, 0)) {
                    name += buffer;
                    if (name.length() >= 10) {  // �ִ� ���� �˻�
                        break;
                    }
                }
            }
        }

        return name;
    }
}