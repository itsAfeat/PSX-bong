#pragma once

typedef struct {
	u_short tpage;		// The sprites Tpage
	u_short clut;		// The sprites CLUT
	u_char 	u,v;		// The U,V offset
	u_int	x,y;		// The position
	u_char 	w,h;		// The width and height
	int		semiTrans;	// The transparency bool
} SPRITE;

SPRITE paul_sprite;
SPRITE puyi_sprite;
SPRITE title_sprite;

char *TIMs[] = {"\\PAUL.TIM;1", "\\PUYI.TIM;1", "\\TITLE.TIM;1"};
SPRITE **SPRITEs;


void display()
{
    DrawSync(0);
    VSync(0);

    PutDispEnv(&dispenv[db]);
    PutDrawEnv(&drawenv[db]);

    SetDispMask(1);
    DrawOTag(ot[db]+OTLEN-1);

    db = !db;
    nextpri = pribuffer[db];
}

void getSprite(TIM_IMAGE* tim, SPRITE* sprite)
{
	sprite->tpage = getTPage(tim->mode&0x3, 0, tim->prect->x, tim->prect->y);
	if (tim->mode&0x8) { sprite->clut = getClut(tim->crect->x, tim->crect->y); }

	sprite->w = tim->prect->w<<(2-tim->mode&0x3);
	sprite->h = tim->prect->h;

	sprite->u = (tim->prect->x&0x3f)<<(2-tim->mode&0x3);
	sprite->v = tim->prect->y&0xff;
}

void setupSprite(SPRITE* sprite, int x, int y, int semiTrans)
{
	sprite->x = x;
	sprite->y = y;
	sprite->semiTrans = semiTrans;
}

char* sortSprite(int x, int y, u_long* ot, char* pri, SPRITE* sprite)
{
	SPRT* sprt;
	DR_TPAGE* tpage;

	sprt = (SPRT*)pri;
	setSprt(sprt);

	setXY0(sprt, x, y);
	setWH(sprt, sprite->w, sprite->h);
	setUV0(sprt, sprite->u, sprite->v);
	setShadeTex(sprt, 1);
	setSemiTrans(sprt, sprite->semiTrans);
	sprt->clut = sprite->clut;

	addPrim(ot, sprt);
	pri += sizeof(SPRT);

	tpage = (DR_TPAGE*)pri;
	setDrawTPage(tpage, 0, 1, sprite->tpage);
	addPrim(ot, tpage);

	return pri+sizeof(DR_TPAGE);
}

void loadTexture(u_int* tim, TIM_IMAGE* tparam)
{
    GetTimInfo((uint32_t*) tim, tparam);
    LoadImage(tparam->prect, (u_long*)tparam->paddr);
    DrawSync(0);

    if (tparam->mode&0x8)
    {
        LoadImage(tparam->crect, (u_long*)tparam->caddr);
        DrawSync(0);
    }
}

void draw()
{
    ClearOTagR(ot[db], OTLEN);
	for (int i = 0; i <= (sizeof(SPRITEs)/sizeof(SPRITEs[0])); i++)
	{ nextpri = sortSprite(SPRITEs[i]->x, SPRITEs[i]->y, ot[db], nextpri, SPRITEs[i]); }
}