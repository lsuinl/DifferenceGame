#pragma once

namespace render
{
    void InitRender();

    void BeginDraw();

    void EndDraw();

    void ReleaseRender();

    void DrawText(int x, int y, const char* text, COLORREF color, int fontsize); //글자.

    void DrawBitmap(int x, int y, HBITMAP hBitmap); //그림(이미지 그리기 하위)
    void DrawBitmapTo(int x, int y, HBITMAP hBitmap); //배경 투명 그림(이미지 그리기 하위)

    HBITMAP DrawBackGround(const char* name, int width, int height, int x, int y,bool to); //이미지 그리기
    }