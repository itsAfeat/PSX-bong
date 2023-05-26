#pragma once

typedef struct {
	u_short tpage;
	u_short clut;
    u_char  w,h;    // The width and height of the characters
    u_int   mIndex; // The max index for the UV calculations
} FONT_SPRITE;

FONT_SPRITE font8x16;

void getFont(TIM_IMAGE* tim, FONT_SPRITE* fontSprt, int w, int h, int mIndex)
{
	fontSprt->tpage = getTPage(tim->mode&0x3, 0, tim->prect->x, tim->prect->y);
	if (tim->mode&0x8) { fontSprt->clut = getClut(tim->crect->x, tim->crect->y); }

    fontSprt->w = w;
    fontSprt->h = h;
    fontSprt->mIndex = mIndex;
}

char* printFont(int x, int y, u_long* ot, char* pri, const char* string, FONT_SPRITE* fontSprt)
{
    int start_x = x;
    int customChar = 0;

    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == '\n')
        { x = start_x; y += fontSprt->h; continue; }
        if (string[i] == '~' && !customChar)
        { customChar = 1; continue; }

        int u_offset, v_offset;
        int index = ((int)string[i])-32;

        if (index != 0)
        {
            if (customChar && string[i] != '~')
            { index = ((int)'~'-32)+(string[i]-'0'); customChar = 0; }

            int u = index-(((int)index/fontSprt->mIndex)*fontSprt->mIndex);
            u_offset = u == 0 ? fontSprt->mIndex : u;
            v_offset = (int)index/fontSprt->mIndex;

            if (index%fontSprt->mIndex == 0) { v_offset--; }
            u_offset--;
        }
        else
        { x += fontSprt->w; continue; }

        SPRT* sprt;
        DR_TPAGE* tpage;

        sprt = (SPRT*)pri;
        setSprt(sprt);
        setXY0(sprt, x, y);
        setWH(sprt, fontSprt->w, fontSprt->h);
        setUV0(sprt, fontSprt->w*u_offset, fontSprt->h*v_offset);
        setShadeTex(sprt, 1);
        sprt->clut = fontSprt->clut;

        addPrim(ot, sprt);
        pri += sizeof(SPRT);

        tpage = (DR_TPAGE*)pri;
        setDrawTPage(tpage, 0, 1, fontSprt->tpage);
        addPrim(ot, tpage);
        pri += sizeof(DR_TPAGE);

        x += fontSprt->w;
    }

    return pri;
}