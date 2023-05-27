#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#include <psxapi.h>
#include <psxpad.h>
#include <psxcd.h>

#include "core.h"
#include "graphics.h"
#include "font.h"
#include "controller.h"
#include "game.h"


void loadStuff()
{
    SPRITEs[0] = malloc(sizeof(SPRITE));
    SPRITEs[1] = malloc(sizeof(SPRITE));
    SPRITEs[2] = malloc(sizeof(SPRITE));
    SPRITEs[3] = malloc(sizeof(SPRITE));
    SPRITEs[4] = malloc(sizeof(SPRITE));
    SPRITEs[5] = malloc(sizeof(SPRITE));
    SPRITEs[6] = malloc(sizeof(SPRITE));
    SPRITEs[0] = &paul_sprite;
    SPRITEs[1] = &puyi_sprite;
    SPRITEs[2] = &title_sprite;
    SPRITEs[3] = &backTL_sprite;
    SPRITEs[4] = &backTR_sprite;
    SPRITEs[5] = &backBL_sprite;
    SPRITEs[6] = &backBR_sprite;
    
    SCENEs[0] = &sc_title;
    SCENEs[1] = &sc_game;
    SCENEs[2] = &sc_win;
    SCENEs[3] = &sc_credits;
    SCENEs[4] = &sc_highscores;
    SCENEs[5] = &sc_debug;

    for(int i = 0; i < (sizeof(TIMs)/sizeof(TIMs[0])); i++)
    {
        TIM_IMAGE _tim;
        u_int* _buffer = (u_int*)loadFile(TIMs[i]);
        loadTexture(_buffer, &_tim);
        getSprite(&_tim, SPRITEs[i]);

        free(_buffer);
    }

    // Load the 8x16 font
    TIM_IMAGE _tim;
    u_int* _buffer = (u_int*)loadFile("\\FONT8X16.TIM;1");
    loadTexture(_buffer, &_tim);
    getFont(&_tim, &font8x16, 8, 16, 28);

    free(_buffer);
}

int main()
{
    init();
    controllerInit();
    
    loadStuff();

    int center_x = 320/2-32;
    int center_y = 256/2-32;

    setupSprite(&paul_sprite, center_x-40, center_y, 0);
    setupSprite(&puyi_sprite, center_x+40, center_y, 1);
    setupSprite(&title_sprite, 320/2-title_sprite.w/2, 32, 0);

    uint8_t t = 0;

    while (1)
    {
        SCENEs[current_scene]->update();
        SCENEs[current_scene]->draw();
        display();

        if (t != con1._pad->type)
        { t = con1._pad->type; printf("PAD-TYPE: 0x%d\n", t); }
    }
    return 0;
}