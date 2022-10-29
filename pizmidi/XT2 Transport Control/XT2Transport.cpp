#include "XT2Transport.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new XT2TransportControl(audioMaster);
}

XT2TransportControlProgram::XT2TransportControlProgram()
{
    // default Program Values
    fPauseNote  = MIDI_TO_FLOAT_X(0);
    fPauseMsg   = 0.0f;
    fStopNote   = MIDI_TO_FLOAT_X(1);
    fStopMsg    = 0.0f;
    fRewindNote = MIDI_TO_FLOAT_X(2);
    fRewindMsg  = 0.0f;
    fRecordNote = MIDI_TO_FLOAT_X(3);
    fRecordMsg  = 0.0f;
    fChannel    = CHANNEL_TO_FLOAT(0); //0==any channel

    strcpy(name, "default");
}

//-----------------------------------------------------------------------------
XT2TransportControl::XT2TransportControl(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new XT2TransportControlProgram[numPrograms];

    //make built-in programs
    int i;
    for (i = 0; i < kNumPrograms; i++)
    {
        switch (i)
        {
            case 0:
                programs[i].fPauseNote  = MIDI_TO_FLOAT_X(0);
                programs[i].fPauseMsg   = 0.0f;
                programs[i].fStopNote   = MIDI_TO_FLOAT_X(1);
                programs[i].fStopMsg    = 0.0f;
                programs[i].fRewindNote = MIDI_TO_FLOAT_X(2);
                programs[i].fRewindMsg  = 0.0f;
                programs[i].fRecordNote = MIDI_TO_FLOAT_X(3);
                programs[i].fRecordMsg  = 0.0f;
                programs[i].fChannel    = CHANNEL_TO_FLOAT(0);
                sprintf(programs[i].name, "Notes\0");
                break;
            case 1:
                programs[i].fPauseNote  = MIDI_TO_FLOAT_X(0);
                programs[i].fPauseMsg   = 1.0f;
                programs[i].fStopNote   = MIDI_TO_FLOAT_X(1);
                programs[i].fStopMsg    = 1.0f;
                programs[i].fRewindNote = MIDI_TO_FLOAT_X(2);
                programs[i].fRewindMsg  = 1.0f;
                programs[i].fRecordNote = MIDI_TO_FLOAT_X(3);
                programs[i].fRecordMsg  = 1.0f;
                programs[i].fChannel    = CHANNEL_TO_FLOAT(0);
                sprintf(programs[i].name, "CCs\0");
                break;
            case 2:
                programs[i].fPauseNote  = MIDI_TO_FLOAT_X(0);
                programs[i].fPauseMsg   = 0.5f;
                programs[i].fStopNote   = MIDI_TO_FLOAT_X(1);
                programs[i].fStopMsg    = 0.5f;
                programs[i].fRewindNote = MIDI_TO_FLOAT_X(2);
                programs[i].fRewindMsg  = 0.5f;
                programs[i].fRecordNote = MIDI_TO_FLOAT_X(3);
                programs[i].fRecordMsg  = 0.5f;
                programs[i].fChannel    = CHANNEL_TO_FLOAT(0);
                sprintf(programs[i].name, "ProgChanges\0");
                break;
        }
    }

    if (programs)
        setProgram(0);

    init();
}

#if WIN32
#include <psapi.h> // NT only!
#include <windows.h>

//finds the energyXT window by title:
HWND xtWindow = NULL;
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    char String[255];
    if (! hWnd)
        return TRUE; // Not a window
    if (! ::IsWindowVisible(hWnd))
        return TRUE; // Not visible
    if (! SendMessage(hWnd, WM_GETTEXT, sizeof(String), (LPARAM) String))
        return TRUE; // No window title
    else if (strncmp(String, (char*) lParam, strlen((char*) lParam)) == 0)
    {
        //window has correct name; check if it's from "energyXT.exe"
        HINSTANCE hInstance = (HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE);
        DWORD dwProcessId;
        DWORD dwThreadId = GetWindowThreadProcessId(hWnd, &dwProcessId);
        HANDLE hProcess  = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
        if (GetModuleFileNameEx(hProcess, hInstance, String, sizeof(String)))
        {
            char* xtfile = strstr(String, XT_EXE_NAME);
            if (xtfile)
            {
                if (strncmp(xtfile, XT_EXE_NAME, strlen(XT_EXE_NAME)) == 0)
                {
                    xtWindow = hWnd;
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

void XT2TransportControl::XT2_Transport(int button)
{
    //find the energyXT window:
    xtWindow = NULL;
    EnumWindows(EnumWindowsProc, (LPARAM) XT_WINDOW_TITLE);

    if (xtWindow != NULL)
    {
        //transport buttons:
        if (button == 0)
            SendMessage(xtWindow, WM_KEYDOWN, VK_RETURN, 0); //play/pause
        else if (button == 1)
            SendMessage(xtWindow, WM_KEYDOWN, '0', 0); //stop
        else if (button == 2)
            SendMessage(xtWindow, WM_KEYDOWN, '3', 0); //rewind
        else if (button == 3)
            SendMessage(xtWindow, WM_KEYDOWN, 'R', 0); //record
    }
}

#else
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>
//figure out linux method here

void XT2TransportControl::XT2_Transport(int button)
{
    Display* display = XOpenDisplay(NULL);
    if (button == 0)
    {
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, ' '), True, 0); //space
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, ' '), False, 0);
    }
    else if (button == 1)
    {
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, '0'), True, 0); //0
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, '0'), False, 0);
    }
    else if (button == 2)
    {
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, '3'), True, 0); //3
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, '3'), False, 0);
    }
    else if (button == 3)
    {
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, 'R'), True, 0); //R
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, 'R'), False, 0);
    }
    XCloseDisplay(display);
}
#endif

//-----------------------------------------------------------------------------------------
XT2TransportControl::~XT2TransportControl()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void XT2TransportControl::setProgram(VstInt32 program)
{
    XT2TransportControlProgram* ap = &programs[program];

    curProgram = program;
    setParameter(kPauseNote, ap->fPauseNote);
    setParameter(kPauseMsg, ap->fPauseMsg);
    setParameter(kStopNote, ap->fStopNote);
    setParameter(kStopMsg, ap->fStopMsg);
    setParameter(kRewindNote, ap->fRewindNote);
    setParameter(kRewindMsg, ap->fRewindMsg);
    setParameter(kRecordNote, ap->fRecordNote);
    setParameter(kRecordMsg, ap->fRecordMsg);
    setParameter(kChannel, ap->fChannel);
}

//------------------------------------------------------------------------
void XT2TransportControl::setProgramName(char* name)
{
    strcpy(programs[curProgram].name, name);
}

//------------------------------------------------------------------------
void XT2TransportControl::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool XT2TransportControl::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void XT2TransportControl::setParameter(VstInt32 index, float value)
{
    XT2TransportControlProgram* ap = &programs[curProgram];

    switch (index)
    {
        case kPauseNote:
            fPauseNote = ap->fPauseNote = value;
            break;
        case kPauseMsg:
            fPauseMsg = ap->fPauseMsg = value;
            break;
        case kStopNote:
            fStopNote = ap->fStopNote = value;
            break;
        case kStopMsg:
            fStopMsg = ap->fStopMsg = value;
            break;
        case kRewindNote:
            fRewindNote = ap->fRewindNote = value;
            break;
        case kRewindMsg:
            fRewindMsg = ap->fRewindMsg = value;
            break;
        case kRecordNote:
            fRecordNote = ap->fRecordNote = value;
            break;
        case kRecordMsg:
            fRecordMsg = ap->fRecordMsg = value;
            break;
        case kChannel:
            fChannel = ap->fChannel = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float XT2TransportControl::getParameter(VstInt32 index)
{
    float v = 0;

    switch (index)
    {
        case kPauseNote:
            v = fPauseNote;
            break;
        case kPauseMsg:
            v = fPauseMsg;
            break;
        case kStopNote:
            v = fStopNote;
            break;
        case kStopMsg:
            v = fStopMsg;
            break;
        case kRewindNote:
            v = fRewindNote;
            break;
        case kRewindMsg:
            v = fRewindMsg;
            break;
        case kRecordNote:
            v = fRecordNote;
            break;
        case kRecordMsg:
            v = fRecordMsg;
            break;
        case kChannel:
            v = fChannel;
            break;

        default:
            break;
    }
    return v;
}

//-----------------------------------------------------------------------------------------
void XT2TransportControl::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kPauseNote:
            strcpy(label, "Play #");
            break;
        case kPauseMsg:
            strcpy(label, "Play Msg");
            break;
        case kStopNote:
            strcpy(label, "Stop #");
            break;
        case kStopMsg:
            strcpy(label, "Stop Msg");
            break;
        case kRewindNote:
            strcpy(label, "Rewind #");
            break;
        case kRewindMsg:
            strcpy(label, "Rewind Msg");
            break;
        case kRecordNote:
            strcpy(label, "Record #");
            break;
        case kRecordMsg:
            strcpy(label, "Record Msg");
            break;
        case kChannel:
            strcpy(label, "Channel");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void XT2TransportControl::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kPauseNote:
            if (FLOAT_TO_MIDI_X(fPauseNote) == -1)
                strcpy(text, "Learn...");
            else if (fPauseMsg >= 0.33)
                sprintf(text, "%d", FLOAT_TO_MIDI_X(fPauseNote));
            else
                strcpy(text, getNoteName(FLOAT_TO_MIDI_X(fPauseNote), bottomOctave));
            break;

        case kPauseMsg:
            if (fPauseMsg < 0.33)
                strcpy(text, "Note");
            else if (fPauseMsg < 0.66)
                strcpy(text, "Prog");
            else
                strcpy(text, "CC");
            break;

        case kStopNote:
            if (FLOAT_TO_MIDI_X(fStopNote) == -1)
                strcpy(text, "Learn...");
            else if (fStopMsg >= 0.33)
                sprintf(text, "%d", FLOAT_TO_MIDI_X(fStopNote));
            else
                strcpy(text, getNoteName(FLOAT_TO_MIDI_X(fStopNote), bottomOctave));
            break;

        case kStopMsg:
            if (fStopMsg < 0.33)
                strcpy(text, "Note");
            else if (fStopMsg < 0.66)
                strcpy(text, "Prog");
            else
                strcpy(text, "CC");
            break;

        case kRewindNote:
            if (FLOAT_TO_MIDI_X(fRewindNote) == -1)
                strcpy(text, "Learn...");
            else if (fRewindMsg >= 0.33)
                sprintf(text, "%d", FLOAT_TO_MIDI_X(fRewindNote));
            else
                strcpy(text, getNoteName(FLOAT_TO_MIDI_X(fRewindNote), bottomOctave));
            break;

        case kRewindMsg:
            if (fRewindMsg < 0.33)
                strcpy(text, "Note");
            else if (fRewindMsg < 0.66)
                strcpy(text, "Prog");
            else
                strcpy(text, "CC");
            break;

        case kRecordNote:
            if (FLOAT_TO_MIDI_X(fRecordNote) == -1)
                strcpy(text, "Learn...");
            else if (fRecordMsg >= 0.33)
                sprintf(text, "%d", FLOAT_TO_MIDI_X(fRecordNote));
            else
                strcpy(text, getNoteName(FLOAT_TO_MIDI_X(fRecordNote), bottomOctave));
            break;

        case kRecordMsg:
            if (fRecordMsg < 0.33)
                strcpy(text, "Note");
            else if (fRecordMsg < 0.66)
                strcpy(text, "Prog");
            else
                strcpy(text, "CC");
            break;

        case kChannel:
            if (FLOAT_TO_CHANNEL(fChannel) < 1)
                strcpy(text, "Any");
            else
                sprintf(text, "%d", FLOAT_TO_CHANNEL(fChannel));
            break;

        default:
            break;
    }
}

void XT2TransportControl::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    int pausenote     = FLOAT_TO_MIDI_X(fPauseNote);
    int stopnote      = FLOAT_TO_MIDI_X(fStopNote);
    int rewnote       = FLOAT_TO_MIDI_X(fRewindNote);
    int recnote       = FLOAT_TO_MIDI_X(fRecordNote);
    int listenchannel = FLOAT_TO_CHANNEL(fChannel);

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];
        int status         = tomod.midiData[0] & 0xf0;       // scraping  channel
        int channel        = (tomod.midiData[0] & 0x0f) + 1; // isolating channel (1-16)
        int data1          = tomod.midiData[1] & 0x7f;
        int data2          = tomod.midiData[2] & 0x7f;

        if (channel == listenchannel || listenchannel == 0)
        { //only look at the selected channel

            if (status == MIDI_NOTEON && data2 > 0)
            {
                if (data1 == pausenote && fPauseMsg < 0.33)
                    XT2_Transport(0);
                if (data1 == stopnote && fStopMsg < 0.33)
                    XT2_Transport(1);
                if (data1 == rewnote && fRewindMsg < 0.33)
                    XT2_Transport(2);
                if (data1 == recnote && fRecordMsg < 0.33)
                    XT2_Transport(3);

                //midi learn
                if (pausenote == -1)
                {
                    setParameterAutomated(kPauseNote, MIDI_TO_FLOAT_X(data1));
                    setParameterAutomated(kPauseMsg, 0.f);
                }
                else if (stopnote == -1)
                {
                    setParameterAutomated(kStopNote, MIDI_TO_FLOAT_X(data1));
                    setParameterAutomated(kStopMsg, 0.f);
                }
                else if (rewnote == -1)
                {
                    setParameterAutomated(kRewindNote, MIDI_TO_FLOAT_X(data1));
                    setParameterAutomated(kRewindMsg, 0.f);
                }
                else if (recnote == -1)
                {
                    setParameterAutomated(kRecordNote, MIDI_TO_FLOAT_X(data1));
                    setParameterAutomated(kRecordMsg, 0.f);
                }
            }
            else if (status == MIDI_CONTROLCHANGE && data2 == 127)
            {
                if (data1 == pausenote && fPauseMsg >= 0.66)
                    XT2_Transport(0);
                if (data1 == stopnote && fStopMsg >= 0.66)
                    XT2_Transport(1);
                if (data1 == rewnote && fRewindMsg >= 0.66)
                    XT2_Transport(2);
                if (data1 == recnote && fRecordMsg >= 0.66)
                    XT2_Transport(3);

                //midi learn
                if (pausenote == -1)
                {
                    setParameterAutomated(kPauseNote, MIDI_TO_FLOAT_X(data1));
                    setParameterAutomated(kPauseMsg, 0.5f);
                }
                else if (stopnote == -1)
                {
                    setParameterAutomated(kStopNote, MIDI_TO_FLOAT_X(data1));
                    setParameterAutomated(kStopMsg, 0.5f);
                }
                else if (rewnote == -1)
                {
                    setParameterAutomated(kRewindNote, MIDI_TO_FLOAT_X(data1));
                    setParameterAutomated(kRewindMsg, 0.5f);
                }
                else if (recnote == -1)
                {
                    setParameterAutomated(kRecordNote, MIDI_TO_FLOAT_X(data1));
                    setParameterAutomated(kRecordMsg, 0.5f);
                }
            }
            else if (status == MIDI_PROGRAMCHANGE)
            {
                if (data1 == pausenote && fPauseMsg >= 0.33 && fPauseMsg < 0.66)
                    XT2_Transport(0);
                if (data1 == stopnote && fStopMsg >= 0.33 && fStopMsg < 0.66)
                    XT2_Transport(1);
                if (data1 == rewnote && fRewindMsg >= 0.33 && fRewindMsg < 0.66)
                    XT2_Transport(2);
                if (data1 == recnote && fRecordMsg >= 0.33 && fRecordMsg < 0.66)
                    XT2_Transport(3);

                //midi learn
                if (pausenote == -1)
                {
                    setParameterAutomated(kPauseNote, MIDI_TO_FLOAT_X(data1));
                    setParameterAutomated(kPauseMsg, 1.f);
                }
                else if (stopnote == -1)
                {
                    setParameterAutomated(kStopNote, MIDI_TO_FLOAT_X(data1));
                    setParameterAutomated(kStopMsg, 1.f);
                }
                else if (rewnote == -1)
                {
                    setParameterAutomated(kRewindNote, MIDI_TO_FLOAT_X(data1));
                    setParameterAutomated(kRewindMsg, 1.f);
                }
                else if (recnote == -1)
                {
                    setParameterAutomated(kRecordNote, MIDI_TO_FLOAT_X(data1));
                    setParameterAutomated(kRecordMsg, 1.f);
                }
            }
        }
    }
}
