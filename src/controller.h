#pragma once

u_char padbuff[2][34];
typedef struct {
    PADTYPE* _pad;
    int _id;
    int curr_pressed;
    int last_pressed;
} CONTROLLER;

CONTROLLER con1;
CONTROLLER con2;


void controllerInit()
{
    InitPAD(padbuff[0], 34, padbuff[1], 34);
    padbuff[0][0] = padbuff[0][1] = 0xff;
    padbuff[1][0] = padbuff[1][1] = 0xff;

    con1._id = 0;
    con2._id = 1;

    con1.curr_pressed = 0xff;
    con1.last_pressed = 0xff;
    con2.curr_pressed = 0xff;
    con2.last_pressed = 0xff;

    StartPAD();
    ChangeClearPAD(1);
}

void pollController(CONTROLLER* con)
{
    con->last_pressed = con->curr_pressed;
    con->_pad = (PADTYPE*)padbuff[con->_id];
    con->curr_pressed = con->_pad->btn;
}

int isConnected(CONTROLLER con)
{ return con._pad->stat == 0; }

int isType(CONTROLLER con, uint8_t type)
{ return con._pad->type == type; }

int isPressed(CONTROLLER con, uint16_t btn)
{
    // If the current is btn but not the last
    if (!(con.curr_pressed&btn) && con.last_pressed&btn)
    { return 1; }
    return 0;
}

int isHeld(CONTROLLER con, uint16_t btn)
{
    // Both the last and current are pressed
    if (!(con.curr_pressed&btn) && !(con.last_pressed&btn))
    { return 1; }
    return 0;
}

int isReleased(CONTROLLER con, uint16_t btn)
{
    // If the last is the btn but the current
    if (con.curr_pressed&btn && !(con.last_pressed&btn))
    { return 1; }
    return 0;
}