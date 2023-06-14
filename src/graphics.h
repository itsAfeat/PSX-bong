#pragma once

typedef struct {
	u_short tpage;		// The sprites Tpage
	u_short clut;		// The sprites CLUT
	u_char 	u,v;		// The U,V offset
	u_int	x,y;		// The position
	u_char 	w,h;		// The width and height
	int		semiTrans;	// The transparency bool
} SPRITE;

typedef struct {
    SPRITE spr;     	// The ball's sprite
    int x, y, dir;   	// The direction of the ball
    u_int speed;    	// The speed of the ball
    u_int counter_max;  // Timer start for speed increase
	u_int counter;		// The aforementioned timer
} BALL;

typedef struct {
	SPRITE spr;
	int x, y, speed;
} PAD;

SPRITE paul_sprite;
SPRITE puyi_sprite;
SPRITE title_sprite;
SPRITE backTL_sprite;
SPRITE backTR_sprite;
SPRITE backBL_sprite;
SPRITE backBR_sprite;
SPRITE pad_sprite;
SPRITE ball_sprite;

BALL ball_obj;
PAD pad1, pad2;

char *TIMs[] = {"\\PAUL.TIM;1", "\\PUYI.TIM;1",
				"\\TITLE.TIM;1", "\\BACKTL.TIM;1",
				"\\BACKTR.TIM;1", "\\BACKBL.TIM;1",
				"\\BACKBR.TIM;1", "\\PAD.TIM;1",
				"\\BALL.TIM;1"
				};
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

char* sortSprite(int fixed, int x, int y, u_long* ot, char* pri, SPRITE* sprite)
{
	SPRT* sprt;
	DR_TPAGE* tpage;

	sprt = (SPRT*)pri;
	setSprt(sprt);

	if (fixed)	{ setXY0(sprt, x>>12, y>>12); }
	else		{ setXY0(sprt, x, y); }
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

// 1 or 2 (random) if collision; 0 if none
int collisionCheck(SPRITE spr1, int x1, int y1, SPRITE spr2, int x2, int y2)
{
	int spr1_x1 = x1;
	int spr1_y1 = y1;
	int spr2_x1 = x2;
	int spr2_y1 = y2;

	int spr1_x2 = spr1_x1 + spr1.w;
	int spr1_y2 = spr1_y1 + spr1.h;
	int spr2_x2 = spr2_x1 + spr2.w;
	int spr2_y2 = spr2_y1 + spr2.h;

	if (spr1_x1 < spr2_x2 &&
		spr1_x2 > spr2_x1 &&
		spr1_y1 < spr2_y2 &&
		spr1_y2 > spr2_y1)
	{ return rand() % 2 + 1; }
	return 0;
}