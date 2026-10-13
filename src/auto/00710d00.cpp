#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(int, __fastcall, C_00863ba8_LoadSoundFile, 0x0070F630, (C_00863ba8* self, void* , const char* path, const char* name, char forceMono, int quality));
CSF_FUNC(char*, __cdecl, __strupr, 0x007B302E, (char* String));
CSF_FUNC(int, __cdecl, RtDictFindNamedEntry, 0x007DBD20, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwAudioEffects_GetDictionary, 0x007DBE90, ());

// Looks up a named sound effect in the RW audio effects dictionary; if missing and
// shouldLoad is set, loads it from disk via the sound object's LoadSoundFile.
static int __fastcall C_00863ba8_FindOrLoadSoundEffect(C_00863ba8* self, void* /*edx*/,
                                                       char* source, char forceMono,
                                                       char shouldLoad, int quality)
{
    if (!source || !RwAudioEffects_GetDictionary())
        return 0;

    char normalizedName[80];
    NormalizeResourceName(source, normalizedName, 80);
    __strupr(normalizedName);

    int dictionary = RwAudioEffects_GetDictionary();
    int result = RtDictFindNamedEntry(dictionary, reinterpret_cast<int>(normalizedName));
    if (!result) {
        if (shouldLoad)
            return C_00863ba8_LoadSoundFile(self, nullptr, source, normalizedName, forceMono, quality);
    }
    return result;
}

HOOK(0x00710D00, C_00863ba8_FindOrLoadSoundEffect, "auto");
