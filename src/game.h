#pragma once
#define SCENELEN 6

typedef enum {
    ID_TITLE = 0,
    ID_GAME = 1,
    ID_WIN = 2,
    ID_CREDITS = 3,
    ID_DEBUG = 4
} SCENE_ENUM;

int current_scene = ID_TITLE;

const char* john_str1 = "In loving memory of John & Puyi";
const char* john_str2 = "\"Bazinga\"";
const char* title_str = " START\nCREDITS";
const char* win_str = "   REMATCH\nQUIT TO START";
const char* credits_str = "Programming: Alexander Wiencken\n        Art: Also Alexander\n      Audio: Still him\n    Testing: Guess who";
int title_y = 96;
int win_y = 192;

int player1_score = 0;
int player2_score = 0;
char player1_score_txt[1];
char player2_score_txt[1];

const int BALLXSTART = (320/2-4)<<12;
const int BALLYSTART = (256/2-4)<<12;
const int PAD1XSTART = 32;
const int PAD2XSTART = 280; // 320-32-8
const int PADYSTART  = 104; // 256/2-24
const float ANGLECONST = 11.378; // 4096 / 360

u_int rand_seed = 0;
u_int paused = 0;

typedef struct {
    void (* const draw)(void);
    void (* const update)(void);
} SCENE;

/// TITLE
static void debug_draw(void)
{
    FntPrint(-1, "SCENE: DEBUG");
    FntFlush(-1);
    int john_x1 = 320/2-(strlen(john_str1)*font8x16.w)/2;
    int john_x2 = 320/2-(strlen(john_str2)*font8x16.w)/2;

    ClearOTagR(ot[db], OTLEN);
    nextpri = printFont(john_x1, 256/2-48, ot[db], nextpri, john_str1, &font8x16);
    nextpri = printFont(john_x2, 256/2+32, ot[db], nextpri, john_str2, &font8x16);
    nextpri = sortSprite(0, paul_sprite.x, paul_sprite.y, ot[db], nextpri, &paul_sprite);
    nextpri = sortSprite(0, puyi_sprite.x, puyi_sprite.y, ot[db], nextpri, &puyi_sprite);
}
static void debug_update(void)
{
    pollController(&con1);

    if (isConnected(con1))
    {
        if (isType(con1, PAD_ID_DIGITAL) ||
            isType(con1, PAD_ID_ANALOG)  ||
            isType(con1, PAD_ID_ANALOG_STICK))
            {
                if (isHeld(con1, PAD_UP))
                { paul_sprite.y-=5; }
                if (isHeld(con1, PAD_DOWN))
                { paul_sprite.y+=5; }
                if (isHeld(con1, PAD_LEFT))
                { paul_sprite.x-=5; }
                if (isHeld(con1, PAD_RIGHT))
                { paul_sprite.x+=5; }

                if (isPressed(con1, PAD_L1) && isPressed(con1, PAD_R1))
                { current_scene = ID_TITLE; }

                if (isType(con1, PAD_ID_ANALOG) ||
                    isType(con1, PAD_ID_ANALOG_STICK))
                    {
                        int ls_x = con1._pad->ls_x-128;
                        int ls_y = con1._pad->ls_y-128;
                        int rs_x = con1._pad->rs_x-128;
                        int rs_y = con1._pad->rs_y-128;

                        if (ls_x < -10)
                        { paul_sprite.x -= ((ls_x < -50) ? 50 : -ls_x)/10; }
                        if (ls_x > 10)
                        { paul_sprite.x += ((ls_x > 50) ? 50 : ls_x)/10; }
                        if (ls_y < -10)
                        { paul_sprite.y -= ((ls_y < -50) ? 50 : -ls_y)/10; }
                        if (ls_y > 10)
                        { paul_sprite.y += ((ls_y > 50) ? 50 : ls_y)/10; }
                        
                        if (rs_x < -10)
                        { puyi_sprite.x -= ((rs_x < -50) ? 50 : -rs_x)/10; }
                        if (rs_x > 10)
                        { puyi_sprite.x += ((rs_x > 50) ? 50 : rs_x)/10; }
                        if (rs_y < -10)
                        { puyi_sprite.y -= ((rs_y < -50) ? 50 : -rs_y)/10; }
                        if (rs_y > 10)
                        { puyi_sprite.y += ((rs_y > 50) ? 50 : rs_y)/10; }
                    }
            }
    }
}

/// TITLE
static void title_draw(void)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: TITLE");
        FntFlush(-1);
    #endif // DEBUG

    ClearOTagR(ot[db], OTLEN);
    nextpri = sortSprite(0, title_sprite.x, title_sprite.y, ot[db], nextpri, &title_sprite);
    nextpri = printFont(320/2-(7*font8x16.w/2), 96, ot[db], nextpri, title_str, &font8x16);
    nextpri = printFont(320/2-(7*font8x16.w/2)-16, title_y, ot[db], nextpri, "~2", &font8x16);
}
static void title_update(void)
{
    pollController(&con1);
    pollController(&con2);

    if (isConnected(con1) || isConnected(con2))
    {
        if (isPressed(con1, PAD_UP) || isPressed(con1, PAD_DOWN) || isPressed(con2, PAD_UP) || isPressed(con2, PAD_DOWN))
        { title_y = (title_y == 96) ? 112 : 96; }

        if (isPressed(con1, PAD_CROSS) || isPressed(con1, PAD_START) || isPressed(con2, PAD_CROSS) || isPressed(con2, PAD_START))
        {
            srand(rand_seed);
            ball_obj.spr = ball_sprite;
            ball_obj.dir = rand() % 10 <= 4 ? 270 : 90;
            ball_obj.speed = 1;
            ball_obj.counter_max = 5;
            ball_obj.counter = 0;

            ball_obj.x = BALLXSTART;
            ball_obj.y = BALLYSTART;

            pad1.spr = pad_sprite;
            pad1.speed = 3;
            pad1.x = PAD1XSTART;
            pad1.y = PADYSTART;
            
            pad2.spr = pad_sprite;
            pad2.speed = 3;
            pad2.x = PAD2XSTART;
            pad2.y = PADYSTART;

            if (title_y == 96)  { current_scene = ID_GAME; player1_score = player2_score = 0; }
            if (title_y == 112) { current_scene = ID_CREDITS; }
        }

        #ifdef DEBUG
        if (isPressed(con1, PAD_L1) && isPressed(con1, PAD_R1))
        { current_scene = ID_DEBUG; }
        #endif // DEBUG
    }
}

/// GAME
static void game_draw(void)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: GAME");
        FntFlush(-1);
    #endif // DEBUG

    ClearOTagR(ot[db], OTLEN);
    
    if (paused)
    { nextpri = printFont(320/2-(6*font8x16.w/2), 256/2-font8x16.h, ot[db], nextpri, "PAUSED", &font8x16); }
    
    nextpri = sortSprite(0, pad1.x, pad1.y, ot[db], nextpri, &pad_sprite);
    nextpri = sortSprite(0, pad2.x, pad2.y, ot[db], nextpri, &pad_sprite);
    nextpri = sortSprite(1, ball_obj.x, ball_obj.y, ot[db], nextpri, &ball_sprite);

    nextpri = printFont(0, 0, ot[db], nextpri, "Player 1", &font8x16);
    nextpri = printFont(320-64, 0, ot[db], nextpri, "Player 2", &font8x16);

    sprintf(player1_score_txt, "%d", player1_score);
    sprintf(player2_score_txt, "%d", player2_score);

    nextpri = printFont(320/2-28, 4, ot[db], nextpri, player1_score_txt, &font8x16);
    nextpri = printFont(320/2+20, 4, ot[db], nextpri, player2_score_txt, &font8x16);

    nextpri = sortSprite(0, 0, 24, ot[db], nextpri, &backTL_sprite);
    nextpri = sortSprite(0, backTL_sprite.w, 24, ot[db], nextpri, &backTR_sprite);
    nextpri = sortSprite(0, 0, 232-backBL_sprite.h, ot[db], nextpri, &backBL_sprite);
    nextpri = sortSprite(0, backTR_sprite.w, 232-backBR_sprite.h, ot[db], nextpri, &backBR_sprite);
}
static void game_update(void)
{
    pollController(&con1);
    pollController(&con2);

    if (isConnected(con1))
    {
        if (isPressed(con1, PAD_START)) { paused = !paused; }
        if (!paused) {
            if (isHeld(con1, PAD_UP)) { pad1.y -= pad1.speed; }
            if (isHeld(con1, PAD_DOWN)) { pad1.y += pad1.speed; }
        }
        else
        { if (isPressed(con1, PAD_SELECT)) { current_scene = ID_TITLE; paused = !paused; }}

        #ifdef DEBUG
            if (isPressed(con1, PAD_LEFT)) { player1_score++; }
            if (isPressed(con1, PAD_RIGHT)) { player2_score++; }
            if (isHeld(con1, PAD_TRIANGLE)) { pad2.y -= pad2.speed; }
            if (isHeld(con1, PAD_CROSS)) { pad2.y += pad2.speed; }
        #endif // DEBUG

    }

    if (isConnected(con2))
    {
        if (isPressed(con2, PAD_START)) { paused = !paused; }
        if (!paused) {
            if (isHeld(con2, PAD_UP)) { pad2.y -= pad2.speed; }
            if (isHeld(con2, PAD_DOWN)) { pad2.y += pad2.speed; }
        }
        else
        { if (isPressed(con2, PAD_SELECT)) { current_scene = ID_TITLE; paused = !paused; }}
    }
    
    pad1.y = clamp(pad1.y, 30, 256-pad_sprite.h-30);
    pad2.y = clamp(pad2.y, 30, 256-pad_sprite.h-30);
    
    if (player1_score == 10 || player2_score == 10)
    { current_scene = ID_WIN; }

    /// BALL LOGIC
    if (!paused) {
        int new_x = ball_obj.x + isin(ball_obj.dir * ANGLECONST) * ball_obj.speed;
        int new_y = ball_obj.y + icos(ball_obj.dir * ANGLECONST) * ball_obj.speed;

        int colCheck1 = collisionCheck(pad1.spr, pad1.x, pad1.y, ball_obj.spr, new_x>>12, new_y>>12);
        int colCheck2 = collisionCheck(pad2.spr, pad2.x, pad2.y, ball_obj.spr, new_x>>12, new_y>>12);

        if (colCheck1 != 0 && ball_obj.dir > 180)
        { flipAngle(&ball_obj.dir); ball_obj.counter++; }
        else if (colCheck2 != 0 && ball_obj.dir <= 180)
        { flipAngle(&ball_obj.dir); ball_obj.counter++; }
        else if (new_y < 30<<12 || new_y > (256-ball_obj.spr.h-30)<<12)
        {
            reflectAngle(&ball_obj.dir);
            if (isBetween(ball_obj.dir, 160, 180))
            { ball_obj.dir -= 10; }
            if (isBetween(ball_obj.dir, 180, 200))
            { ball_obj.dir += 10; }
            if (isBetween(ball_obj.dir, 340, 359))
            { ball_obj.dir -= 10; }
            if (isBetween(ball_obj.dir, 0, 20))
            { ball_obj.dir += 10; }
        }
        else
        { ball_obj.x = new_x; ball_obj.y = new_y; }

        if (colCheck1 == 1 && ball_obj.dir > 20)
        { ball_obj.dir += rand() % 20 + 10; }
        else if (colCheck1 == 2 && ball_obj.dir < 160)
        { ball_obj.dir += rand() % 20 + 10; }

        if (colCheck2 == 1 && ball_obj.dir < 340)
        { ball_obj.dir -= rand() % 10 + 10; }
        if (colCheck2 == 2 && ball_obj.dir > 200)
        { ball_obj.dir += rand() % 10 + 10; }

        if (ball_obj.x <= 10<<12 || ball_obj.x >= 310<<12)
        {
            if (ball_obj.x <= 10<<12)   player2_score++;
            else                        player1_score++;
            ball_obj.x = BALLXSTART;
            ball_obj.y = BALLYSTART;

            pad1.x = PAD1XSTART;
            pad1.y = PADYSTART;
            
            pad2.x = PAD2XSTART;
            pad2.y = PADYSTART;

            ball_obj.speed = 2;
            ball_obj.counter = 0;
            ball_obj.dir = rand() % 10 <= 4 ? 270 : 90;
        }

        if (ball_obj.counter >= ball_obj.counter_max)
        { ball_obj.counter = 0; ball_obj.speed++; }
    }
}

/// WIN
static void win_draw(void)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: WIN");
        FntFlush(-1);
    #endif // DEBUG
    
    ClearOTagR(ot[db], OTLEN);

    if (player1_score > player2_score)
    { nextpri = printFont(320/2-(11*font8x16.w/2), 256/2-(font8x16.h), ot[db], nextpri, "PLAYER 1 WON", &font8x16); }
    else
    { nextpri = printFont(320/2-(11*font8x16.w/2), 256/2-(font8x16.h), ot[db], nextpri, "PLAYER 2 WON", &font8x16); }

    nextpri = printFont(320/2-(12*font8x16.w/2), 192, ot[db], nextpri, win_str, &font8x16);
    nextpri = printFont(320/2-(12*font8x16.w/2)-16, win_y, ot[db], nextpri, "~2", &font8x16);
}
static void win_update(void)
{
    pollController(&con1);

    if (isConnected(con1))
    {
        if (isPressed(con1, PAD_UP) || isPressed(con1, PAD_DOWN))
        { win_y = (win_y == 192) ? 208 : 192; }

        if (isPressed(con1, PAD_CROSS) || isPressed(con1, PAD_START))
        {
            ball_obj.spr = ball_sprite;
            ball_obj.dir = rand() % 10 <= 4 ? 270 : 90;
            ball_obj.speed = 1;
            ball_obj.counter_max = 5;
            ball_obj.counter = 0;

            ball_obj.x = BALLXSTART;
            ball_obj.y = BALLYSTART;

            pad1.spr = pad_sprite;
            pad1.speed = 3;
            pad1.x = PAD1XSTART;
            pad1.y = PADYSTART;
            
            pad2.spr = pad_sprite;
            pad2.speed = 3;
            pad2.x = PAD2XSTART;
            pad2.y = PADYSTART;

            if (win_y == 192) { current_scene = ID_GAME; player1_score = player2_score = 0; }
            if (win_y == 208) { current_scene = ID_TITLE; }
        }
    }
}

/// CREDITS
static void credits_draw(void)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: CREDITS");
        FntFlush(-1);
    #endif // DEBUG
    
    ClearOTagR(ot[db], OTLEN);
    nextpri = printFont(320/2-(31*font8x16.w/2), 256/2-(4*font8x16.h/2), ot[db], nextpri, credits_str, &font8x16);
}
static void credits_update(void)
{
    pollController(&con1);
    if (isConnected(con1))
    { if (isPressed(con1, PAD_START)) { current_scene = ID_TITLE; } }
}

SCENE sc_debug      = { debug_draw, debug_update };
SCENE sc_title      = { title_draw, title_update };
SCENE sc_game       = { game_draw, game_update };
SCENE sc_win        = { win_draw, win_update };
SCENE sc_credits    = { credits_draw, credits_update };
SCENE* SCENEs[SCENELEN];