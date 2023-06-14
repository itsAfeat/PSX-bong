#pragma once
#define OTLEN 8

DISPENV dispenv[2];
DRAWENV drawenv[2];
int db;

u_long ot[2][OTLEN];

char pribuffer[2][32768];
char *nextpri;

void init()
{
    ResetGraph(0);
    CdInit();

    SetDefDispEnv(&dispenv[0], 0, 0, 320, 256);
    SetDefDispEnv(&dispenv[1], 0, 256, 320, 256);

    dispenv[0].screen.y = dispenv[1].screen.y = 24;

    SetVideoMode(MODE_PAL);

    SetDefDrawEnv(&drawenv[0], 0, 256, 320, 256);
    SetDefDrawEnv(&drawenv[1], 0, 0, 320, 256);

	setRGB0(&drawenv[0], 0, 0, 0);
	setRGB0(&drawenv[1], 0, 0, 0);

    drawenv[0].isbg = 1;
    drawenv[1].isbg = 1;

    PutDispEnv(&dispenv[0]);
    PutDrawEnv(&drawenv[0]);

    db = 0;
    nextpri = pribuffer[db];

	FntLoad(960, 0);
	FntOpen(0, 8, 320, 256, 0, 100);
}

char* loadFile(const char* filename)
{
    CdlFILE filePos;
    int numsecs;
    char* buff;

    if (CdSearchFile(&filePos, (char*) filename) == NULL)
    { printf("%s not found", filename); }
    else
    {
        printf("%s was found", filename);

        numsecs = (filePos.size+2047)/2048;
        buff = (char*)malloc(2048*numsecs);

        CdControl(CdlSetloc, (u_char*)&filePos.pos, 0);
        CdRead(numsecs, (u_long*)buff, CdlModeSpeed);
        CdReadSync(0, 0);
    }

    return buff;
}

int clamp(int i, int min, int max)
{
    const int t = i  < min ? min : i;
    return t > max ? max : t;
}

void flipAngle(int *a)
{
	int na = 360 - *a;
    na = na < 0 ? 360+na : na;
    *a = na;
}

void reflectAngle(int *a)
{
    if (*a < 180)
    {
        int na = 180 - *a;
        *a = na;
    }
    else
    {
        int na = 180 - *a;
        na = na < 0 ? 360+na : na;
        *a = na;
    }
}

int isBetween(int n, int min, int max)
{ return (n >= min && n <= max); }