#ifndef PIZVSTBASE_H
#define PIZVSTBASE_H

#include "CVSTHost.h"
#include "public.sdk/source/vst2.x/audioeffectx.h"
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <Shlobj.h>
#include <windows.h>
#endif

#ifdef _DEBUG
#define dbg(i) std::cout << i << "\n";
#define dbg2(i) std::cout << i;
#else
#define dbg(i)
#define dbg2(i)
#endif

typedef std::vector<VstMidiEvent> VstMidiEventVec;
typedef std::vector<VstMidiSysexEvent> VstSysexEventVec;

#define MAX_EVENTS_PER_TIMESLICE 256

struct MyVstEvents
{
    VstInt32 numEvents;
    VstIntPtr reserved;
    VstEvent* events[MAX_EVENTS_PER_TIMESLICE];
};

bool getInstancePath (char* outInstancePath, char* fileName, bool hostpath = true);
bool getAppDataPath (char* path, const char* name);

inline void removeExtension (char* filename)
{
    if (! filename)
        return;
    char* lastdot = strrchr (filename, '.');
    if (lastdot)
        *lastdot = '\0';
}

inline void getHostStuff (char* host, bool& inst, int& numoutputs, bool& ignoreDefault)
{
    ignoreDefault = false;
    dbg ("host vendor: " << host);
    if (strncmp ("Ableton", host, 7) == 0
        || strncmp ("Steinberg", host, 9) == 0
        || strncmp ("Twelve Tone", host, 11) == 0
        || strncmp ("Image-Line", host, 10) == 0
        || strncmp ("brainspawn", host, 10) == 0)
    {
        inst = true;
        numoutputs = 2;
        ignoreDefault = true;
    }
    else if (strncmp ("Polac", host, 5) == 0)
    {
        inst = true;
        numoutputs = 0;
        ignoreDefault = true;
    }
    else if (strncmp ("Native Instruments", host, 18) == 0
             || strncmp ("Music OS", host, 8) == 0)
    {
        inst = false;
        numoutputs = 2;
        ignoreDefault = true;
    }
    else if (strncmp ("PreSonus", host, 8) == 0)
    {
        inst = true;
        numoutputs = 2;
        ignoreDefault = true;
    }
    else if (strncmp ("Pedalboard 2", host, 12) == 0
             || strncmp ("Juce VST Host", host, 13) == 0
             || strncmp ("Plogue", host, 6) == 0
             || strncmp ("Ross Bencina", host, 12) == 0
             || strncmp ("MuTools.com", host, 11) == 0
             || strncmp ("XT Software", host, 11) == 0)
    {
        inst = false;
        numoutputs = 0;
        ignoreDefault = true;
    }
}

inline void readIniFile (char* host, bool& inst, int& numinputs, int& numoutputs, int& bottomOctave, bool ignoreDefault)
{
    dbg ("host product: " << host);
    char* path;
    char* name;
    path = new char[512];
    name = new char[512];
    getInstancePath (path, name, false);
    if (! strncmp (name, "midiAudioToCC", 13))
        inst = false;

    strcat (path, "pizmidi.ini");
    FILE* file = fopen (path, "r");
    if (file == NULL)
    {
        getAppDataPath (path, "pizmidi");
        strcat (path, "pizmidi.ini");
        file = fopen (path, "r");
    }
    if (file != NULL)
    {
        char line[128] = " ";
        bool hostmatch = false;
        char hostname[128] = "host=";
        strcat (hostname, host);
        strcat (hostname, "\n");
        dbg ("====== pizmidi.ini =======");
        while (fgets (line, sizeof line, file) != NULL)
        {
            dbg2 (line);
            if (line[0] != ';')
            {
                if (! strncmp ("default", line, 7) && ! ignoreDefault)
                {
                    hostmatch = true;
                }
                else if (! strcmp (hostname, line))
                {
                    hostmatch = true;
                }
                else if (! strncmp ("host=", line, 5))
                {
                    hostmatch = false;
                }
                if (hostmatch)
                {
                    if (strncmp ("force effect", line, 12) == 0)
                        inst = false;
                    if (strncmp ("force inst", line, 10) == 0)
                        inst = true;
                    if (strncmp ("audio outs", line, 10) == 0)
                        numoutputs = 2;
                    if (strncmp ("no audio outs", line, 13) == 0)
                        numoutputs = 0;
                    if (strncmp ("audio ins", line, 9) == 0)
                        numinputs = 2;
                    if (strncmp ("no audio ins", line, 12) == 0)
                        numinputs = 0;
                    if (strncmp ("bottom octave=", line, 14) == 0)
                    {
                        bottomOctave = atoi ((const char*) (&line[14]));
                    }
                }
            }
        }
        dbg ("====== end of pizmidi.ini =======\n");
        fclose (file);
    }
    char* pch = strchr (name, '_');
    if (pch)
    {
        if (! strncmp ("_inst.", pch, 6))
        {
            inst = true;
            numoutputs = 2;
        }
        else if (! strncmp ("_nofx.", pch, 6))
        {
            inst = true;
            numoutputs = 2;
        }
        else if (! strncmp ("_fx.", pch, 4))
        {
            inst = false;
            numinputs = 2;
            numoutputs = 2;
        }
    }
    dbg ("pizmidi.ini path: " << path);
    dbg ((inst ? "instrument" : "effect"));
    dbg ("ins: " << numinputs);
    dbg ("outs: " << numoutputs);
    delete path;
    delete name;
}

inline bool readDefaultBank (char* plugName, CFxBank* bank)
{
    char* path;
    char* adpath;
    char* name;
    path = new char[512];
    adpath = new char[512];
    name = new char[512];
    getInstancePath (path, name, false);
    strcat (name, ".fxb");
    strcat (path, name);
    if (getAppDataPath (adpath, "pizmidi"))
    {
        strcat (adpath, name);
        FILE* file = fopen (adpath, "r");
        if (file != NULL)
        {
            strcpy (path, adpath);
        }
    }
    bank->LoadBank (path);
    if (bank->LoadBank (path))
        return true;
    return false;
}

inline bool readDefaultBank (char* plugName, CFxBank* bank, VstInt32 ID)
{
    char* path;
    char* name;
    path = new char[512];
    name = new char[512];
    if (! getAppDataPath (path, "pizmidi"))
        getInstancePath (path, name, false);
    strcat (path, plugName);
    strcat (path, ".fxb");
    bank->LoadBank (path);
    if (bank->LoadBank (path))
    {
        return (bank->GetFxID() == ID);
    }
    return false;
}

#endif
