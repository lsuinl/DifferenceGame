#include "SUIN.h"
#include "RenderSystem.h"
#include <Windows.h>
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

    void DrawLine(int x1, int y1, int x2, int y2, COLORREF color)
    {
        HPEN hPen = CreatePen(PS_SOLID, 5, color);

        HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

        MoveToEx(backMemDC, x1, y1, NULL);

        LineTo(backMemDC, x2, y2);

        SelectObject(backMemDC, hOldPen);

        DeleteObject(hPen);
    }


    void DrawRect(int x, int y, int width, int height, COLORREF color)
    {
        HPEN hPen = CreatePen(PS_SOLID, 1, color);

        HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

        HBRUSH hBrush = CreateSolidBrush(color);

        HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);

        Rectangle(backMemDC, x, y, x + width, y + height);

        SelectObject(backMemDC, hOldPen);
        SelectObject(backMemDC, hOldBrush);

        DeleteObject(hPen);
        DeleteObject(hBrush);
    }


    void DrawCircle(int x, int y, int radius, COLORREF color)
    {
        HPEN hPen = CreatePen(PS_SOLID, 1, color);

        HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

        HBRUSH hBrush = CreateSolidBrush(color);

        HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);

        Ellipse(backMemDC, x - radius, y - radius, x + radius, y + radius);

        SelectObject(backMemDC, hOldPen);
        SelectObject(backMemDC, hOldBrush);

        DeleteObject(hPen);
        DeleteObject(hBrush);

    }

    void DrawPolygon(POINT points[], int count, COLORREF color)
    {
        HPEN hPen = CreatePen(PS_SOLID, 1, color);

        HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

        HBRUSH hBrush = CreateSolidBrush(color);

        HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);

        Polygon(backMemDC, points, count);

        SelectObject(backMemDC, hOldPen);
        SelectObject(backMemDC, hOldBrush);

        DeleteObject(hPen);
        DeleteObject(hBrush);

    }

    void DrawText(int x, int y, const char* text, COLORREF color,int fontsize)
    {
        // 폰트 추가 생성
        AddFontResource(TEXT("source/font/BlackHanSans-Regular.ttf"));
        HFONT currentFont = CreateFont(fontsize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Black Han Sans"));

        // 백 버퍼에 텍스트 그리기
        HFONT oldFont = (HFONT)SelectObject(backMemDC, currentFont);
        SetTextColor(backMemDC, color);
        SetTextColor(backMemDC, color);
        SetBkMode(backMemDC, TRANSPARENT); // 배경을 투명으로 설정
        TextOutA(backMemDC, x, y, text, strlen(text));
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
        // 윈도우의 크기를 얻음
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

        // DIB를 위한 정보를 저장할 BITMAPINFO 구조체 생성
        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = imageWidth;
        bmi.bmiHeader.biHeight = -imageHeight;  // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;  // 32비트 컬러 (RGB + Alpha)
        bmi.bmiHeader.biCompression = BI_RGB;

        // 이미지의 픽셀 데이터를 가져올 메모리 버퍼 생성
        BYTE* pBits;
        HBITMAP hBitmap32 = CreateDIBSection(bitmapMemDC, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);

        // 이미지를 32비트 형식으로 변환하여 메모리 버퍼에 복사
        HDC hdcTemp = CreateCompatibleDC(NULL);
        HBITMAP hOldBitmapTemp = (HBITMAP)SelectObject(hdcTemp, hBitmap32);
        BitBlt(hdcTemp, 0, 0, imageWidth, imageHeight, bitmapMemDC, 0, 0, SRCCOPY);
        SelectObject(hdcTemp, hOldBitmapTemp);
        DeleteDC(hdcTemp);

        // TransparentBlt 함수를 사용하여 이미지 그리기
        TransparentBlt(backMemDC, x, y, imageWidth, imageHeight, bitmapMemDC, 0, 0, imageWidth, imageHeight, RGB(255, 255, 255));

        // 사용한 리소스 해제
        SelectObject(bitmapMemDC, hOldBitmap);
        DeleteObject(hBitmap32);
        DeleteDC(bitmapMemDC);
    }


    HBITMAP LoadImages(const char* path,int width,int height)
    {
        HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, width, height, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        return hBitmap;
    }

    void ReleaseImage(HBITMAP hBitmap)
    {
        DeleteObject(hBitmap);
    }
    HBITMAP DrawBackGround(const char* name, int width, int height, int x, int y,bool to)
    {
        HBITMAP hBackmap = render::LoadImages(name, width, height);
        if (to)
            render::DrawBitmapTo(x, y, hBackmap);
        else
            render::DrawBitmap(x, y, hBackmap);
        render::ReleaseImage(hBackmap);
        return hBackmap;
    }
}