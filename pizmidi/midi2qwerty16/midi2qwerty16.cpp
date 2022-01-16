#if _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#else
//#include "/System/Library/Frameworks/ApplicationServices.framework/Versions/A/Frameworks/CoreGraphics.framework/Versions/A/Headers/CGEvent.h"
//#include "/System/Library/Frameworks/Carbon.framework/Versions/A/Frameworks/HIToolbox.framework/Versions/A/Headers/Events.h"
#endif

#include "keys.h"
#include "midi2qwerty16.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new midi2qwerty (audioMaster);
}

#if _WIN32

// this way works for the window with focus:
void midi2qwerty::key2 (int vkeycode, int modifiers, bool keyup, bool instantaneous)
{
    int mods = wxMOD_NONE;
    if (modifiers == 0)
        mods = wxMOD_NONE;
    else if (modifiers == 1)
        mods = wxMOD_SHIFT;
    else if (modifiers == 2)
        mods = wxMOD_CONTROL;
    else if (modifiers == 3)
        mods = wxMOD_ALT;
    else if (modifiers == 4)
        mods = wxMOD_META;
    else if (modifiers == 5)
        mods = wxMOD_SHIFT | wxMOD_CONTROL;
    else if (modifiers == 6)
        mods = wxMOD_SHIFT | wxMOD_ALT;
    else if (modifiers == 7)
        mods = wxMOD_SHIFT | wxMOD_META;
    else if (modifiers == 8)
        mods = wxMOD_ALT | wxMOD_CONTROL;
    else if (modifiers == 9)
        mods = wxMOD_META | wxMOD_CONTROL;
    else if (modifiers == 10)
        mods = wxMOD_ALT | wxMOD_META;
    else if (modifiers == 11)
        mods = wxMOD_SHIFT | wxMOD_CONTROL | wxMOD_ALT;
    else if (modifiers == 12)
        mods = wxMOD_SHIFT | wxMOD_CONTROL | wxMOD_META;
    else if (modifiers == 13)
        mods = wxMOD_SHIFT | wxMOD_ALT | wxMOD_META;
    else if (modifiers == 14)
        mods = wxMOD_ALT | wxMOD_CONTROL | wxMOD_META;
    else if (modifiers == 15)
        mods = wxMOD_ALL;

    bool mousebutton = false;
    if (vkeycode == 0x01)
    {
        mousebutton = true;
        if (instantaneous)
            vkeycode = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
        else if (! keyup)
            vkeycode = MOUSEEVENTF_LEFTDOWN;
        else if (keyup)
            vkeycode = MOUSEEVENTF_LEFTUP;
    }
    else if (vkeycode == 0x02)
    {
        mousebutton = true;
        if (instantaneous)
            vkeycode = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
        else if (! keyup)
            vkeycode = MOUSEEVENTF_RIGHTDOWN;
        else if (keyup)
            vkeycode = MOUSEEVENTF_RIGHTUP;
    }
    else if (vkeycode == 0x04)
    {
        mousebutton = true;
        if (instantaneous)
            vkeycode = MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MIDDLEUP;
        else if (! keyup)
            vkeycode = MOUSEEVENTF_MIDDLEDOWN;
        else if (keyup)
            vkeycode = MOUSEEVENTF_MIDDLEUP;
    }
    int X = 0;
    int Y = 0;

    if (instantaneous)
    {
        if ((mods & wxMOD_SHIFT) == wxMOD_SHIFT)
            keybd_event (VK_SHIFT, 0, 0, 0);
        if ((mods & wxMOD_ALT) == wxMOD_ALT)
            keybd_event (VK_MENU, 0, 0, 0);
        if ((mods & wxMOD_CONTROL) == wxMOD_CONTROL)
            keybd_event (VK_CONTROL, 0, 0, 0);
        if ((mods & wxMOD_META) == wxMOD_META)
            keybd_event (VK_LWIN, 0, 0, 0);
        if (mousebutton)
            mouse_event (vkeycode, X, Y, 0, 0);
        else
        {
            keystroke (vkeycode);
            //keybd_event(vkeycode,0,0,0);
            //keybd_event(vkeycode,0,KEYEVENTF_KEYUP,0);
        }
        if ((mods & wxMOD_CONTROL) == wxMOD_CONTROL)
            keybd_event (VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
        if ((mods & wxMOD_ALT) == wxMOD_ALT)
            keybd_event (VK_MENU, 0, KEYEVENTF_KEYUP, 0);
        if ((mods & wxMOD_SHIFT) == wxMOD_SHIFT)
            keybd_event (VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        if ((mods & wxMOD_META) == wxMOD_META)
            keybd_event (VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
    }
    else if (! keyup)
    {
        if ((mods & wxMOD_SHIFT) == wxMOD_SHIFT)
            keybd_event (VK_SHIFT, 0, 0, 0);
        if ((mods & wxMOD_ALT) == wxMOD_ALT)
            keybd_event (VK_MENU, 0, 0, 0);
        if ((mods & wxMOD_CONTROL) == wxMOD_CONTROL)
            keybd_event (VK_CONTROL, 0, 0, 0);
        if ((mods & wxMOD_META) == wxMOD_META)
            keybd_event (VK_LWIN, 0, 0, 0);
        if (mousebutton)
            mouse_event (vkeycode, X, Y, 0, 0);
        else
            keybd_event (vkeycode, 0, 0, 0);
    }
    else if (keyup)
    {
        if (mousebutton)
            mouse_event (vkeycode, X, Y, 0, 0);
        else
            keybd_event (vkeycode, 0, KEYEVENTF_KEYUP, 0);
        if ((mods & wxMOD_CONTROL) == wxMOD_CONTROL)
            keybd_event (VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
        if ((mods & wxMOD_ALT) == wxMOD_ALT)
            keybd_event (VK_MENU, 0, KEYEVENTF_KEYUP, 0);
        if ((mods & wxMOD_SHIFT) == wxMOD_SHIFT)
            keybd_event (VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        if ((mods & wxMOD_META) == wxMOD_META)
            keybd_event (VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
    }
}

//SendInput method
void midi2qwerty::keystroke (int vkeycode)
{
    /*
	HWND w = GetActiveWindow();
	if (!w) w = GetForegroundWindow();
	if (w) {SendMessage(w,WM_KEYDOWN,vkeycode,0);SendMessage(w,WM_KEYUP,vkeycode,0);}
*/
    INPUT input[2];
    memset (input, 0, sizeof (input));
    input[0].type = INPUT_KEYBOARD;

    input[0].ki.wVk         = vkeycode;
    input[0].ki.dwFlags     = 0;
    input[0].ki.time        = 0;
    input[0].ki.dwExtraInfo = 0;

    input[1].ki.wVk         = vkeycode;
    input[1].ki.dwFlags     = KEYEVENTF_KEYUP;
    input[1].ki.time        = 0;
    input[1].ki.dwExtraInfo = 0;

    SendInput (2, input, sizeof (INPUT));
    //*/
}

/* 
void mouseclick(int button) {

    INPUT input[2];
    memset(input, 0, sizeof(input));
    input[0].type = INPUT_KEYBOARD;

    input[0].ki.wVk = vkeycode;
    input[0].ki.dwFlags = 0;
    input[0].ki.time = 0;
    input[0].ki.dwExtraInfo = 0;
    
    input[1].ki.wVk = vkeycode;
    input[1].ki.dwFlags = WM_LBUTTONUP;
    input[1].ki.time = 0;
    input[1].ki.dwExtraInfo = 0;
    
    SendInput(2,input,sizeof(INPUT));
}
*/

#elif defined(__linux__)

void midi2qwerty::key2 (int vkeycode, int modifiers, bool keyup, bool instantaneous)
{
    int wxMODS = wxMOD_NONE;
    if (modifiers == 0)
        wxMODS = wxMOD_NONE;
    else if (modifiers == 1)
        wxMODS = wxMOD_SHIFT;
    else if (modifiers == 2)
        wxMODS = wxMOD_CONTROL;
    else if (modifiers == 3)
        wxMODS = wxMOD_ALT;
    else if (modifiers == 4)
        wxMODS = wxMOD_META;
    else if (modifiers == 5)
        wxMODS = wxMOD_SHIFT | wxMOD_CONTROL;
    else if (modifiers == 6)
        wxMODS = wxMOD_SHIFT | wxMOD_ALT;
    else if (modifiers == 7)
        wxMODS = wxMOD_SHIFT | wxMOD_META;
    else if (modifiers == 8)
        wxMODS = wxMOD_ALT | wxMOD_CONTROL;
    else if (modifiers == 9)
        wxMODS = wxMOD_META | wxMOD_CONTROL;
    else if (modifiers == 10)
        wxMODS = wxMOD_ALT | wxMOD_META;
    else if (modifiers == 11)
        wxMODS = wxMOD_SHIFT | wxMOD_CONTROL | wxMOD_ALT;
    else if (modifiers == 12)
        wxMODS = wxMOD_SHIFT | wxMOD_CONTROL | wxMOD_META;
    else if (modifiers == 13)
        wxMODS = wxMOD_SHIFT | wxMOD_ALT | wxMOD_META;
    else if (modifiers == 14)
        wxMODS = wxMOD_ALT | wxMOD_CONTROL | wxMOD_META;
    else if (modifiers == 15)
        wxMODS = wxMOD_ALL;

    Display* display = XOpenDisplay (NULL);

    if (instantaneous)
    {
        if ((wxMODS & wxMOD_SHIFT) == wxMOD_SHIFT)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Shift_L), True, 0);
        if ((wxMODS & wxMOD_ALT) == wxMOD_ALT)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Alt_L), True, 0);
        if ((wxMODS & wxMOD_CONTROL) == wxMOD_CONTROL)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Control_L), True, 0);
        if ((wxMODS & wxMOD_META) == wxMOD_META)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Super_L), True, 0);
        XTestFakeKeyEvent (display, XKeysymToKeycode (display, WINVK_TO_LINXK (vkeycode)), True, 0);
        XTestFakeKeyEvent (display, XKeysymToKeycode (display, WINVK_TO_LINXK (vkeycode)), False, 0);
        if ((wxMODS & wxMOD_CONTROL) == wxMOD_CONTROL)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Control_L), False, 0);
        if ((wxMODS & wxMOD_ALT) == wxMOD_ALT)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Alt_L), False, 0);
        if ((wxMODS & wxMOD_SHIFT) == wxMOD_SHIFT)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Shift_L), False, 0);
        if ((wxMODS & wxMOD_META) == wxMOD_META)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Super_L), False, 0);
    }
    else if (! keyup)
    {
        if ((wxMODS & wxMOD_SHIFT) == wxMOD_SHIFT)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Shift_L), True, 0);
        if ((wxMODS & wxMOD_ALT) == wxMOD_ALT)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Alt_L), True, 0);
        if ((wxMODS & wxMOD_CONTROL) == wxMOD_CONTROL)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Control_L), True, 0);
        if ((wxMODS & wxMOD_META) == wxMOD_META)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Super_L), True, 0);
        XTestFakeKeyEvent (display, XKeysymToKeycode (display, WINVK_TO_LINXK (vkeycode)), True, 0);
    }
    else if (keyup)
    {
        XTestFakeKeyEvent (display, XKeysymToKeycode (display, WINVK_TO_LINXK (vkeycode)), False, 0);
        if ((wxMODS & wxMOD_CONTROL) == wxMOD_CONTROL)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Control_L), False, 0);
        if ((wxMODS & wxMOD_ALT) == wxMOD_ALT)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Alt_L), False, 0);
        if ((wxMODS & wxMOD_SHIFT) == wxMOD_SHIFT)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Shift_L), False, 0);
        if ((wxMODS & wxMOD_META) == wxMOD_META)
            XTestFakeKeyEvent (display, XKeysymToKeycode (display, XK_Super_L), False, 0);
    }

    XCloseDisplay (display);
}
#else
//mac

void midi2qwerty::key2 (int vkeycode, int modifiers, bool keyup, bool instantaneous)
{
    int wxMODS = wxMOD_NONE;
    if (modifiers == 0)
        wxMODS = wxMOD_NONE;
    else if (modifiers == 1)
        wxMODS = wxMOD_SHIFT;
    else if (modifiers == 2)
        wxMODS = wxMOD_CONTROL;
    else if (modifiers == 3)
        wxMODS = wxMOD_ALT;
    else if (modifiers == 4)
        wxMODS = wxMOD_META;
    else if (modifiers == 5)
        wxMODS = wxMOD_SHIFT | wxMOD_CONTROL;
    else if (modifiers == 6)
        wxMODS = wxMOD_SHIFT | wxMOD_ALT;
    else if (modifiers == 7)
        wxMODS = wxMOD_SHIFT | wxMOD_META;
    else if (modifiers == 8)
        wxMODS = wxMOD_ALT | wxMOD_CONTROL;
    else if (modifiers == 9)
        wxMODS = wxMOD_META | wxMOD_CONTROL;
    else if (modifiers == 10)
        wxMODS = wxMOD_ALT | wxMOD_META;
    else if (modifiers == 11)
        wxMODS = wxMOD_SHIFT | wxMOD_CONTROL | wxMOD_ALT;
    else if (modifiers == 12)
        wxMODS = wxMOD_SHIFT | wxMOD_CONTROL | wxMOD_META;
    else if (modifiers == 13)
        wxMODS = wxMOD_SHIFT | wxMOD_ALT | wxMOD_META;
    else if (modifiers == 14)
        wxMODS = wxMOD_ALT | wxMOD_CONTROL | wxMOD_META;
    else if (modifiers == 15)
        wxMODS = wxMOD_ALL;

    int keyCode = keyNameToKeyCode (getKeyName (vkeycode));

    if (keyCode != -1)
    {
        if (instantaneous)
        {
            if ((wxMODS & wxMOD_SHIFT) == wxMOD_SHIFT)
                CGPostKeyboardEvent (NULL, kVK_Shift, true);
            if ((wxMODS & wxMOD_ALT) == wxMOD_ALT)
                CGPostKeyboardEvent (NULL, kVK_Option, true);
            if ((wxMODS & wxMOD_CONTROL) == wxMOD_CONTROL)
                CGPostKeyboardEvent (NULL, kVK_Control, true);
            if ((wxMODS & wxMOD_META) == wxMOD_META)
                CGPostKeyboardEvent (NULL, kVK_Command, true);
            else
            {
                CGPostKeyboardEvent (NULL, keyCode, true);
                CGPostKeyboardEvent (NULL, keyCode, false);
            }
            if ((wxMODS & wxMOD_SHIFT) == wxMOD_SHIFT)
                CGPostKeyboardEvent (NULL, kVK_Shift, false);
            if ((wxMODS & wxMOD_ALT) == wxMOD_ALT)
                CGPostKeyboardEvent (NULL, kVK_Option, false);
            if ((wxMODS & wxMOD_CONTROL) == wxMOD_CONTROL)
                CGPostKeyboardEvent (NULL, kVK_Control, false);
            if ((wxMODS & wxMOD_META) == wxMOD_META)
                CGPostKeyboardEvent (NULL, kVK_Command, false);
        }
        else if (! keyup)
        {
            if ((wxMODS & wxMOD_SHIFT) == wxMOD_SHIFT)
                CGPostKeyboardEvent (NULL, kVK_Shift, true);
            if ((wxMODS & wxMOD_ALT) == wxMOD_ALT)
                CGPostKeyboardEvent (NULL, kVK_Option, true);
            if ((wxMODS & wxMOD_CONTROL) == wxMOD_CONTROL)
                CGPostKeyboardEvent (NULL, kVK_Control, true);
            if ((wxMODS & wxMOD_META) == wxMOD_META)
                CGPostKeyboardEvent (NULL, kVK_Command, true);
            CGPostKeyboardEvent (0, keyCode, true);
        }
        else if (keyup)
        {
            CGPostKeyboardEvent (0, keyCode, false);
            if ((wxMODS & wxMOD_SHIFT) == wxMOD_SHIFT)
                CGPostKeyboardEvent (NULL, kVK_Shift, false);
            if ((wxMODS & wxMOD_ALT) == wxMOD_ALT)
                CGPostKeyboardEvent (NULL, kVK_Option, false);
            if ((wxMODS & wxMOD_CONTROL) == wxMOD_CONTROL)
                CGPostKeyboardEvent (NULL, kVK_Control, false);
            if ((wxMODS & wxMOD_META) == wxMOD_META)
                CGPostKeyboardEvent (NULL, kVK_Command, false);
        }
    }
}

#endif

midi2qwertyProgram::midi2qwertyProgram()
{
    // default Program Values
    for (int i = 0; i < kNumSlots; i++)
    {
        param[i * 4]     = 0.0;                       //msg
        param[i * 4 + 1] = MIDI_TO_FLOAT (i);         //note
        param[i * 4 + 2] = MOD_TO_FLOAT (0);          //mods
        param[i * 4 + 3] = KEYCODE_TO_FLOAT (96 + i); //key
    }
    param[kChannel] = CHANNEL_TO_FLOAT016 (0); //0==any channel
    strcpy (name, "default");
}

//-----------------------------------------------------------------------------
midi2qwerty::midi2qwerty (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams), programs (0)
{
    for (int i = 0; i < 16; i++)
        lastPC[i] = 0;

    programs = new midi2qwertyProgram[numPrograms];

    //make built-in programs
    if (programs)
    {
        CFxBank* defaultBank = new CFxBank (kNumPrograms, kNumParams);
        if (readDefaultBank (PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    for (int p = 0; p < kNumParams; p++)
                    {
                        programs[i].param[p] = defaultBank->GetProgParm (i, p);
                    }
                    strcpy (programs[i].name, defaultBank->GetProgramName (i));
                }
            }
        }
        else
        {
            // built-in programs
            for (int i = 0; i < kNumPrograms; i++)
            {
                switch (i)
                {
                    case 0:
                        programs[i].param[kMsg1]  = 0.15f;
                        programs[i].param[kNote1] = MIDI_TO_FLOAT (0);
                        programs[i].param[kKey1]  = KEYCODE_TO_FLOAT (25); //return
                        programs[i].param[kMsg2]  = 0.15f;
                        programs[i].param[kNote2] = MIDI_TO_FLOAT (1.1);
                        programs[i].param[kKey2]  = KEYCODE_TO_FLOAT (96); //num0
                        programs[i].param[kMsg3]  = 0.15f;
                        programs[i].param[kNote3] = MIDI_TO_FLOAT (2.1);
                        programs[i].param[kKey3]  = KEYCODE_TO_FLOAT (99); //num3
                        programs[i].param[kMsg4]  = 0.15f;
                        programs[i].param[kNote4] = MIDI_TO_FLOAT (3.1);
                        programs[i].param[kKey4]  = KEYCODE_TO_FLOAT (106); //num*
                        sprintf (programs[i].name, "Notes->XT2 transport");
                        break;
                    case 1:
                        programs[i].param[kNote1] = MIDI_TO_FLOAT (0);
                        programs[i].param[kMsg1]  = 0.45f;
                        programs[i].param[kKey1]  = KEYCODE_TO_FLOAT (25); //return
                        programs[i].param[kNote2] = MIDI_TO_FLOAT (1.1);
                        programs[i].param[kMsg2]  = 0.45f;
                        programs[i].param[kKey2]  = KEYCODE_TO_FLOAT (96); //num0
                        programs[i].param[kNote3] = MIDI_TO_FLOAT (2.1);
                        programs[i].param[kMsg3]  = 0.45f;
                        programs[i].param[kKey3]  = KEYCODE_TO_FLOAT (99); //num3
                        programs[i].param[kNote4] = MIDI_TO_FLOAT (3.1);
                        programs[i].param[kMsg4]  = 0.45f;
                        programs[i].param[kKey4]  = KEYCODE_TO_FLOAT (106); //num*
                        sprintf (programs[i].name, "CCs->XT2 transport");
                        break;
                    case 2:
                        programs[i].param[kNote1] = MIDI_TO_FLOAT (0);
                        programs[i].param[kMsg1]  = 0.15f;
                        programs[i].param[kKey1]  = KEYCODE_TO_FLOAT (45); //insert
                        programs[i].param[kMod1]  = MOD_TO_FLOAT (2);
                        programs[i].param[kNote2] = MIDI_TO_FLOAT (1.1);
                        programs[i].param[kMsg2]  = 0.15f;
                        programs[i].param[kKey2]  = KEYCODE_TO_FLOAT (97); //num1
                        programs[i].param[kMod2]  = MOD_TO_FLOAT (0);
                        programs[i].param[kNote3] = MIDI_TO_FLOAT (2.1);
                        programs[i].param[kMsg3]  = 0.15f;
                        programs[i].param[kKey3]  = KEYCODE_TO_FLOAT ('Z'); //z
                        programs[i].param[kMod3]  = MOD_TO_FLOAT (2);
                        programs[i].param[kNote4] = MIDI_TO_FLOAT (3.1);
                        programs[i].param[kMsg4]  = 0.15f;
                        programs[i].param[kKey4]  = KEYCODE_TO_FLOAT (106); //num*
                        programs[i].param[kMod4]  = MOD_TO_FLOAT (0);
                        strcpy (programs[i].name, "stuff");
                        break;
                    case 3:
                        programs[i].param[kNote1] = MIDI_TO_FLOAT (1.1);
                        programs[i].param[kMsg1]  = 0.55f;
                        programs[i].param[kKey1]  = KEYCODE_TO_FLOAT (109); //num-
                        programs[i].param[kMod1]  = MOD_TO_FLOAT (0);
                        programs[i].param[kNote2] = MIDI_TO_FLOAT (1.1);
                        programs[i].param[kMsg2]  = 0.45f;
                        programs[i].param[kKey2]  = KEYCODE_TO_FLOAT (107); //num+
                        programs[i].param[kMod2]  = MOD_TO_FLOAT (0);
                        programs[i].param[kNote3] = MIDI_TO_FLOAT (2.1);
                        programs[i].param[kMsg3]  = 1.0f;
                        programs[i].param[kKey3]  = KEYCODE_TO_FLOAT (58); //,
                        programs[i].param[kMod3]  = MOD_TO_FLOAT (0);
                        programs[i].param[kNote4] = MIDI_TO_FLOAT (2.1);
                        programs[i].param[kMsg4]  = 0.85f;
                        programs[i].param[kKey4]  = KEYCODE_TO_FLOAT (59); //.
                        programs[i].param[kMod4]  = MOD_TO_FLOAT (0);
                        strcpy (programs[i].name, "zoom/scroll");
                        break;
                }
            }
        }
        setProgram (0);
    }

    for (int i = 0; i < kNumSlots; i++)
        keydown[i] = false;

    settingprog = false;
    //InitAscii2KeyCodeTable();
    init();
}

//-----------------------------------------------------------------------------------------
midi2qwerty::~midi2qwerty()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void midi2qwerty::setProgram (VstInt32 program)
{
    midi2qwertyProgram* ap = &programs[program];

    settingprog = true;
    curProgram  = program;
    for (int i = 0; i < kNumParams - 1; i++)
    {
        setParameter (i, ap->param[i]);
    }
    setParameter (kProg, (float) program / (float) (kNumPrograms - 1));
    updateDisplay();
    settingprog = false;
}

//------------------------------------------------------------------------
void midi2qwerty::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
    updateDisplay();
}

//------------------------------------------------------------------------
void midi2qwerty::getProgramName (char* name)
{
    strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool midi2qwerty::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void midi2qwerty::setParameter (VstInt32 index, float value)
{
    if (index < kNumParams)
    {
        midi2qwertyProgram* ap = &programs[curProgram];
        if (index % 4 == 0)
        {
            param[index] = ap->param[index] = value;
            updateDisplay();
        }
        else if (index == kProg)
        {
            param[kProg] = value;
            if (! settingprog)
            {
                setProgram ((int) (param[kProg] * (kNumPrograms - 1)));
                updateDisplay();
            }
        }
        else
            param[index] = ap->param[index] = value;
    }
}

//-----------------------------------------------------------------------------------------
float midi2qwerty::getParameter (VstInt32 index)
{
    if (index < kNumParams)
    {
        return param[index];
    }
    return 0;
}

//-----------------------------------------------------------------------------------------
void midi2qwerty::getParameterName (VstInt32 index, char* label)
{
    if (index < kNumParams)
    {
        int slot = (index / 4) + 1;
        int data = index % 4;
        if (index == kChannel)
            strcpy (label, "channel");
        else if (index == kProg)
            strcpy (label, "Program");
        else if (data == 0)
            sprintf (label, "%d: data", slot);
        else if (data == 1)
            strcpy (label, " note/cc");
        else if (data == 2)
            strcpy (label, " modifiers");
        else if (data == 3)
            strcpy (label, " key");
    }
}

//-----------------------------------------------------------------------------------------
void midi2qwerty::getParameterDisplay (VstInt32 index, char* text)
{
    if (index < kNumParams)
    {
        int slot = index / 4;
        int data = index % 4;
        if (data == 0)
        {
            if (param[index] < 0.1f)
                strcpy (text, "Off");
            else if (param[index] < 0.2f)
                strcpy (text, "Note");
            else if (param[index] < 0.3f)
                strcpy (text, "ProgCh +");
            else if (param[index] < 0.4f)
                strcpy (text, "ProgCh -");
            else if (param[index] < 0.5f)
                strcpy (text, "CC (127)");
            else if (param[index] < 0.6f)
                strcpy (text, "CC (0)");
            else if (param[index] < 0.7f)
                strcpy (text, "CC (>64)");
            else if (param[index] < 0.8f)
                strcpy (text, "CC (<64)");
            else if (param[index] < 0.9f)
                strcpy (text, "CC slider > 100");
            else
                strcpy (text, "CC slider < 27");
        }
        else if (data == 1)
        {
            if (param[slot * 4] >= 0.3)
                sprintf (text, "%d", FLOAT_TO_MIDI (param[index]));
            else if (param[slot * 4] >= 0.1)
                sprintf (text, "%d (%s)", FLOAT_TO_MIDI (param[index]), getNoteName (FLOAT_TO_MIDI (param[index]), bottomOctave));
            else
                strcpy (text, " ");
        }
        else if (data == 2)
        {
            if (param[slot * 4] >= 0.1)
                strcpy (text, getModifierName (param[index]));
            else
                strcpy (text, " ");
        }
        else if (data == 3)
        {
            if (param[slot * 4] >= 0.1)
                strcpy (text, getKeyName (FLOAT_TO_KEYCODE (param[index])));
            else
                strcpy (text, " ");
        }

        if (index == kChannel)
        {
            if (FLOAT_TO_CHANNEL016 (param[kChannel]) < 1)
                strcpy (text, "Any");
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL016 (param[kChannel]));
        }
        else if (index == kProg)
            getProgramNameIndexed (0, roundToInt (param[kProg] * (kNumPrograms - 1)), text);
    }
}

void midi2qwerty::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    int msg[kNumSlots];
    short note[kNumSlots];
    short key[kNumSlots];
    short mod[kNumSlots];
    for (int s = 0; s < kNumSlots; s++)
    {
        if (param[s * 4] < 0.1f)
            msg[s] = msgOff;
        else if (param[s * 4] < 0.2f)
            msg[s] = msgNote;
        else if (param[s * 4] < 0.3f)
            msg[s] = msgPCUp;
        else if (param[s * 4] < 0.4f)
            msg[s] = msgPCDn;
        else if (param[s * 4] < 0.5f)
            msg[s] = msgCCMax;
        else if (param[s * 4] < 0.6f)
            msg[s] = msgCCMin;
        else if (param[s * 4] < 0.7f)
            msg[s] = msgCCHigh;
        else if (param[s * 4] < 0.8f)
            msg[s] = msgCCLow;
        else if (param[s * 4] < 0.9f)
            msg[s] = msgCCSliderUp;
        else
            msg[s] = msgCCSliderDn;
        note[s] = FLOAT_TO_MIDI (param[s * 4 + 1]);
        mod[s]  = FLOAT_TO_MOD (param[s * 4 + 2]);
        key[s]  = FLOAT_TO_KEYCODE (param[s * 4 + 3]);
    }

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];
        short status       = tomod.midiData[0] & 0xf0;       // scraping  channel
        short channel      = (tomod.midiData[0] & 0x0f) + 1; // isolating channel (1-16)
        short data1        = tomod.midiData[1] & 0x7f;
        short data2        = tomod.midiData[2] & 0x7f;
        if (status == MIDI_NOTEON && data2 == 0)
            status = MIDI_NOTEOFF;
        bool discard = false;

        const short listenchannel = FLOAT_TO_CHANNEL016 (param[kChannel]);

        if (channel == listenchannel || listenchannel == 0)
        { //only look at the selected channel
            int s;
            for (s = 0; s < kNumSlots; s++)
            {
                if (status == MIDI_NOTEON)
                {
                    if (data1 == note[s] && msg[s] == msgNote)
                    {
                        key2 (key[s], mod[s]);
                        discard = true;
                    }
                }
                else if (status == MIDI_NOTEOFF)
                {
                    if (data1 == note[s] && msg[s] == msgNote)
                    {
                        key2 (key[s], mod[s], true);
                        discard = true;
                    }
                }
                else if (status == MIDI_PROGRAMCHANGE)
                {
                    if (msg[s] == msgPCUp)
                    {
                        //program increment
                        if (lastPC[channel - 1] == 127 && data1 == 0)
                        {
                            key2 (key[s], mod[s], false, true);
                            discard = true;
                        }
                        else if (data1 == lastPC[channel - 1] + 1)
                        {
                            key2 (key[s], mod[s], false, true);
                            discard = true;
                        }
                    }
                    else if (msg[s] == msgPCDn)
                    {
                        //program decrement
                        if (lastPC[channel - 1] == 0 && data1 == 127)
                        {
                            key2 (key[s], mod[s], false, true);
                            discard = true;
                        }
                        else if (data1 == lastPC[channel - 1] - 1)
                        {
                            key2 (key[s], mod[s], false, true);
                            discard = true;
                        }
                    }
                }
                else if (status == MIDI_CONTROLCHANGE)
                {
                    if (data2 == 127)
                    {
                        if (data1 == note[s] && msg[s] == msgCCMax)
                        {
                            key2 (key[s], mod[s], false, true);
                            discard = true;
                        }
                    }
                    else if (data2 == 0)
                    {
                        if (data1 == note[s] && msg[s] == msgCCMin)
                        {
                            key2 (key[s], mod[s], false, true);
                            discard = true;
                        }
                    }
                    if (data2 > 64)
                    {
                        if (data1 == note[s] && msg[s] == msgCCHigh)
                        {
                            key2 (key[s], mod[s], false, true);
                            discard = true;
                        }
                    }
                    else if (data2 < 64)
                    {
                        if (data1 == note[s] && msg[s] == msgCCLow)
                        {
                            key2 (key[s], mod[s], false, true);
                            discard = true;
                        }
                    }
                    if (data2 > 100)
                    {
                        //if in CC- mode, send keyup if key is down
                        if (data1 == note[s] && msg[s] == msgCCSliderDn && keydown[s])
                        {
                            key2 (key[s], mod[s], true);
                            keydown[s] = false;
                            discard    = true;
                        }
                        //if in CC+ mode, send keydown if key isn't already down
                        if (data1 == note[s] && msg[s] == msgCCSliderUp && ! keydown[s])
                        {
                            key2 (key[s], mod[s]);
                            keydown[s] = true;
                            discard    = true;
                        }
                    }
                    else if (data2 < 27)
                    {
                        //if CC+ mode, send keyup if key is down
                        if (data1 == note[s] && msg[s] == msgCCSliderUp && keydown[s])
                        {
                            key2 (key[s], mod[s], true);
                            keydown[s] = false;
                            discard    = true;
                        }
                        //in CC- mode, send keydown if key isn't already down
                        if (data1 == note[s] && msg[s] == msgCCSliderDn && ! keydown[s])
                        {
                            key2 (key[s], mod[s]);
                            keydown[s] = true;
                            discard    = true;
                        }
                    }
                    else
                    {
                        //27-100 sends keyup in either CC+ or CC- mode
                        if (data1 == note[s] && (msg[s] == msgCCSliderUp || msg[s] == msgCCSliderDn) && keydown[s])
                        {
                            key2 (key[s], mod[s], true);
                            keydown[s] = false;
                            discard    = true;
                        }
                    }
                }
            }
        }
        if (status == MIDI_PROGRAMCHANGE)
            lastPC[channel - 1] = data1;
        if (! discard)
            outputs[0].push_back (tomod);
    }
}
