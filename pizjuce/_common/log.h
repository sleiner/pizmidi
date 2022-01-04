#ifndef PIZ_LOG_H
#define PIZ_LOG_H

#if MAKE_LOG || defined (_DEBUG)

#ifndef MAKE_LOG
	#define MAKE_LOG 1
#endif
#ifdef _WIN32
	#include <windows.h>
	//BOOL stupid = AllocConsole();
	//FILE* dumb = freopen ("CONOUT$", "w", stdout);
#endif

#include "JuceHeader.h"
#include "JucePluginCharacteristics.h"
inline void logDebugString(const String& text, bool toFile=false, bool toDbgview=true, bool toConsole=true)
{
    if (toFile) {
        File logfile = File(String("~/Desktop/") + String(JucePlugin_Name) + String(".log"));
        if (logfile.create()) {
            logfile.appendText(text + "\n");
        }
    }
    #ifdef _WIN32
	if (toDbgview) OutputDebugString(String(String(JucePlugin_Name) + String(": ") + text + String("\n")).toUTF8());
	if (toConsole) printf(String(String(JucePlugin_Name) + String(": ") + text + String("\n")).toUTF8());
	#else 
	printf(String(JucePlugin_PluginCode + ": " + text + "\n").toUTF8());
    #endif
}
#else 
	#define logDebugString(i)
#endif

#endif