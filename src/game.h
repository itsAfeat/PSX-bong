#pragma once
#define SCENELEN 6

/**
 * Scene numbers
 * 5 = debug
 * 0 = title
 * 1 = game
 * 2 = win
 * 3 = credits
 * 4 = highscores
 */
int current_scene = 0;

const char* john_str1 = "In loving memory of John & Puyi";
const char* john_str2 = "\"Bazinga\"";
const char* title_str = "  START\nHIGHSCORE\n CREDITS";

int title_y = 96;

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
    nextpri = sortSprite(paul_sprite.x, paul_sprite.y, ot[db], nextpri, &paul_sprite);
    nextpri = sortSprite(puyi_sprite.x, puyi_sprite.y, ot[db], nextpri, &puyi_sprite);
    
    //for (int i = 0; i <= (sizeof(SPRITEs)/sizeof(SPRITEs[0])); i++)
	//{ nextpri = sortSprite(SPRITEs[i]->x, SPRITEs[i]->y, ot[db], nextpri, SPRITEs[i]); }
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

                if (isType(con1, PAD_ID_ANALOG) ||
                    isType(con1, PAD_ID_ANALOG_STICK))
                    {
                        int ls_x = con1._pad->ls_x-128;
                        int ls_y = con1._pad->ls_y-128;

                        if (ls_x < -10)
                        { paul_sprite.x -= ((ls_x < -50) ? 50 : -ls_x)/10; }
                        if (ls_x > 10)
                        { paul_sprite.x += ((ls_x > 50) ? 50 : ls_x)/10; }
                        if (ls_y < -10)
                        { paul_sprite.y -= ((ls_y < -50) ? 50 : -ls_y)/10; }
                        if (ls_y > 10)
                        { paul_sprite.y += ((ls_y > 50) ? 50 : ls_y)/10; }
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
    nextpri = sortSprite(title_sprite.x, title_sprite.y, ot[db], nextpri, &title_sprite);
    nextpri = printFont(320/2-(9*font8x16.w/2), 96, ot[db], nextpri, title_str, &font8x16);
    nextpri = printFont(320/2-(9*font8x16.w/2)-16, title_y, ot[db], nextpri, "~2", &font8x16);
}
static void title_update(void)
{
    pollController(&con1);

    if (isConnected(con1))
    {
        if (isPressed(con1, PAD_UP))
        {
            if (title_y == 96)
            { title_y = 144; }
            title_y -= 16;
        }
        if (isPressed(con1, PAD_DOWN))
        {
            if (title_y == 128)
            { title_y = 80; }
            title_y += 16;
        }

        if (isPressed(con1, PAD_CROSS) || isPressed(con1, PAD_START))
        {
            if (title_y == 96)  { current_scene = 1; }
            if (title_y == 112) { current_scene = 4; }
            if (title_y == 128) { current_scene = 3; }
        }
        if (isPressed(con1, PAD_SELECT))
        { current_scene = SCENELEN-1; }
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
}
static void game_update(void)
{
    pollController(&con1);
    if (isConnected(con1))
    { if (isPressed(con1, PAD_START)) { current_scene = 0; } }
}

/// WIN
static void win_draw(void)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: WIN");
        FntFlush(-1);
    #endif // DEBUG
    
    ClearOTagR(ot[db], OTLEN);
}
static void win_update(void)
{
    
}

/// CREDITS
static void credits_draw(void)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: CREDITS");
        FntFlush(-1);
    #endif // DEBUG
    
    ClearOTagR(ot[db], OTLEN);
}
static void credits_update(void)
{
    pollController(&con1);
    if (isConnected(con1))
    { if (isPressed(con1, PAD_START)) { current_scene = 0; } }
}

/// HIGHSCORES
static void highscores_draw(void)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: HIGHSCORES");
        FntFlush(-1);
    #endif // DEBUG
    
    ClearOTagR(ot[db], OTLEN);
}
static void highscores_update(void)
{
    pollController(&con1);
    if (isConnected(con1))
    { if (isPressed(con1, PAD_START)) { current_scene = 0; } }
}

SCENE sc_debug      = { debug_draw, debug_update };
SCENE sc_title      = { title_draw, title_update };
SCENE sc_game       = { game_draw, game_update };
SCENE sc_win        = { win_draw, win_update };
SCENE sc_credits    = { credits_draw, credits_update };
SCENE sc_highscores = { highscores_draw, highscores_update };
SCENE* SCENEs[SCENELEN];