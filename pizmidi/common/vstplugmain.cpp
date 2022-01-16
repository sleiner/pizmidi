#include "pizvstbase.h"
#include "public.sdk/source/vst2.x/audioeffect.h"

#if defined(_WIN32) && defined(_DEBUG)
BOOL stupid = AllocConsole();
FILE* dumb  = freopen ("CONOUT$", "w", stdout);
#endif

//just for fxb loading:
#include "CVSTHost.cpp"

//------------------------------------------------------------------------
/** Must be implemented externally. */
extern AudioEffect* createEffectInstance (audioMasterCallback audioMaster);

extern "C"
{
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define VST_EXPORT __attribute__ ((visibility ("default")))
#elif defined(_WIN32)
#define VST_EXPORT __declspec(dllexport)
#else
#define VST_EXPORT
#endif

    //------------------------------------------------------------------------
    /** Prototype of the export function main */
    //------------------------------------------------------------------------
    VST_EXPORT AEffect* VSTPluginMain (audioMasterCallback audioMaster)
    {
        // Get VST Version of the Host
        if (! audioMaster (0, audioMasterVersion, 0, 0, 0, 0))
            return 0; // old version

        // Create the AudioEffect
        AudioEffect* effect = createEffectInstance (audioMaster);
        if (! effect)
            return 0;

        // Return the VST AEffect structur
        return effect->getAeffect();
    }

    // support for old hosts not looking for VSTPluginMain
#if (TARGET_API_MAC_CARBON && __ppc__)
    VST_EXPORT AEffect* main_macho (audioMasterCallback audioMaster)
    {
        return VSTPluginMain (audioMaster);
    }
#elif defined(_WIN32) && ! defined(_WIN64)
    VST_EXPORT void* main (audioMasterCallback audioMaster)
    {
        return (void*) VSTPluginMain (audioMaster);
    }
#elif BEOS
    VST_EXPORT AEffect* main_plugin (audioMasterCallback audioMaster)
    {
        return VSTPluginMain (audioMaster);
    }
#elif defined(__linux__)
    AEffect* main_plugin (audioMasterCallback audioMaster) asm("main");
#endif
}

//------------------------------------------------------------------------
#if _WIN32
HINSTANCE hInstance;

extern "C"
{
    BOOL WINAPI DllMain (HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
    {
        hInstance = hInst;
        return 1;
    }
}

bool getAppDataPath (char* path, const char* name)
{
    if (path == NULL || name == NULL)
        return false;

    char temp[256];
    if (SHGetSpecialFolderPathA (0, temp, CSIDL_APPDATA, 0))
    {
        sprintf (path, "%s\\%s\\", temp, name);
        dbg ("getAppDataPath: " << path);
    }
    else
    {
        dbg ("getAppDataPath failed");
        return false;
    }

    const DWORD attr = GetFileAttributes (path);

    return (attr != 0xffffffff) && ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

bool getInstancePath (char* outInstancePath, char* fileName, bool hostpath)
{
    if (outInstancePath == NULL || fileName == NULL)
        return false;

    WCHAR filename[256];
    memset (filename, 0, 256 * sizeof (WCHAR));

    DWORD success = 0;
    if (hostpath)
        success = GetModuleFileNameW (NULL, filename, 255);
    else
        success = GetModuleFileNameW ((HMODULE) hInstance, filename, 255);

    if (success)
        int err = WideCharToMultiByte (0, 0, filename, success, outInstancePath, 511, NULL, NULL);
    else
        success = GetModuleFileNameA ((HMODULE) hInstance, outInstancePath, 255);
    if (! success)
    {
        dbg ("Failed to get filename");
    }

    const char* checkString = hostpath ? ".exe" : ".dll";

    int foundloc = 0;
    int countloc = 0;

    for (int i = 0; i < 256; i++)
    {
        if (outInstancePath[i] == checkString[foundloc++])
        {
            if (foundloc == 4)
                break;
        }
        else
            foundloc = 0;

        countloc++;
    }

    if (foundloc < 4)
    {
        dbg ("scanning path for .dll did not succeed!");
        dbg (outInstancePath);
        return false;
    }
    outInstancePath[countloc + 1] = 0;
    char* tmp                     = strrchr (outInstancePath, '\\');
    if (tmp)
    {
        tmp++;
        strcpy (fileName, tmp);
        removeExtension (fileName);
    }

    countloc -= 4;

    if (countloc < 3)
    {
        dbg ("scanning path for .dll countloc <3..!");
        countloc = 4;
    }

    for (int i = countloc; i > 0; i--)
    {
        if (outInstancePath[i] == '\\')
        {
            outInstancePath[i + 1] = '\0';
            break;
        }
    }
    dbg ("path: " << outInstancePath << fileName << checkString);
    return true;
}

#elif defined(__linux__)
bool getAppDataPath (char* path, const char* name)
{
    if (path == NULL || name == NULL)
        return false;

    sprintf (path, "~/.%s/", name);
    return true;
}

bool getInstancePath (char* outInstancePath, char* fileName, bool hostpath)
{
    if (outInstancePath == NULL || fileName == NULL)
        return;

    strcpy (outInstancePath, "~/.pizmidi/");
    strcpy (fileName, "?");
}
#else
//mac
bool getAppDataPath (char* path, const char* name)
{
    if (path == NULL || name == NULL)
        return false;

    sprintf (path, "~/Library/%s/", name);
    return true;
}

bool getInstancePath (char* outInstancePath, char* fileName, bool hostpath)
{
    if (outInstancePath == NULL || fileName == NULL)
        return false;

    strcpy (outInstancePath, "~/Library/pizmidi/");
    strcpy (fileName, "?");
    return true;
}
#endif
