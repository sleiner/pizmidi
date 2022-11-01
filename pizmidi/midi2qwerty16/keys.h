#ifndef KEYS_H
#define KEYS_H

int FLOAT_TO_KEYCODE(float i)
{
    //int k = ((unsigned char)((i) * 115.0f) + 8 );
    int k = roundToInt(i * 100.0f) + 23;
    //remapping
    if (k == 23)
    {
        k = 8; //BACKSPACE
    }
    else if (k == 24)
    {
        k = 9; //TAB
    }
    else if (k == 25)
    {
        k = 13; //ENTER
    }
    else if (k == 26)
    {
        k = 19; //PAUSE
    }
    //27 escape
    else if (k == 28)
    {
        k = 0x01; //VK_LBUTTON
    }
    else if (k == 29)
    {
        k = 0x02; //VK_RBUTTON
    }
    else if (k == 30)
    {
        k = 0x04; //VK_MBUTTON
    }
    else if (k == 31)
    {
        k = 0x5D; //VK_APPS
    }
    //32-40 space, arrows, etc
    else if (k == 41)
    {
        k = 0xDB; // [ { for US
    }
    else if (k == 42)
    {
        k = 0xDC; // \ | for US
    }
    else if (k == 43)
    {
        k = 0xDD; // ] } for US
    }
    //44-46 ins, del, prtscr
    else if (k == 47)
    {
        k = 0xC0; // ` ~ for US
    }
    //48-57 numbers
    else if (k == 58)
    {
        k = 0xBD; // - _
    }
    else if (k == 59)
    {
        k = 0xBB; // = +
    }
    else if (k == 60)
    {
        k = 0xBC; // , <
    }
    else if (k == 61)
    {
        k = 0xBE; // . >
    }
    else if (k == 62)
    {
        k = 0xBF; // / ? for US
    }
    else if (k == 63)
    {
        k = 0xBA; // ; : for US
    }
    else if (k == 64)
    {
        k = 0xDE; // ' " for US
    }
    //65-90 letters A-Z
    else if (k == 91)
    {
        k = 0x14; //  caps lock
    }
    else if (k == 92)
    {
        k = 0x10; //  lshift
    }
    else if (k == 93)
    {
        k = 0x11; //  lctrl
    }
    else if (k == 94)
    {
        k = 0x12; //  lalt
    }
    else if (k == 95)
    {
        k = 0x5B; //  lwin
    }
    //96-111 numpad
    else if (k == 108)
    {
        k = 0x90; //num lock
    }
    //112-123 F1-F12
    return k;
}

//#define KEYCODE_TO_FLOAT(i) ( (( (float)(i+0.1) ) - 8) / 115.0f)
#define KEYCODE_TO_FLOAT(i) ((((float) (i + 0.1)) - 23) / 100.0f)
#define FLOAT_TO_MOD(i)     ((char) ((i) *15))
#define MOD_TO_FLOAT(i)     ((float) (i + 0.1) / 15.f)

enum wxKeyModifier
{
    wxMOD_NONE    = 0x0000,
    wxMOD_ALT     = 0x0001,
    wxMOD_CONTROL = 0x0002,
    wxMOD_ALTGR   = wxMOD_ALT | wxMOD_CONTROL,
    wxMOD_SHIFT   = 0x0004,
    wxMOD_META    = 0x0008,
#if defined(__WXMAC__) || defined(__WXCOCOA__)
    wxMOD_CMD = wxMOD_META,
#else
    wxMOD_CMD = wxMOD_CONTROL,
#endif
    wxMOD_ALL = wxMOD_ALT | wxMOD_CONTROL | wxMOD_SHIFT | wxMOD_META
};

#if WIN32
char* getKeyName(unsigned int keycode)
{
    char* text;
    text = new char[16];
    if (keycode == 8)
    {
        strcpy(text, "BACKSPACE");
    }
    else if (keycode == 9)
    {
        strcpy(text, "TAB");
    }
    else if (keycode == 13)
    {
        strcpy(text, "ENTER");
    }
    else if (keycode == 19)
    {
        strcpy(text, "PAUSE");
    }
    else if (keycode == 27)
    {
        strcpy(text, "ESCAPE");
    }

    //28-30 for mouse buttons
    else if (keycode == 0x01)
    {
        strcpy(text, "Left Click");
    }
    else if (keycode == 0x02)
    {
        strcpy(text, "Right Click");
    }
    else if (keycode == 0x04)
    {
        strcpy(text, "Middle Click");
    }

    else if (keycode == 0x5D)
    {
        strcpy(text, "MENU");
    }
    else if (keycode == 32)
    {
        strcpy(text, "SPACE");
    }
    else if (keycode == 33)
    {
        strcpy(text, "PAGE UP");
    }
    else if (keycode == 34)
    {
        strcpy(text, "PAGE DOWN");
    }
    else if (keycode == 35)
    {
        strcpy(text, "END");
    }
    else if (keycode == 36)
    {
        strcpy(text, "HOME");
    }
    else if (keycode == 37)
    {
        strcpy(text, "LEFT");
    }
    else if (keycode == 38)
    {
        strcpy(text, "UP");
    }
    else if (keycode == 39)
    {
        strcpy(text, "RIGHT");
    }
    else if (keycode == 40)
    {
        strcpy(text, "DOWN");
    }
    else if (keycode == 0xDB)
    {
        strcpy(text, "[ {");
    }
    else if (keycode == 0xDC)
    {
        strcpy(text, "\\ |");
    }
    else if (keycode == 0xDD)
    {
        strcpy(text, "] }");
    }
    else if (keycode == 44)
    {
        strcpy(text, "PRINT SCREEN");
    }
    else if (keycode == 45)
    {
        strcpy(text, "INSERT");
    }
    else if (keycode == 46)
    {
        strcpy(text, "DELETE");
    }
    else if (keycode == 0xC0)
    {
        strcpy(text, "` ~");
    }
    else if (keycode == 48)
    {
        strcpy(text, "0 )");
    }
    else if (keycode == 49)
    {
        strcpy(text, "1 !");
    }
    else if (keycode == 50)
    {
        strcpy(text, "2 @");
    }
    else if (keycode == 51)
    {
        strcpy(text, "3 #");
    }
    else if (keycode == 52)
    {
        strcpy(text, "4 $");
    }
    else if (keycode == 53)
    {
        strcpy(text, "5 %");
    }
    else if (keycode == 54)
    {
        strcpy(text, "6 ^");
    }
    else if (keycode == 55)
    {
        strcpy(text, "7 &");
    }
    else if (keycode == 56)
    {
        strcpy(text, "8 *");
    }
    else if (keycode == 57)
    {
        strcpy(text, "9 (");
    }
    else if (keycode == 0xBD)
    {
        strcpy(text, "- _");
    }
    else if (keycode == 0xBB)
    {
        strcpy(text, "= +");
    }
    else if (keycode == 0xBC)
    {
        strcpy(text, ", <");
    }
    else if (keycode == 0xBE)
    {
        strcpy(text, ". >");
    }
    else if (keycode == 0xBF)
    {
        strcpy(text, "/ ?");
    }
    else if (keycode == 0xBA)
    {
        strcpy(text, "; :");
    }
    else if (keycode == 0xDE)
    {
        strcpy(text, "' \"");
    }
    else if (65 <= keycode && keycode <= 90)
    {
        sprintf(text, "%c", keycode); //letters
    }
    else if (keycode == 0x14)
    {
        strcpy(text, "CAPS LOCK");
    }
    else if (keycode == 0x10)
    {
        strcpy(text, "SHIFT");
    }
    else if (keycode == 0x12)
    {
        strcpy(text, "CTRL");
    }
    else if (keycode == 0x11)
    {
        strcpy(text, "ALT");
    }
    else if (keycode == 0x5B)
    {
        strcpy(text, "WIN");
    }
    else if (keycode == 96)
    {
        strcpy(text, "NUMPAD 0");
    }
    else if (keycode == 97)
    {
        strcpy(text, "NUMPAD 1");
    }
    else if (keycode == 98)
    {
        strcpy(text, "NUMPAD 2");
    }
    else if (keycode == 99)
    {
        strcpy(text, "NUMPAD 3");
    }
    else if (keycode == 100)
    {
        strcpy(text, "NUMPAD 4");
    }
    else if (keycode == 101)
    {
        strcpy(text, "NUMPAD 5");
    }
    else if (keycode == 102)
    {
        strcpy(text, "NUMPAD 6");
    }
    else if (keycode == 103)
    {
        strcpy(text, "NUMPAD 7");
    }
    else if (keycode == 104)
    {
        strcpy(text, "NUMPAD 8");
    }
    else if (keycode == 105)
    {
        strcpy(text, "NUMPAD 9");
    }
    else if (keycode == 106)
    {
        strcpy(text, "NUMPAD *");
    }
    else if (keycode == 107)
    {
        strcpy(text, "NUMPAD +");
    }
    else if (keycode == 0x90)
    {
        strcpy(text, "NUM LOCK");
    }
    else if (keycode == 109)
    {
        strcpy(text, "NUMPAD -");
    }
    else if (keycode == 110)
    {
        strcpy(text, "NUMPAD .");
    }
    else if (keycode == 111)
    {
        strcpy(text, "NUMPAD /");
    }
    else if (keycode == 112)
    {
        strcpy(text, "F1");
    }
    else if (keycode == 113)
    {
        strcpy(text, "F2");
    }
    else if (keycode == 114)
    {
        strcpy(text, "F3");
    }
    else if (keycode == 115)
    {
        strcpy(text, "F4");
    }
    else if (keycode == 116)
    {
        strcpy(text, "F5");
    }
    else if (keycode == 117)
    {
        strcpy(text, "F6");
    }
    else if (keycode == 118)
    {
        strcpy(text, "F7");
    }
    else if (keycode == 119)
    {
        strcpy(text, "F8");
    }
    else if (keycode == 120)
    {
        strcpy(text, "F9");
    }
    else if (keycode == 121)
    {
        strcpy(text, "F10");
    }
    else if (keycode == 122)
    {
        strcpy(text, "F11");
    }
    else if (keycode == 123)
    {
        strcpy(text, "F12");
    }
    else
    {
        sprintf(text, "keycode %d", keycode);
    }
    return text;
}

char* getModifierName(float i)
{
    char* Modifier;
    Modifier = new char[24];
    if (FLOAT_TO_MOD(i) == 0)
    {
        strcpy(Modifier, "None");
    }
    else if (FLOAT_TO_MOD(i) == 1)
    {
        strcpy(Modifier, "Shift");
    }
    else if (FLOAT_TO_MOD(i) == 2)
    {
        strcpy(Modifier, "Ctrl");
    }
    else if (FLOAT_TO_MOD(i) == 3)
    {
        strcpy(Modifier, "Alt");
    }
    else if (FLOAT_TO_MOD(i) == 4)
    {
        strcpy(Modifier, "Win");
    }
    else if (FLOAT_TO_MOD(i) == 5)
    {
        strcpy(Modifier, "ShiftCtrl");
    }
    else if (FLOAT_TO_MOD(i) == 6)
    {
        strcpy(Modifier, "ShiftAlt");
    }
    else if (FLOAT_TO_MOD(i) == 7)
    {
        strcpy(Modifier, "ShiftWin");
    }
    else if (FLOAT_TO_MOD(i) == 8)
    {
        strcpy(Modifier, "CtrlAlt");
    }
    else if (FLOAT_TO_MOD(i) == 9)
    {
        strcpy(Modifier, "CtrlWin");
    }
    else if (FLOAT_TO_MOD(i) == 10)
    {
        strcpy(Modifier, "AltWin");
    }
    else if (FLOAT_TO_MOD(i) == 11)
    {
        strcpy(Modifier, "ShiftCtrlAlt");
    }
    else if (FLOAT_TO_MOD(i) == 12)
    {
        strcpy(Modifier, "ShiftCtrlWin");
    }
    else if (FLOAT_TO_MOD(i) == 13)
    {
        strcpy(Modifier, "ShiftWinAlt");
    }
    else if (FLOAT_TO_MOD(i) == 14)
    {
        strcpy(Modifier, "WinCtrlAlt");
    }
    else
    {
        strcpy(Modifier, "ShiftCtrlAltWin");
    }
    return Modifier;
}

void InitAscii2KeyCodeTable(){};

#elif defined(__linux__)
//map keys from winuser.h to the ones in keysymdef.h
int WINVK_TO_LINXK(int k)
{
    if (k == 0x08)
    {
        k = XK_BackSpace;
    }
    else if (k == 0x09)
    {
        k = XK_Tab;
    }
    else if (k == 0x0D)
    {
        k = XK_Return;
    }
    else if (k == 0x13)
    {
        k = XK_Pause;
    }
    else if (k == 0x1B)
    {
        k = XK_Escape;
    }
    else if (k == 0x01)
    {
        k = XK_Pointer_Button1;
    }
    else if (k == 0x02)
    {
        k = XK_Pointer_Button3;
    }
    else if (k == 0x04)
    {
        k = XK_Pointer_Button2;
    }
    else if (k == 0x5D)
    {
        k = XK_Menu;
    }
    else if (k == 0x20)
    {
        k = XK_space;
    }
    else if (k == 0x21)
    {
        k = XK_Page_Up;
    }
    else if (k == 0x22)
    {
        k = XK_Page_Down;
    }
    else if (k == 0x23)
    {
        k = XK_End;
    }
    else if (k == 0x24)
    {
        k = XK_Home;
    }
    else if (k == 0x25)
    {
        k = XK_Left;
    }
    else if (k == 0x26)
    {
        k = XK_Up;
    }
    else if (k == 0x27)
    {
        k = XK_Right;
    }
    else if (k == 0x28)
    {
        k = XK_Down;
    }
    else if (k == 0xDB)
    {
        k = 0x5B; //'[{' for US
    }
    else if (k == 0xDC)
    {
        k = 0x5C; //'\|' for US
    }
    else if (k == 0xDD)
    {
        k = 0x5D; //']}' for US
    }
    else if (k == 0x2C)
    {
        k = XK_Print;
    }
    else if (k == 0x2D)
    {
        k = XK_Insert;
    }
    else if (k == 0x2E)
    {
        k = XK_Delete;
    }
    else if (k == 0x2F)
    {
        k = XK_0;
    }
    // 0x30 through 0x39 are 0-9
    else if (k == 0xBC)
    {
        k = XK_comma; // ,
    }
    else if (k == 0xBE)
    {
        k = XK_period; // .
    }
    else if (k == 0xBB)
    {
        k = XK_equal; // =
    }
    else if (k == 0xBD)
    {
        k = XK_minus; // -
    }
    else if (k == 0xBA)
    {
        k = XK_semicolon; // ';:' for US
    }
    else if (k == 0xBF)
    {
        k = XK_slash; // '/?' for US
    }
    else if (k == 0xC0)
    {
        k = XK_grave; // '`~' for US
    }
    else if (k == 0xDE)
    {
        k = XK_apostrophe; //  ''"' for US
    }
    // 0x41 through 0x5A are A-Z
    else if (k == 0x14)
    {
        k = XK_Caps_Lock;
    }
    else if (k == 0x10)
    {
        k = XK_Shift_L; //  lshift
    }
    else if (k == 0x11)
    {
        k = XK_Control_L;
    }
    else if (k == 0x12)
    {
        k = XK_Alt_L;
    }
    else if (k == 0x5B)
    {
        k = XK_Super_L;
    }
    else if (k == 0x60)
    {
        k = XK_KP_0;
    }
    else if (k == 0x61)
    {
        k = XK_KP_1;
    }
    else if (k == 0x62)
    {
        k = XK_KP_2;
    }
    else if (k == 0x63)
    {
        k = XK_KP_3;
    }
    else if (k == 0x64)
    {
        k = XK_KP_4;
    }
    else if (k == 0x65)
    {
        k = XK_KP_5;
    }
    else if (k == 0x66)
    {
        k = XK_KP_6;
    }
    else if (k == 0x67)
    {
        k = XK_KP_7;
    }
    else if (k == 0x68)
    {
        k = XK_KP_8;
    }
    else if (k == 0x69)
    {
        k = XK_KP_9;
    }
    else if (k == 0x6A)
    {
        k = XK_KP_Multiply;
    }
    else if (k == 0x6B)
    {
        k = XK_KP_Add;
    }
    else if (k == 0x6C)
    {
        k = XK_KP_Subtract;
    }
    else if (k == 0x90)
    {
        k = XK_Num_Lock;
    }
    else if (k == 0x6E)
    {
        k = XK_KP_Decimal;
    }
    else if (k == 0x6F)
    {
        k = XK_KP_Divide;
    }
    else if (k == 0x70)
    {
        k = XK_F1;
    }
    else if (k == 0x71)
    {
        k = XK_F2;
    }
    else if (k == 0x72)
    {
        k = XK_F3;
    }
    else if (k == 0x73)
    {
        k = XK_F4;
    }
    else if (k == 0x74)
    {
        k = XK_F5;
    }
    else if (k == 0x75)
    {
        k = XK_F6;
    }
    else if (k == 0x76)
    {
        k = XK_F7;
    }
    else if (k == 0x77)
    {
        k = XK_F8;
    }
    else if (k == 0x78)
    {
        k = XK_F9;
    }
    else if (k == 0x79)
    {
        k = XK_F10;
    }
    else if (k == 0x7A)
    {
        k = XK_F11;
    }
    else if (k == 0x7B)
    {
        k = XK_F12;
    }
    return k;
}

char* getKeyName(unsigned int keycode)
{
    char* text;
    text = new char[16];
    if (keycode == 8)
    {
        strcpy(text, "BACKSPACE");
    }
    else if (keycode == 9)
    {
        strcpy(text, "TAB");
    }
    else if (keycode == 13)
    {
        strcpy(text, "ENTER");
    }
    else if (keycode == 19)
    {
        strcpy(text, "PAUSE");
    }
    else if (keycode == 27)
    {
        strcpy(text, "ESCAPE");
    }

    //28-30 for mouse buttons
    else if (keycode == 0x01)
    {
        strcpy(text, "(reserved)");
    }
    else if (keycode == 0x02)
    {
        strcpy(text, "(reserved)");
    }
    else if (keycode == 0x04)
    {
        strcpy(text, "(reserved)");
    }

    else if (keycode == 0x5D)
    {
        strcpy(text, "MENU");
    }
    else if (keycode == 32)
    {
        strcpy(text, "SPACE");
    }
    else if (keycode == 33)
    {
        strcpy(text, "PAGE UP");
    }
    else if (keycode == 34)
    {
        strcpy(text, "PAGE DOWN");
    }
    else if (keycode == 35)
    {
        strcpy(text, "END");
    }
    else if (keycode == 36)
    {
        strcpy(text, "HOME");
    }
    else if (keycode == 37)
    {
        strcpy(text, "LEFT");
    }
    else if (keycode == 38)
    {
        strcpy(text, "UP");
    }
    else if (keycode == 39)
    {
        strcpy(text, "RIGHT");
    }
    else if (keycode == 40)
    {
        strcpy(text, "DOWN");
    }
    else if (keycode == 0xDB)
    {
        strcpy(text, "[ {");
    }
    else if (keycode == 0xDC)
    {
        strcpy(text, "\\ |");
    }
    else if (keycode == 0xDD)
    {
        strcpy(text, "] }");
    }
    else if (keycode == 44)
    {
        strcpy(text, "PRINT SCREEN");
    }
    else if (keycode == 45)
    {
        strcpy(text, "INSERT");
    }
    else if (keycode == 46)
    {
        strcpy(text, "DELETE");
    }
    else if (keycode == 0xC0)
    {
        strcpy(text, "` ~");
    }
    else if (keycode == 48)
    {
        strcpy(text, "0 )");
    }
    else if (keycode == 49)
    {
        strcpy(text, "1 !");
    }
    else if (keycode == 50)
    {
        strcpy(text, "2 @");
    }
    else if (keycode == 51)
    {
        strcpy(text, "3 #");
    }
    else if (keycode == 52)
    {
        strcpy(text, "4 $");
    }
    else if (keycode == 53)
    {
        strcpy(text, "5 %");
    }
    else if (keycode == 54)
    {
        strcpy(text, "6 ^");
    }
    else if (keycode == 55)
    {
        strcpy(text, "7 &");
    }
    else if (keycode == 56)
    {
        strcpy(text, "8 *");
    }
    else if (keycode == 57)
    {
        strcpy(text, "9 (");
    }
    else if (keycode == 0xBD)
    {
        strcpy(text, "- _");
    }
    else if (keycode == 0xBB)
    {
        strcpy(text, "= +");
    }
    else if (keycode == 0xBC)
    {
        strcpy(text, ", <");
    }
    else if (keycode == 0xBE)
    {
        strcpy(text, ". >");
    }
    else if (keycode == 0xBF)
    {
        strcpy(text, "/ ?");
    }
    else if (keycode == 0xBA)
    {
        strcpy(text, "; :");
    }
    else if (keycode == 0xDE)
    {
        strcpy(text, "' \"");
    }
    else if (65 <= keycode && keycode <= 90)
    {
        sprintf(text, "%c", keycode); //letters
    }
    else if (keycode == 0x14)
    {
        strcpy(text, "CAPS LOCK");
    }
    else if (keycode == 0x10)
    {
        strcpy(text, "SHIFT");
    }
    else if (keycode == 0x12)
    {
        strcpy(text, "CTRL");
    }
    else if (keycode == 0x11)
    {
        strcpy(text, "ALT");
    }
    else if (keycode == 0x5B)
    {
        strcpy(text, "SUPER");
    }
    else if (keycode == 96)
    {
        strcpy(text, "NUMPAD 0");
    }
    else if (keycode == 97)
    {
        strcpy(text, "NUMPAD 1");
    }
    else if (keycode == 98)
    {
        strcpy(text, "NUMPAD 2");
    }
    else if (keycode == 99)
    {
        strcpy(text, "NUMPAD 3");
    }
    else if (keycode == 100)
    {
        strcpy(text, "NUMPAD 4");
    }
    else if (keycode == 101)
    {
        strcpy(text, "NUMPAD 5");
    }
    else if (keycode == 102)
    {
        strcpy(text, "NUMPAD 6");
    }
    else if (keycode == 103)
    {
        strcpy(text, "NUMPAD 7");
    }
    else if (keycode == 104)
    {
        strcpy(text, "NUMPAD 8");
    }
    else if (keycode == 105)
    {
        strcpy(text, "NUMPAD 9");
    }
    else if (keycode == 106)
    {
        strcpy(text, "NUMPAD *");
    }
    else if (keycode == 107)
    {
        strcpy(text, "NUMPAD +");
    }
    else if (keycode == 0x90)
    {
        strcpy(text, "NUM LOCK");
    }
    else if (keycode == 109)
    {
        strcpy(text, "NUMPAD -");
    }
    else if (keycode == 110)
    {
        strcpy(text, "NUMPAD .");
    }
    else if (keycode == 111)
    {
        strcpy(text, "NUMPAD /");
    }
    else if (keycode == 112)
    {
        strcpy(text, "F1");
    }
    else if (keycode == 113)
    {
        strcpy(text, "F2");
    }
    else if (keycode == 114)
    {
        strcpy(text, "F3");
    }
    else if (keycode == 115)
    {
        strcpy(text, "F4");
    }
    else if (keycode == 116)
    {
        strcpy(text, "F5");
    }
    else if (keycode == 117)
    {
        strcpy(text, "F6");
    }
    else if (keycode == 118)
    {
        strcpy(text, "F7");
    }
    else if (keycode == 119)
    {
        strcpy(text, "F8");
    }
    else if (keycode == 120)
    {
        strcpy(text, "F9");
    }
    else if (keycode == 121)
    {
        strcpy(text, "F10");
    }
    else if (keycode == 122)
    {
        strcpy(text, "F11");
    }
    else if (keycode == 123)
    {
        strcpy(text, "F12");
    }
    else
    {
        sprintf(text, "keycode %d", keycode);
    }
    return text;
}

char* getModifierName(float i)
{
    char* Modifier;
    Modifier = new char[24];
    if (FLOAT_TO_MOD(i) == 0)
    {
        strcpy(Modifier, "None");
    }
    else if (FLOAT_TO_MOD(i) == 1)
    {
        strcpy(Modifier, "Shift");
    }
    else if (FLOAT_TO_MOD(i) == 2)
    {
        strcpy(Modifier, "Ctrl");
    }
    else if (FLOAT_TO_MOD(i) == 3)
    {
        strcpy(Modifier, "Alt");
    }
    else if (FLOAT_TO_MOD(i) == 4)
    {
        strcpy(Modifier, "Super");
    }
    else if (FLOAT_TO_MOD(i) == 5)
    {
        strcpy(Modifier, "ShiftCtrl");
    }
    else if (FLOAT_TO_MOD(i) == 6)
    {
        strcpy(Modifier, "ShiftAlt");
    }
    else if (FLOAT_TO_MOD(i) == 7)
    {
        strcpy(Modifier, "ShiftSuper");
    }
    else if (FLOAT_TO_MOD(i) == 8)
    {
        strcpy(Modifier, "CtrlAlt");
    }
    else if (FLOAT_TO_MOD(i) == 9)
    {
        strcpy(Modifier, "CtrlSuper");
    }
    else if (FLOAT_TO_MOD(i) == 10)
    {
        strcpy(Modifier, "AltSuper");
    }
    else if (FLOAT_TO_MOD(i) == 11)
    {
        strcpy(Modifier, "ShiftCtrlAlt");
    }
    else if (FLOAT_TO_MOD(i) == 12)
    {
        strcpy(Modifier, "ShiftCtrlSuper");
    }
    else if (FLOAT_TO_MOD(i) == 13)
    {
        strcpy(Modifier, "ShiftAltSuper");
    }
    else if (FLOAT_TO_MOD(i) == 14)
    {
        strcpy(Modifier, "CtrlAltSuper");
    }
    else if (FLOAT_TO_MOD(i) == 15)
    {
        strcpy(Modifier, "ShiftCtrlAltSuper");
    }
    return Modifier;
}

void InitAscii2KeyCodeTable(){};

#else
//mac

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Define keycodes only found in OSX >= 10.5 for older versions
 */
#if MAC_OS_X_VERSION_MAX_ALLOWED <= 1040
#define kVK_ANSI_Keypad0        0x52
#define kVK_ANSI_Keypad1        0x53
#define kVK_ANSI_Keypad2        0x54
#define kVK_ANSI_Keypad3        0x55
#define kVK_ANSI_Keypad4        0x56
#define kVK_ANSI_Keypad5        0x57
#define kVK_ANSI_Keypad6        0x58
#define kVK_ANSI_Keypad7        0x59
#define kVK_ANSI_Keypad8        0x5b
#define kVK_ANSI_Keypad9        0x5c
#define kVK_ANSI_KeypadDecimal  0x41
#define kVK_ANSI_KeypadDivide   0x4b
#define kVK_ANSI_KeypadEnter    0x4c
#define kVK_ANSI_KeypadMinus    0x4e
#define kVK_ANSI_KeypadMultiply 0x43
#define kVK_ANSI_KeypadPlus     0x45
#define kVK_Control             0x3b
#define kVK_Delete              0x33
#define kVK_DownArrow           0x7d
#define kVK_End                 0x77
#define kVK_Escape              0x35
#define kVK_F1                  0x7a
#define kVK_F10                 0x6d
#define kVK_F11                 0x67
#define kVK_F12                 0x6f
#define kVK_F2                  0x78
#define kVK_F3                  0x63
#define kVK_F4                  0x76
#define kVK_F5                  0x60
#define kVK_F6                  0x61
#define kVK_F7                  0x62
#define kVK_F8                  0x64
#define kVK_F9                  0x65
#define kVK_ForwardDelete       0x75
#define kVK_Help                0x72
#define kVK_Home                0x73
#define kVK_LeftArrow           0x7b
#define kVK_Option              0x3a
#define kVK_PageDown            0x79
#define kVK_PageUp              0x74
#define kVK_Return              0x24
#define kVK_RightArrow          0x7c
#define kVK_Shift               0x38
#define kVK_Tab                 0x30
#define kVK_UpArrow             0x7e
#define kVK_Space               0x31
#define kVK_Command             0x37
#endif /* MAC_OS_X_VERSION_MAX_ALLOWED <= 1040 */

char* getKeyName(unsigned int keycode)
{
    char* text;
    text = new char[16];
    if (keycode == 8)
    {
        strcpy(text, "DELETE"); //backspace
    }
    else if (keycode == 9)
    {
        strcpy(text, "TAB");
    }
    else if (keycode == 13)
    {
        strcpy(text, "RETURN");
    }
    else if (keycode == 19)
    {
        strcpy(text, "PAUSE");
    }
    else if (keycode == 27)
    {
        strcpy(text, "ESCAPE");
    }

    //28-30 for mouse buttons
    else if (keycode == 0x01)
    {
        strcpy(text, "(reserved)"); //"Left Click");
    }
    else if (keycode == 0x02)
    {
        strcpy(text, "(reserved)"); //"Right Click");
    }
    else if (keycode == 0x04)
    {
        strcpy(text, "(reserved)"); //"Middle Click");
    }

    else if (keycode == 0x5D)
    {
        strcpy(text, "HELP"); //"MENU");
    }
    else if (keycode == 32)
    {
        strcpy(text, "SPACE");
    }
    else if (keycode == 33)
    {
        strcpy(text, "PAGE UP");
    }
    else if (keycode == 34)
    {
        strcpy(text, "PAGE DOWN");
    }
    else if (keycode == 35)
    {
        strcpy(text, "END");
    }
    else if (keycode == 36)
    {
        strcpy(text, "HOME");
    }
    else if (keycode == 37)
    {
        strcpy(text, "LEFT");
    }
    else if (keycode == 38)
    {
        strcpy(text, "UP");
    }
    else if (keycode == 39)
    {
        strcpy(text, "RIGHT");
    }
    else if (keycode == 40)
    {
        strcpy(text, "DOWN");
    }
    else if (keycode == 0xDB)
    {
        strcpy(text, "[ {");
    }
    else if (keycode == 0xDC)
    {
        strcpy(text, "\\ |");
    }
    else if (keycode == 0xDD)
    {
        strcpy(text, "] }");
    }
    else if (keycode == 44)
    {
        strcpy(text, "(reserved)"); //"PRINT SCREEN");
    }
    else if (keycode == 45)
    {
        strcpy(text, "INSERT");
    }
    else if (keycode == 46)
    {
        strcpy(text, "FORWARD DELETE");
    }
    else if (keycode == 0xC0)
    {
        strcpy(text, "` ~");
    }
    else if (keycode == 48)
    {
        strcpy(text, "0 )");
    }
    else if (keycode == 49)
    {
        strcpy(text, "1 !");
    }
    else if (keycode == 50)
    {
        strcpy(text, "2 @");
    }
    else if (keycode == 51)
    {
        strcpy(text, "3 #");
    }
    else if (keycode == 52)
    {
        strcpy(text, "4 $");
    }
    else if (keycode == 53)
    {
        strcpy(text, "5 %");
    }
    else if (keycode == 54)
    {
        strcpy(text, "6 ^");
    }
    else if (keycode == 55)
    {
        strcpy(text, "7 &");
    }
    else if (keycode == 56)
    {
        strcpy(text, "8 *");
    }
    else if (keycode == 57)
    {
        strcpy(text, "9 (");
    }
    else if (keycode == 0xBD)
    {
        strcpy(text, "- _");
    }
    else if (keycode == 0xBB)
    {
        strcpy(text, "= +");
    }
    else if (keycode == 0xBC)
    {
        strcpy(text, ", <");
    }
    else if (keycode == 0xBE)
    {
        strcpy(text, ". >");
    }
    else if (keycode == 0xBF)
    {
        strcpy(text, "/ ?");
    }
    else if (keycode == 0xBA)
    {
        strcpy(text, "; :");
    }
    else if (keycode == 0xDE)
    {
        strcpy(text, "' \"");
    }

    else if (65 <= keycode && keycode <= 90)
    {
        sprintf(text, "%c", keycode); //letters
    }

    else if (keycode == 0x14)
    {
        strcpy(text, "(reserved)"); //"CAPS LOCK");
    }
    else if (keycode == 0x10)
    {
        strcpy(text, "SHIFT");
    }
    else if (keycode == 0x12)
    {
        strcpy(text, "CONTROL");
    }
    else if (keycode == 0x11)
    {
        strcpy(text, "OPTION");
    }
    else if (keycode == 0x5B)
    {
        strcpy(text, "COMMAND");
    }
    else if (keycode == 96)
    {
        strcpy(text, "NUMPAD 0");
    }
    else if (keycode == 97)
    {
        strcpy(text, "NUMPAD 1");
    }
    else if (keycode == 98)
    {
        strcpy(text, "NUMPAD 2");
    }
    else if (keycode == 99)
    {
        strcpy(text, "NUMPAD 3");
    }
    else if (keycode == 100)
    {
        strcpy(text, "NUMPAD 4");
    }
    else if (keycode == 101)
    {
        strcpy(text, "NUMPAD 5");
    }
    else if (keycode == 102)
    {
        strcpy(text, "NUMPAD 6");
    }
    else if (keycode == 103)
    {
        strcpy(text, "NUMPAD 7");
    }
    else if (keycode == 104)
    {
        strcpy(text, "NUMPAD 8");
    }
    else if (keycode == 105)
    {
        strcpy(text, "NUMPAD 9");
    }
    else if (keycode == 106)
    {
        strcpy(text, "NUMPAD *");
    }
    else if (keycode == 107)
    {
        strcpy(text, "NUMPAD +");
    }
    else if (keycode == 0x90)
    {
        strcpy(text, "NUMPAD ENTER"); //"NUM LOCK");
    }
    else if (keycode == 109)
    {
        strcpy(text, "NUMPAD -");
    }
    else if (keycode == 110)
    {
        strcpy(text, "NUMPAD .");
    }
    else if (keycode == 111)
    {
        strcpy(text, "NUMPAD /");
    }
    else if (keycode == 112)
    {
        strcpy(text, "F1");
    }
    else if (keycode == 113)
    {
        strcpy(text, "F2");
    }
    else if (keycode == 114)
    {
        strcpy(text, "F3");
    }
    else if (keycode == 115)
    {
        strcpy(text, "F4");
    }
    else if (keycode == 116)
    {
        strcpy(text, "F5");
    }
    else if (keycode == 117)
    {
        strcpy(text, "F6");
    }
    else if (keycode == 118)
    {
        strcpy(text, "F7");
    }
    else if (keycode == 119)
    {
        strcpy(text, "F8");
    }
    else if (keycode == 120)
    {
        strcpy(text, "F9");
    }
    else if (keycode == 121)
    {
        strcpy(text, "F10");
    }
    else if (keycode == 122)
    {
        strcpy(text, "F11");
    }
    else if (keycode == 123)
    {
        strcpy(text, "F12");
    }
    else
    {
        sprintf(text, "keycode %d", keycode);
    }
    return text;
}

char* getModifierName(float i)
{
    char* Modifier;
    Modifier = new char[24];
    if (FLOAT_TO_MOD(i) == 0)
    {
        strcpy(Modifier, "None");
    }
    else if (FLOAT_TO_MOD(i) == 1)
    {
        strcpy(Modifier, "Shift");
    }
    else if (FLOAT_TO_MOD(i) == 2)
    {
        strcpy(Modifier, "Control");
    }
    else if (FLOAT_TO_MOD(i) == 3)
    {
        strcpy(Modifier, "Option");
    }
    else if (FLOAT_TO_MOD(i) == 4)
    {
        strcpy(Modifier, "Command");
    }
    else if (FLOAT_TO_MOD(i) == 5)
    {
        strcpy(Modifier, "ShiftCtrl");
    }
    else if (FLOAT_TO_MOD(i) == 6)
    {
        strcpy(Modifier, "ShiftOption");
    }
    else if (FLOAT_TO_MOD(i) == 7)
    {
        strcpy(Modifier, "ShiftCommand");
    }
    else if (FLOAT_TO_MOD(i) == 8)
    {
        strcpy(Modifier, "CtrlOption");
    }
    else if (FLOAT_TO_MOD(i) == 9)
    {
        strcpy(Modifier, "CtrlCommand");
    }
    else if (FLOAT_TO_MOD(i) == 10)
    {
        strcpy(Modifier, "OptionCommand");
    }
    else if (FLOAT_TO_MOD(i) == 11)
    {
        strcpy(Modifier, "ShiftCtrlOption");
    }
    else if (FLOAT_TO_MOD(i) == 12)
    {
        strcpy(Modifier, "ShiftCtrlCommand");
    }
    else if (FLOAT_TO_MOD(i) == 13)
    {
        strcpy(Modifier, "ShiftOptionCommand");
    }
    else if (FLOAT_TO_MOD(i) == 14)
    {
        strcpy(Modifier, "CtrlOptionCommand");
    }
    else if (FLOAT_TO_MOD(i) == 15)
    {
        strcpy(Modifier, "ShiftCtrlOptionCommand");
    }
    return Modifier;
}

CGKeyCode keyNameToKeyCode(char* character)
{
    if (! strcmp(character, "RETURN"))
    {
        return kVK_Return;
    }
    if (! strcmp(character, "TAB"))
    {
        return kVK_Tab;
    }
    if (! strcmp(character, "SPACE"))
    {
        return kVK_Space;
    }
    if (! strcmp(character, "DELETE"))
    {
        return kVK_Delete;
    }
    if (! strcmp(character, "ESCAPE"))
    {
        return kVK_Escape;
    }
    if (! strcmp(character, "F1"))
    {
        return kVK_F1;
    }
    if (! strcmp(character, "F2"))
    {
        return kVK_F2;
    }
    if (! strcmp(character, "F3"))
    {
        return kVK_F3;
    }
    if (! strcmp(character, "F4"))
    {
        return kVK_F4;
    }
    if (! strcmp(character, "F5"))
    {
        return kVK_F5;
    }
    if (! strcmp(character, "F6"))
    {
        return kVK_F6;
    }
    if (! strcmp(character, "F7"))
    {
        return kVK_F7;
    }
    if (! strcmp(character, "F8"))
    {
        return kVK_F8;
    }
    if (! strcmp(character, "F9"))
    {
        return kVK_F9;
    }
    if (! strcmp(character, "F10"))
    {
        return kVK_F10;
    }
    if (! strcmp(character, "F11"))
    {
        return kVK_F11;
    }
    if (! strcmp(character, "F12"))
    {
        return kVK_F12;
    }
    if (! strcmp(character, "HELP"))
    {
        return kVK_Help;
    }
    if (! strcmp(character, "HOME"))
    {
        return kVK_Home;
    }
    if (! strcmp(character, "PAGE UP"))
    {
        return kVK_PageUp;
    }
    if (! strcmp(character, "FORWARD DELETE"))
    {
        return kVK_ForwardDelete;
    }
    if (! strcmp(character, "END"))
    {
        return kVK_End;
    }
    if (! strcmp(character, "PAGE DOWN"))
    {
        return kVK_PageDown;
    }
    if (! strcmp(character, "LEFT"))
    {
        return kVK_LeftArrow;
    }
    if (! strcmp(character, "RIGHT"))
    {
        return kVK_RightArrow;
    }
    if (! strcmp(character, "DOWN"))
    {
        return kVK_DownArrow;
    }
    if (! strcmp(character, "UP"))
    {
        return kVK_UpArrow;
    }
    if (! strcmp(character, "NUMPAD 0"))
    {
        return kVK_ANSI_Keypad0;
    }
    if (! strcmp(character, "NUMPAD 1"))
    {
        return kVK_ANSI_Keypad1;
    }
    if (! strcmp(character, "NUMPAD 2"))
    {
        return kVK_ANSI_Keypad2;
    }
    if (! strcmp(character, "NUMPAD 3"))
    {
        return kVK_ANSI_Keypad3;
    }
    if (! strcmp(character, "NUMPAD 4"))
    {
        return kVK_ANSI_Keypad4;
    }
    if (! strcmp(character, "NUMPAD 5"))
    {
        return kVK_ANSI_Keypad5;
    }
    if (! strcmp(character, "NUMPAD 6"))
    {
        return kVK_ANSI_Keypad6;
    }
    if (! strcmp(character, "NUMPAD 7"))
    {
        return kVK_ANSI_Keypad7;
    }
    if (! strcmp(character, "NUMPAD 8"))
    {
        return kVK_ANSI_Keypad8;
    }
    if (! strcmp(character, "NUMPAD 9"))
    {
        return kVK_ANSI_Keypad9;
    }
    if (! strcmp(character, "NUMPAD ."))
    {
        return kVK_ANSI_KeypadDecimal;
    }
    if (! strcmp(character, "NUMPAD /"))
    {
        return kVK_ANSI_KeypadDivide;
    }
    if (! strcmp(character, "NUMPAD ENTER"))
    {
        return kVK_ANSI_KeypadEnter;
    }
    if (! strcmp(character, "NUMPAD -"))
    {
        return kVK_ANSI_KeypadMinus;
    }
    if (! strcmp(character, "NUMPAD *"))
    {
        return kVK_ANSI_KeypadMultiply;
    }
    if (! strcmp(character, "NUMPAD +"))
    {
        return kVK_ANSI_KeypadPlus;
    }
    if (! strcmp(character, "SHIFT"))
    {
        return kVK_Shift;
    }
    if (! strcmp(character, "OPTION"))
    {
        return kVK_Option;
    }
    if (! strcmp(character, "CONTROL"))
    {
        return kVK_Control;
    }
    if (! strcmp(character, "COMMAND"))
    {
        return kVK_Command;
    }
    if (! strcmp(character, "(reserved)"))
    {
        return -1;
    }
    if (! strcmp(character, "[ {"))
    {
        return kVK_ANSI_LeftBracket;
    }
    if (! strcmp(character, "\\ |"))
    {
        return kVK_ANSI_Backslash;
    }
    if (! strcmp(character, "] }"))
    {
        return kVK_ANSI_RightBracket;
    }
    if (! strcmp(character, "` ~"))
    {
        return kVK_ANSI_Grave;
    }
    if (! strcmp(character, "0 )"))
    {
        return kVK_ANSI_0;
    }
    if (! strcmp(character, "1 !"))
    {
        return kVK_ANSI_1;
    }
    if (! strcmp(character, "2 @"))
    {
        return kVK_ANSI_2;
    }
    if (! strcmp(character, "3 #"))
    {
        return kVK_ANSI_3;
    }
    if (! strcmp(character, "4 $"))
    {
        return kVK_ANSI_4;
    }
    if (! strcmp(character, "5 %"))
    {
        return kVK_ANSI_5;
    }
    if (! strcmp(character, "6 ^"))
    {
        return kVK_ANSI_6;
    }
    if (! strcmp(character, "7 &"))
    {
        return kVK_ANSI_7;
    }
    if (! strcmp(character, "8 *"))
    {
        return kVK_ANSI_8;
    }
    if (! strcmp(character, "9 ("))
    {
        return kVK_ANSI_9;
    }
    if (! strcmp(character, "- _"))
    {
        return kVK_ANSI_Minus;
    }
    if (! strcmp(character, "= +"))
    {
        return kVK_ANSI_Equal;
    }
    if (! strcmp(character, ", <"))
    {
        return kVK_ANSI_Comma;
    }
    if (! strcmp(character, ". >"))
    {
        return kVK_ANSI_Period;
    }
    if (! strcmp(character, "/ ?"))
    {
        return kVK_ANSI_Slash;
    }
    if (! strcmp(character, "; :"))
    {
        return kVK_ANSI_Semicolon;
    }
    if (! strcmp(character, "' \""))
    {
        return kVK_ANSI_Quote;
    }
    if (! strcmp(character, "A"))
    {
        return kVK_ANSI_A;
    }
    if (! strcmp(character, "B"))
    {
        return kVK_ANSI_B;
    }
    if (! strcmp(character, "C"))
    {
        return kVK_ANSI_C;
    }
    if (! strcmp(character, "D"))
    {
        return kVK_ANSI_D;
    }
    if (! strcmp(character, "E"))
    {
        return kVK_ANSI_E;
    }
    if (! strcmp(character, "F"))
    {
        return kVK_ANSI_F;
    }
    if (! strcmp(character, "G"))
    {
        return kVK_ANSI_G;
    }
    if (! strcmp(character, "H"))
    {
        return kVK_ANSI_H;
    }
    if (! strcmp(character, "I"))
    {
        return kVK_ANSI_I;
    }
    if (! strcmp(character, "J"))
    {
        return kVK_ANSI_J;
    }
    if (! strcmp(character, "K"))
    {
        return kVK_ANSI_K;
    }
    if (! strcmp(character, "L"))
    {
        return kVK_ANSI_L;
    }
    if (! strcmp(character, "M"))
    {
        return kVK_ANSI_M;
    }
    if (! strcmp(character, "N"))
    {
        return kVK_ANSI_N;
    }
    if (! strcmp(character, "O"))
    {
        return kVK_ANSI_O;
    }
    if (! strcmp(character, "P"))
    {
        return kVK_ANSI_P;
    }
    if (! strcmp(character, "Q"))
    {
        return kVK_ANSI_Q;
    }
    if (! strcmp(character, "R"))
    {
        return kVK_ANSI_R;
    }
    if (! strcmp(character, "S"))
    {
        return kVK_ANSI_S;
    }
    if (! strcmp(character, "T"))
    {
        return kVK_ANSI_T;
    }
    if (! strcmp(character, "U"))
    {
        return kVK_ANSI_U;
    }
    if (! strcmp(character, "V"))
    {
        return kVK_ANSI_V;
    }
    if (! strcmp(character, "W"))
    {
        return kVK_ANSI_W;
    }
    if (! strcmp(character, "X"))
    {
        return kVK_ANSI_X;
    }
    if (! strcmp(character, "Y"))
    {
        return kVK_ANSI_Y;
    }
    if (! strcmp(character, "Z"))
    {
        return kVK_ANSI_Z;
    }

    return -1;
}

#endif

#endif
