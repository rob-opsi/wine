/*
 * Unit test suite for bitmaps
 *
 * Copyright 2004 Huw Davies
 * Copyright 2006 Dmitry Timoshkov
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdarg.h>
#include <assert.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "mmsystem.h"

#include "wine/test.h"

static BOOL is_win9x;

static void test_createdibitmap(void)
{
    HDC hdc, hdcmem;
    BITMAPINFOHEADER bmih;
    BITMAP bm;
    HBITMAP hbm, hbm_colour, hbm_old;
    INT screen_depth;

    hdc = GetDC(0);
    screen_depth = GetDeviceCaps(hdc, BITSPIXEL);
    memset(&bmih, 0, sizeof(bmih));
    bmih.biSize = sizeof(bmih);
    bmih.biWidth = 10;
    bmih.biHeight = 10;
    bmih.biPlanes = 1;
    bmih.biBitCount = 32;
    bmih.biCompression = BI_RGB;
 
    /* First create an un-initialised bitmap.  The depth of the bitmap
       should match that of the hdc and not that supplied in bmih.
    */

    /* First try 32 bits */
    hbm = CreateDIBitmap(hdc, &bmih, 0, NULL, NULL, 0);
    ok(hbm != NULL, "CreateDIBitmap failed\n");
    ok(GetObject(hbm, sizeof(bm), &bm), "GetObject failed\n");

    ok(bm.bmBitsPixel == screen_depth, "CreateDIBitmap created bitmap of incorrect depth %d != %d\n", bm.bmBitsPixel, screen_depth);
    DeleteObject(hbm);
    
    /* Then 16 */
    bmih.biBitCount = 16;
    hbm = CreateDIBitmap(hdc, &bmih, 0, NULL, NULL, 0);
    ok(hbm != NULL, "CreateDIBitmap failed\n");
    ok(GetObject(hbm, sizeof(bm), &bm), "GetObject failed\n");

    ok(bm.bmBitsPixel == screen_depth, "CreateDIBitmap created bitmap of incorrect depth %d != %d\n", bm.bmBitsPixel, screen_depth);
    DeleteObject(hbm);

    /* Then 1 */
    bmih.biBitCount = 1;
    hbm = CreateDIBitmap(hdc, &bmih, 0, NULL, NULL, 0);
    ok(hbm != NULL, "CreateDIBitmap failed\n");
    ok(GetObject(hbm, sizeof(bm), &bm), "GetObject failed\n");

    ok(bm.bmBitsPixel == screen_depth, "CreateDIBitmap created bitmap of incorrect depth %d != %d\n", bm.bmBitsPixel, screen_depth);
    DeleteObject(hbm);

    /* Now with a monochrome dc we expect a monochrome bitmap */
    hdcmem = CreateCompatibleDC(hdc);

    /* First try 32 bits */
    bmih.biBitCount = 32;
    hbm = CreateDIBitmap(hdcmem, &bmih, 0, NULL, NULL, 0);
    ok(hbm != NULL, "CreateDIBitmap failed\n");
    ok(GetObject(hbm, sizeof(bm), &bm), "GetObject failed\n");

    ok(bm.bmBitsPixel == 1, "CreateDIBitmap created bitmap of incorrect depth %d != %d\n", bm.bmBitsPixel, 1);
    DeleteObject(hbm);
    
    /* Then 16 */
    bmih.biBitCount = 16;
    hbm = CreateDIBitmap(hdcmem, &bmih, 0, NULL, NULL, 0);
    ok(hbm != NULL, "CreateDIBitmap failed\n");
    ok(GetObject(hbm, sizeof(bm), &bm), "GetObject failed\n");

    ok(bm.bmBitsPixel == 1, "CreateDIBitmap created bitmap of incorrect depth %d != %d\n", bm.bmBitsPixel, 1);
    DeleteObject(hbm);
    
    /* Then 1 */
    bmih.biBitCount = 1;
    hbm = CreateDIBitmap(hdcmem, &bmih, 0, NULL, NULL, 0);
    ok(hbm != NULL, "CreateDIBitmap failed\n");
    ok(GetObject(hbm, sizeof(bm), &bm), "GetObject failed\n");

    ok(bm.bmBitsPixel == 1, "CreateDIBitmap created bitmap of incorrect depth %d != %d\n", bm.bmBitsPixel, 1);
    DeleteObject(hbm);

    /* Now select a polychrome bitmap into the dc and we expect
       screen_depth bitmaps again */
    hbm_colour = CreateCompatibleBitmap(hdc, 1, 1);
    hbm_old = SelectObject(hdcmem, hbm_colour);

    /* First try 32 bits */
    bmih.biBitCount = 32;
    hbm = CreateDIBitmap(hdcmem, &bmih, 0, NULL, NULL, 0);
    ok(hbm != NULL, "CreateDIBitmap failed\n");
    ok(GetObject(hbm, sizeof(bm), &bm), "GetObject failed\n");

    ok(bm.bmBitsPixel == screen_depth, "CreateDIBitmap created bitmap of incorrect depth %d != %d\n", bm.bmBitsPixel, screen_depth);
    DeleteObject(hbm);
    
    /* Then 16 */
    bmih.biBitCount = 16;
    hbm = CreateDIBitmap(hdcmem, &bmih, 0, NULL, NULL, 0);
    ok(hbm != NULL, "CreateDIBitmap failed\n");
    ok(GetObject(hbm, sizeof(bm), &bm), "GetObject failed\n");

    ok(bm.bmBitsPixel == screen_depth, "CreateDIBitmap created bitmap of incorrect depth %d != %d\n", bm.bmBitsPixel, screen_depth);
    DeleteObject(hbm);
    
    /* Then 1 */
    bmih.biBitCount = 1;
    hbm = CreateDIBitmap(hdcmem, &bmih, 0, NULL, NULL, 0);
    ok(hbm != NULL, "CreateDIBitmap failed\n");
    ok(GetObject(hbm, sizeof(bm), &bm), "GetObject failed\n");

    ok(bm.bmBitsPixel == screen_depth, "CreateDIBitmap created bitmap of incorrect depth %d != %d\n", bm.bmBitsPixel, screen_depth);
    DeleteObject(hbm);

    SelectObject(hdcmem, hbm_old);
    DeleteObject(hbm_colour);
    DeleteDC(hdcmem);

    /* If hdc == 0 then we get a 1 bpp bitmap */
    if (!is_win9x) {
        bmih.biBitCount = 32;
        hbm = CreateDIBitmap(0, &bmih, 0, NULL, NULL, 0);
        ok(hbm != NULL, "CreateDIBitmap failed\n");
        ok(GetObject(hbm, sizeof(bm), &bm), "GetObject failed\n");

        ok(bm.bmBitsPixel == 1, "CreateDIBitmap created bitmap of incorrect depth %d != %d\n", bm.bmBitsPixel, 1);
        DeleteObject(hbm);
    }
    
    ReleaseDC(0, hdc);
}

#define test_color_todo(got, exp, txt, todo) \
    if (!todo && got != exp && screen_depth < 24) { \
      todo_wine ok(0, #txt " failed at %d-bit screen depth: got 0x%06x expected 0x%06x - skipping DIB tests\n", \
                   screen_depth, (UINT)got, (UINT)exp); \
      return; \
    } else if (todo) todo_wine { ok(got == exp, #txt " failed: got 0x%06x expected 0x%06x\n", (UINT)got, (UINT)exp); } \
    else ok(got == exp, #txt " failed: got 0x%06x expected 0x%06x\n", (UINT)got, (UINT)exp) \

#define test_color(hdc, color, exp, todo_setp, todo_getp) \
{ \
    COLORREF c; \
    c = SetPixel(hdc, 0, 0, color); \
    if (!is_win9x) { test_color_todo(c, exp, SetPixel, todo_setp); } \
    c = GetPixel(hdc, 0, 0); \
    test_color_todo(c, exp, GetPixel, todo_getp); \
}

static void test_dibsections(void)
{
    HDC hdc, hdcmem, hdcmem2;
    HBITMAP hdib, oldbm, hdib2, oldbm2;
    char bmibuf[sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD)];
    char bcibuf[sizeof(BITMAPCOREINFO) + 256 * sizeof(RGBTRIPLE)];
    BITMAPINFO *pbmi = (BITMAPINFO *)bmibuf;
    BITMAPCOREINFO *pbci = (BITMAPCOREINFO *)bcibuf;
    HBITMAP hcoredib;
    char coreBits[256];
    BYTE *bits;
    RGBQUAD rgb[256];
    int ret;
    char logpalbuf[sizeof(LOGPALETTE) + 256 * sizeof(PALETTEENTRY)];
    LOGPALETTE *plogpal = (LOGPALETTE*)logpalbuf;
    WORD *index;
    DWORD *bits32;
    HPALETTE hpal, oldpal;
    DIBSECTION dibsec;
    COLORREF c0, c1;
    int i;
    int screen_depth;
    MEMORY_BASIC_INFORMATION info;

    hdc = GetDC(0);
    screen_depth = GetDeviceCaps(hdc, BITSPIXEL) * GetDeviceCaps(hdc, PLANES);

    memset(pbmi, 0, sizeof(bmibuf));
    pbmi->bmiHeader.biSize = sizeof(pbmi->bmiHeader);
    pbmi->bmiHeader.biHeight = 100;
    pbmi->bmiHeader.biWidth = 512;
    pbmi->bmiHeader.biBitCount = 24;
    pbmi->bmiHeader.biPlanes = 1;
    pbmi->bmiHeader.biCompression = BI_RGB;

    SetLastError(0xdeadbeef);
    hdib = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
    ok(hdib != NULL, "CreateDIBSection error %ld\n", GetLastError());
    ok(GetObject(hdib, sizeof(DIBSECTION), &dibsec) != 0, "GetObject failed for DIBSection\n");
    ok(dibsec.dsBm.bmBits == bits, "dibsec.dsBits %p != bits %p\n", dibsec.dsBm.bmBits, bits);

    /* test the DIB memory */
    ok(VirtualQuery(bits, &info, sizeof(info)) == sizeof(info),
        "VirtualQuery failed\n");
    ok(info.BaseAddress == bits, "%p != %p\n", info.BaseAddress, bits);
    ok(info.AllocationBase == bits, "%p != %p\n", info.AllocationBase, bits);
    ok(info.AllocationProtect == PAGE_READWRITE, "%lx != PAGE_READWRITE\n", info.AllocationProtect);
    ok(info.RegionSize == 0x26000, "0x%lx != 0x26000\n", info.RegionSize);
    ok(info.State == MEM_COMMIT, "%lx != MEM_COMMIT\n", info.State);
    ok(info.Protect == PAGE_READWRITE, "%lx != PAGE_READWRITE\n", info.Protect);
    ok(info.Type == MEM_PRIVATE, "%lx != MEM_PRIVATE\n", info.Type);

    DeleteObject(hdib);

    pbmi->bmiHeader.biBitCount = 8;
    pbmi->bmiHeader.biCompression = BI_RLE8;
    SetLastError(0xdeadbeef);
    hdib = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
    ok(hdib == NULL, "CreateDIBSection should fail when asked to create a compressed DIB section\n");
    ok(GetLastError() == 0xdeadbeef, "wrong error %ld\n", GetLastError());

    pbmi->bmiHeader.biBitCount = 16;
    pbmi->bmiHeader.biCompression = BI_BITFIELDS;
    ((PDWORD)pbmi->bmiColors)[0] = 0xf800;
    ((PDWORD)pbmi->bmiColors)[1] = 0x07e0;
    ((PDWORD)pbmi->bmiColors)[2] = 0x001f;
    SetLastError(0xdeadbeef);
    hdib = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
    ok(hdib != NULL, "CreateDIBSection error %ld\n", GetLastError());

    /* test the DIB memory */
    ok(VirtualQuery(bits, &info, sizeof(info)) == sizeof(info),
        "VirtualQuery failed\n");
    ok(info.BaseAddress == bits, "%p != %p\n", info.BaseAddress, bits);
    ok(info.AllocationBase == bits, "%p != %p\n", info.AllocationBase, bits);
    ok(info.AllocationProtect == PAGE_READWRITE, "%lx != PAGE_READWRITE\n", info.AllocationProtect);
    ok(info.RegionSize == 0x19000, "0x%lx != 0x19000\n", info.RegionSize);
    ok(info.State == MEM_COMMIT, "%lx != MEM_COMMIT\n", info.State);
    ok(info.Protect == PAGE_READWRITE, "%lx != PAGE_READWRITE\n", info.Protect);
    ok(info.Type == MEM_PRIVATE, "%lx != MEM_PRIVATE\n", info.Type);

    DeleteObject(hdib);

    memset(pbmi, 0, sizeof(bmibuf));
    pbmi->bmiHeader.biSize = sizeof(pbmi->bmiHeader);
    pbmi->bmiHeader.biHeight = 16;
    pbmi->bmiHeader.biWidth = 16;
    pbmi->bmiHeader.biBitCount = 1;
    pbmi->bmiHeader.biPlanes = 1;
    pbmi->bmiHeader.biCompression = BI_RGB;
    pbmi->bmiColors[0].rgbRed = 0xff;
    pbmi->bmiColors[0].rgbGreen = 0;
    pbmi->bmiColors[0].rgbBlue = 0;
    pbmi->bmiColors[1].rgbRed = 0;
    pbmi->bmiColors[1].rgbGreen = 0;
    pbmi->bmiColors[1].rgbBlue = 0xff;

    hdib = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
    ok(hdib != NULL, "CreateDIBSection failed\n");
    ok(GetObject(hdib, sizeof(DIBSECTION), &dibsec) != 0, "GetObject failed for DIBSection\n");
    ok(dibsec.dsBmih.biClrUsed == 2,
        "created DIBSection: wrong biClrUsed field: %lu, should be: %u\n", dibsec.dsBmih.biClrUsed, 2);

    /* Test if the old BITMAPCOREINFO structure is supported */    
        
    pbci->bmciHeader.bcSize = sizeof(BITMAPCOREHEADER);
    pbci->bmciHeader.bcBitCount = 0;

    if (!is_win9x) {
        ret = GetDIBits(hdc, hdib, 0, 16, NULL, (BITMAPINFO*) pbci, DIB_RGB_COLORS);
        ok(ret, "GetDIBits doesn't work with a BITMAPCOREHEADER\n");
        ok((pbci->bmciHeader.bcWidth == 16) && (pbci->bmciHeader.bcHeight == 16)
            && (pbci->bmciHeader.bcBitCount == 1) && (pbci->bmciHeader.bcPlanes == 1),
        "GetDIBits did't fill in the BITMAPCOREHEADER structure properly\n");

        ret = GetDIBits(hdc, hdib, 0, 16, &coreBits, (BITMAPINFO*) pbci, DIB_RGB_COLORS);
        ok(ret, "GetDIBits doesn't work with a BITMAPCOREHEADER\n");
        ok((pbci->bmciColors[0].rgbtRed == 0xff) && (pbci->bmciColors[0].rgbtGreen == 0) &&
            (pbci->bmciColors[0].rgbtBlue == 0) && (pbci->bmciColors[1].rgbtRed == 0) &&
            (pbci->bmciColors[1].rgbtGreen == 0) && (pbci->bmciColors[1].rgbtBlue == 0xff),
            "The color table has not been translated to the old BITMAPCOREINFO format\n");

        hcoredib = CreateDIBSection(hdc, (BITMAPINFO*) pbci, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
        ok(hcoredib != NULL, "CreateDIBSection failed with a BITMAPCOREINFO\n");

        ZeroMemory(pbci->bmciColors, 256 * sizeof(RGBTRIPLE));
        ret = GetDIBits(hdc, hcoredib, 0, 16, &coreBits, (BITMAPINFO*) pbci, DIB_RGB_COLORS);
        ok(ret, "GetDIBits doesn't work with a BITMAPCOREHEADER\n");
        ok((pbci->bmciColors[0].rgbtRed == 0xff) && (pbci->bmciColors[0].rgbtGreen == 0) &&
            (pbci->bmciColors[0].rgbtBlue == 0) && (pbci->bmciColors[1].rgbtRed == 0) &&
            (pbci->bmciColors[1].rgbtGreen == 0) && (pbci->bmciColors[1].rgbtBlue == 0xff),
            "The color table has not been translated to the old BITMAPCOREINFO format\n");

        DeleteObject(hcoredib);
    }

    hdcmem = CreateCompatibleDC(hdc);
    oldbm = SelectObject(hdcmem, hdib);

    ret = GetDIBColorTable(hdcmem, 0, 2, rgb);
    ok(ret == 2, "GetDIBColorTable returned %d\n", ret);
    ok(!memcmp(rgb, pbmi->bmiColors, 2 * sizeof(RGBQUAD)),
       "GetDIBColorTable returns table 0: r%02x g%02x b%02x res%02x 1: r%02x g%02x b%02x res%02x\n",
       rgb[0].rgbRed, rgb[0].rgbGreen, rgb[0].rgbBlue, rgb[0].rgbReserved,
       rgb[1].rgbRed, rgb[1].rgbGreen, rgb[1].rgbBlue, rgb[1].rgbReserved);

    c0 = RGB(pbmi->bmiColors[0].rgbRed, pbmi->bmiColors[0].rgbGreen, pbmi->bmiColors[0].rgbBlue);
    c1 = RGB(pbmi->bmiColors[1].rgbRed, pbmi->bmiColors[1].rgbGreen, pbmi->bmiColors[1].rgbBlue);

    test_color(hdcmem, DIBINDEX(0), c0, 0, 1);
    test_color(hdcmem, DIBINDEX(1), c1, 0, 1);
    test_color(hdcmem, DIBINDEX(2), c0, 1, 1);
    test_color(hdcmem, PALETTEINDEX(0), c0, 1, 1);
    test_color(hdcmem, PALETTEINDEX(1), c0, 1, 1);
    test_color(hdcmem, PALETTEINDEX(2), c0, 1, 1);
    test_color(hdcmem, PALETTERGB(pbmi->bmiColors[0].rgbRed, pbmi->bmiColors[0].rgbGreen,
        pbmi->bmiColors[0].rgbBlue), c0, 1, 1);
    test_color(hdcmem, PALETTERGB(pbmi->bmiColors[1].rgbRed, pbmi->bmiColors[1].rgbGreen,
        pbmi->bmiColors[1].rgbBlue), c1, 1, 1);
    test_color(hdcmem, PALETTERGB(0, 0, 0), c0, 1, 1);
    test_color(hdcmem, PALETTERGB(0xff, 0xff, 0xff), c0, 1, 1);
    test_color(hdcmem, PALETTERGB(0, 0, 0xfe), c1, 1, 1);

    SelectObject(hdcmem, oldbm);
    DeleteObject(hdib);

    pbmi->bmiColors[0].rgbRed = 0xff;
    pbmi->bmiColors[0].rgbGreen = 0xff;
    pbmi->bmiColors[0].rgbBlue = 0xff;
    pbmi->bmiColors[1].rgbRed = 0;
    pbmi->bmiColors[1].rgbGreen = 0;
    pbmi->bmiColors[1].rgbBlue = 0;

    hdib = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
    ok(hdib != NULL, "CreateDIBSection failed\n");
    oldbm = SelectObject(hdcmem, hdib);

    ret = GetDIBColorTable(hdcmem, 0, 2, rgb);
    ok(ret == 2, "GetDIBColorTable returned %d\n", ret);
    ok(!memcmp(rgb, pbmi->bmiColors, 2 * sizeof(RGBQUAD)),
       "GetDIBColorTable returns table 0: r%02x g%02x b%02x res%02x 1: r%02x g%02x b%02x res%02x\n",
       rgb[0].rgbRed, rgb[0].rgbGreen, rgb[0].rgbBlue, rgb[0].rgbReserved,
       rgb[1].rgbRed, rgb[1].rgbGreen, rgb[1].rgbBlue, rgb[1].rgbReserved);

    SelectObject(hdcmem, oldbm);
    DeleteObject(hdib);

    pbmi->bmiHeader.biBitCount = 4;
    for (i = 0; i < 16; i++) {
        pbmi->bmiColors[i].rgbRed = i;
        pbmi->bmiColors[i].rgbGreen = 16-i;
        pbmi->bmiColors[i].rgbBlue = 0;
    }
    hdib = CreateDIBSection(hdcmem, pbmi, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
    ok(hdib != NULL, "CreateDIBSection failed\n");
    ok(GetObject(hdib, sizeof(DIBSECTION), &dibsec) != 0, "GetObject failed for DIB Section\n");
    ok(dibsec.dsBmih.biClrUsed == 16,
       "created DIBSection: wrong biClrUsed field: %lu, should be: %u\n", dibsec.dsBmih.biClrUsed, 16);
    DeleteObject(hdib);

    pbmi->bmiHeader.biBitCount = 8;

    for (i = 0; i < 128; i++) {
        pbmi->bmiColors[i].rgbRed = 255 - i * 2;
        pbmi->bmiColors[i].rgbGreen = i * 2;
        pbmi->bmiColors[i].rgbBlue = 0;
        pbmi->bmiColors[255 - i].rgbRed = 0;
        pbmi->bmiColors[255 - i].rgbGreen = i * 2;
        pbmi->bmiColors[255 - i].rgbBlue = 255 - i * 2;
    }
    hdib = CreateDIBSection(hdcmem, pbmi, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
    ok(hdib != NULL, "CreateDIBSection failed\n");
    ok(GetObject(hdib, sizeof(DIBSECTION), &dibsec) != 0, "GetObject failed for DIB Section\n");
    ok(dibsec.dsBmih.biClrUsed == 256,
        "created DIBSection: wrong biClrUsed field: %lu, should be: %u\n", dibsec.dsBmih.biClrUsed, 256);

    oldbm = SelectObject(hdcmem, hdib);

    for (i = 0; i < 256; i++) {
        test_color(hdcmem, DIBINDEX(i), 
            RGB(pbmi->bmiColors[i].rgbRed, pbmi->bmiColors[i].rgbGreen, pbmi->bmiColors[i].rgbBlue), 0, 0);
        test_color(hdcmem, PALETTERGB(pbmi->bmiColors[i].rgbRed, pbmi->bmiColors[i].rgbGreen, pbmi->bmiColors[i].rgbBlue), 
            RGB(pbmi->bmiColors[i].rgbRed, pbmi->bmiColors[i].rgbGreen, pbmi->bmiColors[i].rgbBlue), 0, 0);
    }

    SelectObject(hdcmem, oldbm);
    DeleteObject(hdib);

    pbmi->bmiHeader.biBitCount = 1;

    /* Now create a palette and a palette indexed dib section */
    memset(plogpal, 0, sizeof(logpalbuf));
    plogpal->palVersion = 0x300;
    plogpal->palNumEntries = 2;
    plogpal->palPalEntry[0].peRed = 0xff;
    plogpal->palPalEntry[0].peBlue = 0xff;
    plogpal->palPalEntry[1].peGreen = 0xff;

    index = (WORD*)pbmi->bmiColors;
    *index++ = 0;
    *index = 1;
    hpal = CreatePalette(plogpal);
    ok(hpal != NULL, "CreatePalette failed\n");
    oldpal = SelectPalette(hdc, hpal, TRUE);
    hdib = CreateDIBSection(hdc, pbmi, DIB_PAL_COLORS, (void**)&bits, NULL, 0);
    ok(hdib != NULL, "CreateDIBSection failed\n");
    ok(GetObject(hdib, sizeof(DIBSECTION), &dibsec) != 0, "GetObject failed for DIB Section\n");
    ok(dibsec.dsBmih.biClrUsed == 2,
        "created DIBSection: wrong biClrUsed field: %lu, should be: %u\n", dibsec.dsBmih.biClrUsed, 2);

    /* The colour table has already been grabbed from the dc, so we select back the
       old palette */

    SelectPalette(hdc, oldpal, TRUE);
    oldbm = SelectObject(hdcmem, hdib);
    oldpal = SelectPalette(hdcmem, hpal, TRUE);

    ret = GetDIBColorTable(hdcmem, 0, 2, rgb);
    ok(ret == 2, "GetDIBColorTable returned %d\n", ret);
    ok(rgb[0].rgbRed == 0xff && rgb[0].rgbBlue == 0xff && rgb[0].rgbGreen == 0 &&
       rgb[1].rgbRed == 0    && rgb[1].rgbBlue == 0    && rgb[1].rgbGreen == 0xff,
       "GetDIBColorTable returns table 0: r%02x g%02x b%02x res%02x 1: r%02x g%02x b%02x res%02x\n",
       rgb[0].rgbRed, rgb[0].rgbGreen, rgb[0].rgbBlue, rgb[0].rgbReserved,
       rgb[1].rgbRed, rgb[1].rgbGreen, rgb[1].rgbBlue, rgb[1].rgbReserved);

    c0 = RGB(plogpal->palPalEntry[0].peRed, plogpal->palPalEntry[0].peGreen, plogpal->palPalEntry[0].peBlue);
    c1 = RGB(plogpal->palPalEntry[1].peRed, plogpal->palPalEntry[1].peGreen, plogpal->palPalEntry[1].peBlue);

    test_color(hdcmem, DIBINDEX(0), c0, 0, 1);
    test_color(hdcmem, DIBINDEX(1), c1, 0, 1);
    test_color(hdcmem, DIBINDEX(2), c0, 1, 1);
    test_color(hdcmem, PALETTEINDEX(0), c0, 0, 1);
    test_color(hdcmem, PALETTEINDEX(1), c1, 0, 1);
    test_color(hdcmem, PALETTEINDEX(2), c0, 1, 1);
    test_color(hdcmem, PALETTERGB(plogpal->palPalEntry[0].peRed, plogpal->palPalEntry[0].peGreen,
        plogpal->palPalEntry[0].peBlue), c0, 1, 1);
    test_color(hdcmem, PALETTERGB(plogpal->palPalEntry[1].peRed, plogpal->palPalEntry[1].peGreen,
        plogpal->palPalEntry[1].peBlue), c1, 1, 1);
    test_color(hdcmem, PALETTERGB(0, 0, 0), c1, 1, 1);
    test_color(hdcmem, PALETTERGB(0xff, 0xff, 0xff), c0, 1, 1);
    test_color(hdcmem, PALETTERGB(0, 0, 0xfe), c0, 1, 1);
    test_color(hdcmem, PALETTERGB(0, 1, 0), c1, 1, 1);
    test_color(hdcmem, PALETTERGB(0x3f, 0, 0x3f), c1, 1, 1);
    test_color(hdcmem, PALETTERGB(0x40, 0, 0x40), c0, 1, 1);

    /* Bottom and 2nd row from top green, everything else magenta */
    bits[0] = bits[1] = 0xff;
    bits[13 * 4] = bits[13*4 + 1] = 0xff;


    pbmi->bmiHeader.biBitCount = 32;

    hdib2 = CreateDIBSection(NULL, pbmi, DIB_RGB_COLORS, (void **)&bits32, NULL, 0);
    ok(hdib2 != NULL, "CreateDIBSection failed\n");
    hdcmem2 = CreateCompatibleDC(hdc);
    oldbm2 = SelectObject(hdcmem2, hdib2);

    BitBlt(hdcmem2, 0, 0, 16,16, hdcmem, 0, 0, SRCCOPY);

    ok(bits32[0] == 0xff00, "lower left pixel is %08lx\n", bits32[0]);
    ok(bits32[17] == 0xff00ff, "bottom but one, left pixel is %08lx\n", bits32[17]);

    SelectObject(hdcmem2, oldbm2);
    DeleteObject(hdib2);

    SelectObject(hdcmem, oldbm);
    SelectObject(hdcmem, oldpal);
    DeleteObject(hdib);
    DeleteObject(hpal);


    pbmi->bmiHeader.biBitCount = 8;

    memset(plogpal, 0, sizeof(logpalbuf));
    plogpal->palVersion = 0x300;
    plogpal->palNumEntries = 256;

    for (i = 0; i < 128; i++) {
        plogpal->palPalEntry[i].peRed = 255 - i * 2;
        plogpal->palPalEntry[i].peBlue = i * 2;
        plogpal->palPalEntry[i].peGreen = 0;
        plogpal->palPalEntry[255 - i].peRed = 0;
        plogpal->palPalEntry[255 - i].peGreen = i * 2;
        plogpal->palPalEntry[255 - i].peBlue = 255 - i * 2;
    }

    index = (WORD*)pbmi->bmiColors;
    for (i = 0; i < 256; i++) {
        *index++ = i;
    }

    hpal = CreatePalette(plogpal);
    ok(hpal != NULL, "CreatePalette failed\n");
    oldpal = SelectPalette(hdc, hpal, TRUE);
    hdib = CreateDIBSection(hdc, pbmi, DIB_PAL_COLORS, (void**)&bits, NULL, 0);
    ok(hdib != NULL, "CreateDIBSection failed\n");
    ok(GetObject(hdib, sizeof(DIBSECTION), &dibsec) != 0, "GetObject failed for DIB Section\n");
    ok(dibsec.dsBmih.biClrUsed == 256,
        "created DIBSection: wrong biClrUsed field: %lu, should be: %u\n", dibsec.dsBmih.biClrUsed, 256);

    SelectPalette(hdc, oldpal, TRUE);
    oldbm = SelectObject(hdcmem, hdib);
    oldpal = SelectPalette(hdcmem, hpal, TRUE);

    ret = GetDIBColorTable(hdcmem, 0, 256, rgb);
    ok(ret == 256, "GetDIBColorTable returned %d\n", ret);
    for (i = 0; i < 256; i++) {
        ok(rgb[i].rgbRed == plogpal->palPalEntry[i].peRed && 
            rgb[i].rgbBlue == plogpal->palPalEntry[i].peBlue && 
            rgb[i].rgbGreen == plogpal->palPalEntry[i].peGreen, 
            "GetDIBColorTable returns table %d: r%02x g%02x b%02x res%02x\n",
            i, rgb[i].rgbRed, rgb[i].rgbGreen, rgb[i].rgbBlue, rgb[i].rgbReserved);
    }

    for (i = 0; i < 256; i++) {
        test_color(hdcmem, DIBINDEX(i), 
            RGB(plogpal->palPalEntry[i].peRed, plogpal->palPalEntry[i].peGreen, plogpal->palPalEntry[i].peBlue), 0, 0);
        test_color(hdcmem, PALETTEINDEX(i), 
            RGB(plogpal->palPalEntry[i].peRed, plogpal->palPalEntry[i].peGreen, plogpal->palPalEntry[i].peBlue), 0, 0);
        test_color(hdcmem, PALETTERGB(plogpal->palPalEntry[i].peRed, plogpal->palPalEntry[i].peGreen, plogpal->palPalEntry[i].peBlue), 
            RGB(plogpal->palPalEntry[i].peRed, plogpal->palPalEntry[i].peGreen, plogpal->palPalEntry[i].peBlue), 0, 0);
    }

    SelectPalette(hdcmem, oldpal, TRUE);
    SelectObject(hdcmem, oldbm);
    DeleteObject(hdib);
    DeleteObject(hpal);


    DeleteDC(hdcmem);
    ReleaseDC(0, hdc);
}    

void test_mono_dibsection(void)
{
    HDC hdc, memdc;
    HBITMAP old_bm, mono_ds;
    char bmibuf[sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD)];
    BITMAPINFO *pbmi = (BITMAPINFO *)bmibuf;
    BYTE bits[10 * 4];
    BYTE *ds_bits;
    int num;

    hdc = GetDC(0);

    memdc = CreateCompatibleDC(hdc);

    memset(pbmi, 0, sizeof(bmibuf));
    pbmi->bmiHeader.biSize = sizeof(pbmi->bmiHeader);
    pbmi->bmiHeader.biHeight = 10;
    pbmi->bmiHeader.biWidth = 10;
    pbmi->bmiHeader.biBitCount = 1;
    pbmi->bmiHeader.biPlanes = 1;
    pbmi->bmiHeader.biCompression = BI_RGB;
    pbmi->bmiColors[0].rgbRed = 0xff;
    pbmi->bmiColors[0].rgbGreen = 0xff;
    pbmi->bmiColors[0].rgbBlue = 0xff;
    pbmi->bmiColors[1].rgbRed = 0x0;
    pbmi->bmiColors[1].rgbGreen = 0x0;
    pbmi->bmiColors[1].rgbBlue = 0x0;

    /*
     * First dib section is 'inverted' ie color[0] is white, color[1] is black
     */

    mono_ds = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, (void**)&ds_bits, NULL, 0);
    ok(mono_ds != NULL, "CreateDIBSection rets NULL\n");
    old_bm = SelectObject(memdc, mono_ds);

    /* black border, white interior */
    Rectangle(memdc, 0, 0, 10, 10);
    ok(ds_bits[0] == 0xff, "out_bits %02x\n", ds_bits[0]);
    ok(ds_bits[4] == 0x80, "out_bits %02x\n", ds_bits[4]);

    /* SetDIBitsToDevice with an inverted bmi -> inverted dib section */

    memset(bits, 0, sizeof(bits));
    bits[0] = 0xaa;

    SetDIBitsToDevice(memdc, 0, 0, 10, 10, 0, 0, 0, 10, bits, pbmi, DIB_RGB_COLORS);
    ok(ds_bits[0] == 0xaa, "out_bits %02x\n", ds_bits[0]);

    /* SetDIBitsToDevice with a normal bmi -> inverted dib section */

    pbmi->bmiColors[0].rgbRed = 0x0;
    pbmi->bmiColors[0].rgbGreen = 0x0;
    pbmi->bmiColors[0].rgbBlue = 0x0;
    pbmi->bmiColors[1].rgbRed = 0xff;
    pbmi->bmiColors[1].rgbGreen = 0xff;
    pbmi->bmiColors[1].rgbBlue = 0xff;

    SetDIBitsToDevice(memdc, 0, 0, 10, 10, 0, 0, 0, 10, bits, pbmi, DIB_RGB_COLORS);
    ok(ds_bits[0] == 0x55, "out_bits %02x\n", ds_bits[0]);

    SelectObject(memdc, old_bm);
    DeleteObject(mono_ds);

    /*
     * Next dib section is 'normal' ie color[0] is black, color[1] is white
     */

    pbmi->bmiColors[0].rgbRed = 0x0;
    pbmi->bmiColors[0].rgbGreen = 0x0;
    pbmi->bmiColors[0].rgbBlue = 0x0;
    pbmi->bmiColors[1].rgbRed = 0xff;
    pbmi->bmiColors[1].rgbGreen = 0xff;
    pbmi->bmiColors[1].rgbBlue = 0xff;

    mono_ds = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, (void**)&ds_bits, NULL, 0);
    ok(mono_ds != NULL, "CreateDIBSection rets NULL\n");
    old_bm = SelectObject(memdc, mono_ds);

    /* black border, white interior */
    Rectangle(memdc, 0, 0, 10, 10);
    ok(ds_bits[0] == 0x00, "out_bits %02x\n", ds_bits[0]);
    ok(ds_bits[4] == 0x7f, "out_bits %02x\n", ds_bits[4]);

    /* SetDIBitsToDevice with a normal bmi -> normal dib section */

    SetDIBitsToDevice(memdc, 0, 0, 10, 10, 0, 0, 0, 10, bits, pbmi, DIB_RGB_COLORS);
    ok(ds_bits[0] == 0xaa, "out_bits %02x\n", ds_bits[0]);

    /* SetDIBitsToDevice with a inverted bmi -> normal dib section */

    pbmi->bmiColors[0].rgbRed = 0xff;
    pbmi->bmiColors[0].rgbGreen = 0xff;
    pbmi->bmiColors[0].rgbBlue = 0xff;
    pbmi->bmiColors[1].rgbRed = 0x0;
    pbmi->bmiColors[1].rgbGreen = 0x0;
    pbmi->bmiColors[1].rgbBlue = 0x0;

    SetDIBitsToDevice(memdc, 0, 0, 10, 10, 0, 0, 0, 10, bits, pbmi, DIB_RGB_COLORS);
    ok(ds_bits[0] == 0x55, "out_bits %02x\n", ds_bits[0]);

    /*
     * Take that 'normal' dibsection and change its colour table to an 'inverted' one
     */

    pbmi->bmiColors[0].rgbRed = 0xff;
    pbmi->bmiColors[0].rgbGreen = 0xff;
    pbmi->bmiColors[0].rgbBlue = 0xff;
    pbmi->bmiColors[1].rgbRed = 0x0;
    pbmi->bmiColors[1].rgbGreen = 0x0;
    pbmi->bmiColors[1].rgbBlue = 0x0;
    num = SetDIBColorTable(memdc, 0, 2, pbmi->bmiColors);
    ok(num == 2, "num = %d\n", num);

    /* black border, white interior */
    Rectangle(memdc, 0, 0, 10, 10);
todo_wine {
    ok(ds_bits[0] == 0xff, "out_bits %02x\n", ds_bits[0]);
    ok(ds_bits[4] == 0x80, "out_bits %02x\n", ds_bits[4]);
 }
    /* SetDIBitsToDevice with an inverted bmi -> inverted dib section */

    memset(bits, 0, sizeof(bits));
    bits[0] = 0xaa;

    SetDIBitsToDevice(memdc, 0, 0, 10, 10, 0, 0, 0, 10, bits, pbmi, DIB_RGB_COLORS);
    ok(ds_bits[0] == 0xaa, "out_bits %02x\n", ds_bits[0]);

    /* SetDIBitsToDevice with a normal bmi -> inverted dib section */

    pbmi->bmiColors[0].rgbRed = 0x0;
    pbmi->bmiColors[0].rgbGreen = 0x0;
    pbmi->bmiColors[0].rgbBlue = 0x0;
    pbmi->bmiColors[1].rgbRed = 0xff;
    pbmi->bmiColors[1].rgbGreen = 0xff;
    pbmi->bmiColors[1].rgbBlue = 0xff;

    SetDIBitsToDevice(memdc, 0, 0, 10, 10, 0, 0, 0, 10, bits, pbmi, DIB_RGB_COLORS);
    ok(ds_bits[0] == 0x55, "out_bits %02x\n", ds_bits[0]);

    SelectObject(memdc, old_bm);
    DeleteObject(mono_ds);

    /*
     * Now a dib section with a strange colour map just for fun.  This behaves just like an inverted one.
     */
 
    pbmi->bmiColors[0].rgbRed = 0xff;
    pbmi->bmiColors[0].rgbGreen = 0x0;
    pbmi->bmiColors[0].rgbBlue = 0x0;
    pbmi->bmiColors[1].rgbRed = 0xfe;
    pbmi->bmiColors[1].rgbGreen = 0x0;
    pbmi->bmiColors[1].rgbBlue = 0x0;

    mono_ds = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, (void**)&ds_bits, NULL, 0);
    ok(mono_ds != NULL, "CreateDIBSection rets NULL\n");
    old_bm = SelectObject(memdc, mono_ds);

    /* black border, white interior */
    Rectangle(memdc, 0, 0, 10, 10);
    ok(ds_bits[0] == 0xff, "out_bits %02x\n", ds_bits[0]);
    ok(ds_bits[4] == 0x80, "out_bits %02x\n", ds_bits[4]);

    /* SetDIBitsToDevice with a normal bmi -> inverted dib section */

    pbmi->bmiColors[0].rgbRed = 0x0;
    pbmi->bmiColors[0].rgbGreen = 0x0;
    pbmi->bmiColors[0].rgbBlue = 0x0;
    pbmi->bmiColors[1].rgbRed = 0xff;
    pbmi->bmiColors[1].rgbGreen = 0xff;
    pbmi->bmiColors[1].rgbBlue = 0xff;

    SetDIBitsToDevice(memdc, 0, 0, 10, 10, 0, 0, 0, 10, bits, pbmi, DIB_RGB_COLORS);
    ok(ds_bits[0] == 0x55, "out_bits %02x\n", ds_bits[0]);

    /* SetDIBitsToDevice with a inverted bmi -> inverted dib section */

    pbmi->bmiColors[0].rgbRed = 0xff;
    pbmi->bmiColors[0].rgbGreen = 0xff;
    pbmi->bmiColors[0].rgbBlue = 0xff;
    pbmi->bmiColors[1].rgbRed = 0x0;
    pbmi->bmiColors[1].rgbGreen = 0x0;
    pbmi->bmiColors[1].rgbBlue = 0x0;

    SetDIBitsToDevice(memdc, 0, 0, 10, 10, 0, 0, 0, 10, bits, pbmi, DIB_RGB_COLORS);
    ok(ds_bits[0] == 0xaa, "out_bits %02x\n", ds_bits[0]);

    SelectObject(memdc, old_bm);
    DeleteObject(mono_ds);

    DeleteDC(memdc);
    ReleaseDC(0, hdc);
}

static void test_bitmap(void)
{
    char buf[256], buf_cmp[256];
    HBITMAP hbmp, hbmp_old;
    HDC hdc;
    BITMAP bm;
    INT ret;

    hdc = CreateCompatibleDC(0);
    assert(hdc != 0);

    hbmp = CreateBitmap(15, 15, 1, 1, NULL);
    assert(hbmp != NULL);

    ret = GetObject(hbmp, sizeof(bm), &bm);
    ok(ret == sizeof(bm), "%d != %d\n", ret, sizeof(bm));

    ok(bm.bmType == 0, "wrong bm.bmType %d\n", bm.bmType);
    ok(bm.bmWidth == 15, "wrong bm.bmWidth %d\n", bm.bmWidth);
    ok(bm.bmHeight == 15, "wrong bm.bmHeight %d\n", bm.bmHeight);
    ok(bm.bmWidthBytes == 2, "wrong bm.bmWidthBytes %d\n", bm.bmWidthBytes);
    ok(bm.bmPlanes == 1, "wrong bm.bmPlanes %d\n", bm.bmPlanes);
    ok(bm.bmBitsPixel == 1, "wrong bm.bmBitsPixel %d\n", bm.bmBitsPixel);
    ok(bm.bmBits == NULL, "wrong bm.bmBits %p\n", bm.bmBits);

    assert(sizeof(buf) >= bm.bmWidthBytes * bm.bmHeight);
    assert(sizeof(buf) == sizeof(buf_cmp));

    memset(buf_cmp, 0xAA, sizeof(buf_cmp));
    memset(buf_cmp, 0, bm.bmWidthBytes * bm.bmHeight);

    memset(buf, 0xAA, sizeof(buf));
    ret = GetBitmapBits(hbmp, sizeof(buf), buf);
    ok(ret == bm.bmWidthBytes * bm.bmHeight, "%d != %d\n", ret, bm.bmWidthBytes * bm.bmHeight);
    ok(!memcmp(buf, buf_cmp, sizeof(buf)), "buffers do not match\n");

    hbmp_old = SelectObject(hdc, hbmp);

    ret = GetObject(hbmp, sizeof(bm), &bm);
    ok(ret == sizeof(bm), "%d != %d\n", ret, sizeof(bm));

    ok(bm.bmType == 0, "wrong bm.bmType %d\n", bm.bmType);
    ok(bm.bmWidth == 15, "wrong bm.bmWidth %d\n", bm.bmWidth);
    ok(bm.bmHeight == 15, "wrong bm.bmHeight %d\n", bm.bmHeight);
    ok(bm.bmWidthBytes == 2, "wrong bm.bmWidthBytes %d\n", bm.bmWidthBytes);
    ok(bm.bmPlanes == 1, "wrong bm.bmPlanes %d\n", bm.bmPlanes);
    ok(bm.bmBitsPixel == 1, "wrong bm.bmBitsPixel %d\n", bm.bmBitsPixel);
    ok(bm.bmBits == NULL, "wrong bm.bmBits %p\n", bm.bmBits);

    memset(buf, 0xAA, sizeof(buf));
    ret = GetBitmapBits(hbmp, sizeof(buf), buf);
    ok(ret == bm.bmWidthBytes * bm.bmHeight, "%d != %d\n", ret, bm.bmWidthBytes * bm.bmHeight);
    ok(!memcmp(buf, buf_cmp, sizeof(buf)), "buffers do not match\n");

    hbmp_old = SelectObject(hdc, hbmp_old);
    ok(hbmp_old == hbmp, "wrong old bitmap %p\n", hbmp_old);

    /* test various buffer sizes for GetObject */
    ret = GetObject(hbmp, sizeof(bm) * 2, &bm);
    ok(ret == sizeof(bm), "%d != %d\n", ret, sizeof(bm));

    ret = GetObject(hbmp, sizeof(bm) / 2, &bm);
    ok(ret == 0, "%d != 0\n", ret);

    ret = GetObject(hbmp, 0, &bm);
    ok(ret == 0, "%d != 0\n", ret);

    ret = GetObject(hbmp, 1, &bm);
    ok(ret == 0, "%d != 0\n", ret);

    DeleteObject(hbmp);
    DeleteDC(hdc);
}

START_TEST(bitmap)
{
    is_win9x = GetWindowLongPtrW(GetDesktopWindow(), GWLP_WNDPROC) == 0;

    test_createdibitmap();
    test_dibsections();
    test_mono_dibsection();
    test_bitmap();
}
