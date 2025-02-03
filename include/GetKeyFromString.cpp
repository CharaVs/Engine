#pragma once

#include <raylib.h>
#include <cstring>

int GetKeyFromString(const char* str) {
    if (strcmp(str, "KEY_A") == 0) return KEY_A;
    if (strcmp(str, "KEY_B") == 0) return KEY_B;
    if (strcmp(str, "KEY_C") == 0) return KEY_C;
    if (strcmp(str, "KEY_D") == 0) return KEY_D;
    if (strcmp(str, "KEY_E") == 0) return KEY_E;
    if (strcmp(str, "KEY_F") == 0) return KEY_F;
    if (strcmp(str, "KEY_G") == 0) return KEY_G;
    if (strcmp(str, "KEY_H") == 0) return KEY_H;
    if (strcmp(str, "KEY_I") == 0) return KEY_I;
    if (strcmp(str, "KEY_J") == 0) return KEY_J;
    if (strcmp(str, "KEY_K") == 0) return KEY_K;
    if (strcmp(str, "KEY_L") == 0) return KEY_L;
    if (strcmp(str, "KEY_M") == 0) return KEY_M;
    if (strcmp(str, "KEY_N") == 0) return KEY_N;
    if (strcmp(str, "KEY_O") == 0) return KEY_O;
    if (strcmp(str, "KEY_P") == 0) return KEY_P;
    if (strcmp(str, "KEY_Q") == 0) return KEY_Q;
    if (strcmp(str, "KEY_R") == 0) return KEY_R;
    if (strcmp(str, "KEY_S") == 0) return KEY_S;
    if (strcmp(str, "KEY_T") == 0) return KEY_T;
    if (strcmp(str, "KEY_U") == 0) return KEY_U;
    if (strcmp(str, "KEY_V") == 0) return KEY_V;
    if (strcmp(str, "KEY_W") == 0) return KEY_W;
    if (strcmp(str, "KEY_X") == 0) return KEY_X;
    if (strcmp(str, "KEY_Y") == 0) return KEY_Y;
    if (strcmp(str, "KEY_Z") == 0) return KEY_Z;

    if (strcmp(str, "KEY_ZERO") == 0) return KEY_ZERO;
    if (strcmp(str, "KEY_ONE") == 0) return KEY_ONE;
    if (strcmp(str, "KEY_TWO") == 0) return KEY_TWO;
    if (strcmp(str, "KEY_THREE") == 0) return KEY_THREE;
    if (strcmp(str, "KEY_FOUR") == 0) return KEY_FOUR;
    if (strcmp(str, "KEY_FIVE") == 0) return KEY_FIVE;
    if (strcmp(str, "KEY_SIX") == 0) return KEY_SIX;
    if (strcmp(str, "KEY_SEVEN") == 0) return KEY_SEVEN;
    if (strcmp(str, "KEY_EIGHT") == 0) return KEY_EIGHT;
    if (strcmp(str, "KEY_NINE") == 0) return KEY_NINE;

    if (strcmp(str, "KEY_F1") == 0) return KEY_F1;
    if (strcmp(str, "KEY_F2") == 0) return KEY_F2;
    if (strcmp(str, "KEY_F3") == 0) return KEY_F3;
    if (strcmp(str, "KEY_F4") == 0) return KEY_F4;
    if (strcmp(str, "KEY_F5") == 0) return KEY_F5;
    if (strcmp(str, "KEY_F6") == 0) return KEY_F6;
    if (strcmp(str, "KEY_F7") == 0) return KEY_F7;
    if (strcmp(str, "KEY_F8") == 0) return KEY_F8;
    if (strcmp(str, "KEY_F9") == 0) return KEY_F9;
    if (strcmp(str, "KEY_F10") == 0) return KEY_F10;
    if (strcmp(str, "KEY_F11") == 0) return KEY_F11;
    if (strcmp(str, "KEY_F12") == 0) return KEY_F12;

    if (strcmp(str, "KEY_SPACE") == 0) return KEY_SPACE;
    if (strcmp(str, "KEY_ESCAPE") == 0) return KEY_ESCAPE;
    if (strcmp(str, "KEY_ENTER") == 0) return KEY_ENTER;
    if (strcmp(str, "KEY_BACKSPACE") == 0) return KEY_BACKSPACE;
    if (strcmp(str, "KEY_TAB") == 0) return KEY_TAB;
    if (strcmp(str, "KEY_LEFT_SHIFT") == 0) return KEY_LEFT_SHIFT;
    if (strcmp(str, "KEY_RIGHT_SHIFT") == 0) return KEY_RIGHT_SHIFT;
    if (strcmp(str, "KEY_LEFT_CONTROL") == 0) return KEY_LEFT_CONTROL;
    if (strcmp(str, "KEY_RIGHT_CONTROL") == 0) return KEY_RIGHT_CONTROL;
    if (strcmp(str, "KEY_LEFT_ALT") == 0) return KEY_LEFT_ALT;
    if (strcmp(str, "KEY_RIGHT_ALT") == 0) return KEY_RIGHT_ALT;

    if (strcmp(str, "KEY_UP") == 0) return KEY_UP;
    if (strcmp(str, "KEY_DOWN") == 0) return KEY_DOWN;
    if (strcmp(str, "KEY_LEFT") == 0) return KEY_LEFT;
    if (strcmp(str, "KEY_RIGHT") == 0) return KEY_RIGHT;

    return 0;
}