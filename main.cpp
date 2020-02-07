#include <iostream>
#include <vector>
#include <Windows.h>

int main()
{
    int x = 0;
    int y = 0;
    int w = 100;
    int h = 100;

    int screen_w = GetSystemMetrics(SM_CXFULLSCREEN);
    int screen_h = GetSystemMetrics(SM_CYFULLSCREEN);

    HDC hdc = GetDC(HWND_DESKTOP);
    HBITMAP hbitmap = CreateCompatibleBitmap(hdc, screen_w, screen_h);
    HDC memdc = CreateCompatibleDC(hdc);
    HGDIOBJ oldbmp = SelectObject(memdc, hbitmap);
    BitBlt(memdc, 0, 0, w, h, hdc, x, y, CAPTUREBLT | SRCCOPY);
    SelectObject(memdc, oldbmp);

    BITMAPINFOHEADER infohdr = { sizeof(infohdr), w, h, 1, 32 };
    int size = w * h * 4;
    std::vector<BYTE> bits(size);
    int res = GetDIBits(hdc, hbitmap, 0, h, &bits[0],
        (BITMAPINFO*)&infohdr, DIB_RGB_COLORS);
    if (res != h)
    {
        std::cout << "error\n";
        return 0;
    }

    BYTE* ptr = bits.data();
    //for(y = 0; y < h; y++)
    for (y = h - 1; y >= 0; y--) //bitmaps bits start from bottom, not top
    {
        for (x = 0; x < w; x++)
        {
            BYTE blu = ptr[0];
            BYTE grn = ptr[1];
            BYTE red = ptr[2];
            ptr += 4;
        }
    }

    SelectObject(memdc, oldbmp);
    DeleteObject(hbitmap);
    ReleaseDC(HWND_DESKTOP, hdc);
    DeleteDC(memdc);

    return 0;
}