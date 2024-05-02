#pragma once

namespace render
{
    void InitRender();

    void BeginDraw();

    void EndDraw();

    void ReleaseRender();

    void DrawText(int x, int y, const char* text, COLORREF color, int fontsize); //����.

    void DrawBitmap(int x, int y, HBITMAP hBitmap); //�׸�(�̹��� �׸��� ����)
    void DrawBitmapTo(int x, int y, HBITMAP hBitmap); //��� ���� �׸�(�̹��� �׸��� ����)

    HBITMAP DrawBackGround(const char* name, int width, int height, int x, int y,bool to); //�̹��� �׸���
    }