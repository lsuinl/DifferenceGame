#include "SUIN.h"
#include "RenderSystem.h"
#include <Windows.h>
#include "util.h"
#pragma comment(lib, "msimg32.lib")

namespace render
{
    HWND hWnd;

    HDC frontMemDC;    // 앞면 DC
    HDC backMemDC;    // 뒷면 DC

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
        AddFontResource(TEXT("source//font//BlackHanSans-Regular.ttf"));// 폰트 추가 
        HFONT currentFont = CreateFont(fontsize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Black Han Sans"));

        HFONT oldFont = (HFONT)SelectObject(backMemDC, currentFont);//그리기
        SetTextColor(backMemDC, color);
        SetTextColor(backMemDC, color);
        SetBkMode(backMemDC, TRANSPARENT); // 배경을 투명으로 설정
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

        SelectObject(bitmapMemDC, hOldBitmap);  // 이전 비트맵 복원
        DeleteObject(hBitmap);  // 비트맵 해제
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
        bmi.bmiHeader.biBitCount = 32;  // 32비트 컬러
        bmi.bmiHeader.biCompression = BI_RGB;
        BYTE* pBits;

        HBITMAP hBitmap32 = CreateDIBSection(bitmapMemDC, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);//버퍼생성
        HDC hdcTemp = CreateCompatibleDC(NULL); //버퍼로 복사
        HBITMAP hOldBitmapTemp = (HBITMAP)SelectObject(hdcTemp, hBitmap32);
        BitBlt(hdcTemp, 0, 0, imageWidth, imageHeight, bitmapMemDC, 0, 0, SRCCOPY);
        SelectObject(hdcTemp, hOldBitmapTemp);
        DeleteDC(hdcTemp);
        // 그리기
        TransparentBlt(backMemDC, x, y, imageWidth, imageHeight, bitmapMemDC, 0, 0, imageWidth, imageHeight, RGB(255, 255, 255));
        SelectObject(bitmapMemDC, hOldBitmap);  // 사용한 리소스 해제
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