#include "SUIN.h"
#include "RenderSystem.h"
#include <Windows.h>
#include "util.h"
#pragma comment(lib, "msimg32.lib")

namespace render
{
    HWND hWnd;

    HDC frontMemDC;    // �ո� DC
    HDC backMemDC;    // �޸� DC

    HBITMAP backBitmap = nullptr;

    void InitRender()
    {
        hWnd = global::GetWinApp().GetWindow();
        frontMemDC = GetDC(hWnd);
        backMemDC = CreateCompatibleDC(frontMemDC);
        backBitmap = CreateCompatibleBitmap(frontMemDC, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight());
    }

    void ClearScreen()
    {
        ::PatBlt(backMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), WHITENESS);
    }

    void BeginDraw()
    {
        ClearScreen();
        ::SelectObject(backMemDC, backBitmap);
    }

    void EndDraw()
    {
        ::BitBlt(frontMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), backMemDC, 0, 0, SRCCOPY);
    }

    void ReleaseRender()
    {
        DeleteObject(backBitmap);
        DeleteDC(backMemDC);
        ReleaseDC(hWnd, frontMemDC);
    }

    void DrawText(int x, int y, const char* text, COLORREF color,int fontsize)
    {
        AddFontResource(TEXT("source//font//BlackHanSans-Regular.ttf"));// ��Ʈ �߰� 
        HFONT currentFont = CreateFont(fontsize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Black Han Sans"));

        HFONT oldFont = (HFONT)SelectObject(backMemDC, currentFont);//�׸���
        SetTextColor(backMemDC, color);
        SetTextColor(backMemDC, color);
        SetBkMode(backMemDC, TRANSPARENT); // ����� �������� ����
        TextOutA(backMemDC, x, y, text, util::CheckSize(text));
        SelectObject(backMemDC, oldFont);
        SelectObject(backMemDC, oldFont);

        DeleteObject(currentFont);
    }

    void DrawBitmap(int x, int y, HBITMAP hBitmap)
    {
        HDC bitmapMemDC = CreateCompatibleDC(frontMemDC);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(bitmapMemDC, hBitmap);
        BITMAP bm;
        GetObject(hBitmap, sizeof(BITMAP), &bm);
        HWND hWnd = global::GetWinApp().GetHWnd();

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        int windowWidth = clientRect.right - clientRect.left;
        int windowHeight = clientRect.bottom - clientRect.top;

        ::BitBlt(backMemDC, x, y, windowWidth, windowHeight, bitmapMemDC, 0, 0, SRCCOPY);

        SelectObject(bitmapMemDC, hOldBitmap);  // ���� ��Ʈ�� ����
        DeleteObject(hBitmap);  // ��Ʈ�� ����
        DeleteDC(bitmapMemDC);

    }
    void DrawBitmapTo(int x, int y, HBITMAP hBitmap)
    {
        HDC bitmapMemDC = CreateCompatibleDC(frontMemDC);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(bitmapMemDC, hBitmap);
        BITMAP bm;
        GetObject(hBitmap, sizeof(BITMAP), &bm);
        int imageWidth = bm.bmWidth;
        int imageHeight = bm.bmHeight;
        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = imageWidth;
        bmi.bmiHeader.biHeight = -imageHeight;  // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;  // 32��Ʈ �÷�
        bmi.bmiHeader.biCompression = BI_RGB;
        BYTE* pBits;

        HBITMAP hBitmap32 = CreateDIBSection(bitmapMemDC, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);//���ۻ���
        HDC hdcTemp = CreateCompatibleDC(NULL); //���۷� ����
        HBITMAP hOldBitmapTemp = (HBITMAP)SelectObject(hdcTemp, hBitmap32);
        BitBlt(hdcTemp, 0, 0, imageWidth, imageHeight, bitmapMemDC, 0, 0, SRCCOPY);
        SelectObject(hdcTemp, hOldBitmapTemp);
        DeleteDC(hdcTemp);
        // �׸���
        TransparentBlt(backMemDC, x, y, imageWidth, imageHeight, bitmapMemDC, 0, 0, imageWidth, imageHeight, RGB(255, 255, 255));
        SelectObject(bitmapMemDC, hOldBitmap);  // ����� ���ҽ� ����
        DeleteObject(hBitmap32);
        DeleteDC(bitmapMemDC);
    }

    HBITMAP DrawBackGround(const char* name, int width, int height, int x, int y,bool to)
    {
        HBITMAP hBackmap = (HBITMAP)LoadImageA(NULL, name, IMAGE_BITMAP, width, height, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        if (to) render::DrawBitmapTo(x, y, hBackmap);
        else render::DrawBitmap(x, y, hBackmap);
        DeleteObject(hBackmap);
        return hBackmap;
    }
}